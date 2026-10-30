////////////////////////////////////////////////////////////////
//
//	Physical memory allocator
//
//	File:	phys.cpp
//	Date:	30 Oct 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


// C++
#include <algorithm>
#include <array>
#include <bit>
#include <initializer_list>
#include <limits>
#include <span>
// IgrOS-Kernel library
#include <klib/kAlign.hpp>
#include <klib/kString.hpp>
// IgrOS-Kernel memory
#include <mem/direct.hpp>
#include <mem/phys.hpp>
// IgrOS-Kernel multiboot
#include <multiboot/multiboot.hpp>
// IgrOS-Kernel platform
#include <platform/platform.hpp>


// Memory code zone
namespace igros::mem {


	// Allocator state
	namespace {


		// Bitmap word
		using word_t = igros_dword_t;

		// Pages per bitmap word
		constexpr auto WORD_BITS	{static_cast<igros_usize_t>(std::numeric_limits<word_t>::digits)};
		// Number of tracked pages
		constexpr auto PAGES		{static_cast<igros_usize_t>(phys::MEMORY_LIMIT >> PAGE_SHIFT)};

		// Free pages bitmap (bit set - page is free)
		// Zero-initialized (.bss): all memory is used until init
		constinit std::array<word_t, PAGES / WORD_BITS>	freeMap		{};
		// Number of free pages
		constinit igros_usize_t				freeCount	{0_usize};
		// No free pages in words below this one
		constinit igros_usize_t				searchHint	{0_usize};
		// End of highest memory marked free
		constinit phys_t				memoryTop	{0_u64};


		// Check if page is free
		[[nodiscard]]
		auto isFree(const igros_usize_t page) noexcept -> bool {
			return 0_u32 != (freeMap[page / WORD_BITS] & (1_u32 << (page % WORD_BITS)));
		}

		// Mark page as free
		void setFree(const igros_usize_t page) noexcept {
			freeMap[page / WORD_BITS] |= (1_u32 << (page % WORD_BITS));
			++freeCount;
			searchHint = std::min(searchHint, page / WORD_BITS);
		}

		// Mark page as used
		void setUsed(const igros_usize_t page) noexcept {
			freeMap[page / WORD_BITS] &= ~(1_u32 << (page % WORD_BITS));
			--freeCount;
		}

		// Mark boot string as used (only first byte if string is not directly mapped)
		void markUsedString(const phys_t address) noexcept {
			const auto* const text {direct::string(address)};
			phys::markUsed(address, address + ((nullptr != text) ? klib::kstrlen(text) : 0_usize) + 1_u64);
		}

		// Mark modules list, modules and their names as used
		void markUsedModules(const multiboot::info_t &info) noexcept {
			if (!info.hasInfoModules()) {
				return;
			}
			const auto modulesSize {static_cast<phys_t>(info.modulesCount) * sizeof(multiboot::moduleEntry)};
			phys::markUsed(info.modulesAddr, info.modulesAddr + modulesSize);
			// Modules can't be found if modules list is not directly mapped
			const auto* const modules {direct::pointer<const multiboot::moduleEntry>(info.modulesAddr, modulesSize)};
			if (nullptr == modules) [[unlikely]] {
				return;
			}
			for (const auto &bootModule : std::span {modules, info.modulesCount}) {
				phys::markUsed(bootModule.start, bootModule.end);
				if (0_u32 != bootModule.name) {
					markUsedString(bootModule.name);
				}
			}
		}


	}	// namespace


	// Initialize from bootloader memory map (reserves kernel and boot data)
	void phys::init(const multiboot::info_t &info) noexcept {

		// Start from scratch: all memory is used
		freeMap.fill(0_u32);
		freeCount	= 0_usize;
		searchHint	= freeMap.size();
		memoryTop	= 0_u64;

		// Available memory from bootloader memory map
		auto hasAvailable {false};
		for (const auto &entry : info.memoryMap()) {
			if (multiboot::MEMORY_MAP_TYPE::AVAILABLE == entry.type) {
				phys::markFree(entry.address, entry.address + entry.length);
				hasAvailable = true;
			}
		}
		// Basic memory info as fallback (upper memory in KB, starting at 1 MB)
		if (!hasAvailable && info.hasInfoMemory()) {
			phys::markFree(0x100000_u64, 0x100000_u64 + (static_cast<phys_t>(info.memHigh) << 10));
		}

		// Low memory (real mode IVT, BIOS data, EBDA, video memory, ROMs)
		phys::markUsed(0_u64, LOW_MEMORY_RESERVED);
		// Kernel image (loaded right above low memory)
		phys::markUsed(0_u64, direct::physOf(platform::Platform::kernelEnd()));

		// Multiboot info structure
		phys::markUsed(direct::physOf(&info), direct::physOf(&info) + sizeof(info));
		// Memory map
		if (info.hasInfoMemoryMap()) {
			phys::markUsed(info.mmapAddr, static_cast<phys_t>(info.mmapAddr) + info.mmapLength);
		}
		// Command line and bootloader name
		if (info.hasInfoCommandLine()) {
			markUsedString(info.cmdLine);
		}
		if (info.hasInfoBootloaderName()) {
			markUsedString(info.bootloaderName);
		}
		// Modules list, modules and their names
		markUsedModules(info);

	}


	// Allocate physical page below limit (0 if no memory left)
	[[nodiscard]]
	auto phys::alloc(const phys_t limit) noexcept -> phys_t {
		// Pages below limit
		const auto limitPages	{static_cast<igros_usize_t>(std::min(limit, MEMORY_LIMIT) >> PAGE_SHIFT)};
		const auto limitWords	{(limitPages + WORD_BITS - 1_usize) / WORD_BITS};
		// Find first word with free page
		for (auto word {searchHint}; word < limitWords; ++word) {
			if (0_u32 != freeMap[word]) {
				const auto page {word * WORD_BITS + static_cast<igros_usize_t>(std::countr_zero(freeMap[word]))};
				// Free page is above limit
				if (page >= limitPages) [[unlikely]] {
					break;
				}
				// Allocate page
				setUsed(page);
				searchHint = word;
				return static_cast<phys_t>(page) << PAGE_SHIFT;
			}
		}
		// Out of memory
		return 0_u64;
	}

	// Free physical page
	void phys::free(const phys_t page) noexcept {
		// Ignore null, unaligned and untracked pages
		if ((0_u64 == page) || !klib::kAlign::check<PAGE_SIZE>(page) || (page >= MEMORY_LIMIT)) [[unlikely]] {
			return;
		}
		// Ignore double free
		const auto index {static_cast<igros_usize_t>(page >> PAGE_SHIFT)};
		if (isFree(index)) [[unlikely]] {
			return;
		}
		// Return page
		setFree(index);
	}


	// Number of free pages
	[[nodiscard]]
	auto phys::freePages() noexcept -> igros_usize_t {
		return freeCount;
	}


	// End of highest memory ever marked free
	[[nodiscard]]
	auto phys::top() noexcept -> phys_t {
		return memoryTop;
	}


	// Mark physical memory range [start, end) as free (only whole pages)
	void phys::markFree(const phys_t start, const phys_t end) noexcept {
		// Clamp to tracked memory
		const auto from	{std::min(start, MEMORY_LIMIT)};
		const auto to	{std::min(end, MEMORY_LIMIT)};
		// Whole pages inside range
		const auto first	{static_cast<igros_usize_t>(klib::kAlign::up<PAGE_SIZE>(from) >> PAGE_SHIFT)};
		const auto last		{static_cast<igros_usize_t>(to >> PAGE_SHIFT)};
		// Top of usable memory (whole pages only)
		if (first < last) {
			memoryTop = std::max(memoryTop, static_cast<phys_t>(last) << PAGE_SHIFT);
		}
		for (auto page {first}; page < last; ++page) {
			if (!isFree(page)) {
				setFree(page);
			}
		}
	}

	// Mark physical memory range [start, end) as used (every touched page)
	void phys::markUsed(const phys_t start, const phys_t end) noexcept {
		// Clamp to tracked memory
		const auto from	{std::min(start, MEMORY_LIMIT)};
		const auto to	{std::min(end, MEMORY_LIMIT)};
		// Every page touched by range
		const auto first	{static_cast<igros_usize_t>(from >> PAGE_SHIFT)};
		const auto last		{static_cast<igros_usize_t>(klib::kAlign::up<PAGE_SIZE>(to) >> PAGE_SHIFT)};
		for (auto page {first}; page < last; ++page) {
			if (isFree(page)) {
				setUsed(page);
			}
		}
	}


}	// namespace igros::mem

