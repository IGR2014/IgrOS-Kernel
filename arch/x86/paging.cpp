////////////////////////////////////////////////////////////////
//
//	Paging (i386 2-level and x86_64 4-level page tables)
//
//	File:	paging.cpp
//	Date:	30 Oct 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


// C++
#include <algorithm>
#include <bit>
#include <span>
// IgrOS-Kernel arch x86
#include <arch/x86/cr.hpp>
#include <arch/x86/exceptions.hpp>
#include <arch/x86/features.hpp>
#include <arch/x86/irq.hpp>
#include <arch/x86/native.hpp>
#include <arch/x86/paging.hpp>
// IgrOS-Kernel library
#include <klib/kAlign.hpp>
#include <klib/kMemory.hpp>
#include <klib/kPrint.hpp>
// IgrOS-Kernel memory
#include <mem/direct.hpp>
// IgrOS-Kernel platform
#include <platform/platform.hpp>


// x86 namespace
namespace igros::x86 {


	// Paging state
	namespace {


		// CR0 write protection bit (kernel can't write to read-only pages)
		constexpr auto CR0_WP	{igros_usize_t {0x00010000}};

		// NX bit value if no-execute is enabled (else 0: NX can't be used)
		constinit paging::entry_t	noExecute	{0};



		// Invalidate TLB entry of virtual address (invlpg takes memory operand)
		void invalidate(const mem::virt_t virt) noexcept {
			::pageInvalidate(std::bit_cast<const igros_byte_t*>(mem::address(virt)));
		}


	}	// namespace


	// Entry flag value
	[[nodiscard]]
	constexpr static auto flag(const paging::FLAGS value) noexcept -> paging::entry_t {
		return static_cast<paging::entry_t>(value);
	}


	// Table at physical address (through direct map)
	[[nodiscard]]
	auto paging::table(const mem::phys_t phys) noexcept -> entry_t* {
		// Only directly mapped memory is reachable through kernel offset
		return mem::direct::pointer<entry_t>(phys, PAGE_SIZE);
	}

	// Allocate empty table (reachable through direct map, 0 if no memory)
	[[nodiscard]]
	auto paging::allocTable() noexcept -> mem::phys_t {
		const auto page {mem::phys::alloc(mem::direct::end())};
		if (0_u64 != page) [[likely]] {
			klib::kmemset(std::span {paging::table(page), ENTRIES}, entry_t {0});
		}
		return page;
	}

	// Index of virtual address in table of given level (0 - page table)
	[[nodiscard]]
	auto paging::index(const mem::virt_t virt, const igros_usize_t level) noexcept -> igros_usize_t {
		return (mem::address(virt) >> (PAGE_SHIFT + INDEX_BITS * level)) & (ENTRIES - 1_usize);
	}


	// Entry of virtual address at given level of given hierarchy (create - allocate missing tables)
	[[nodiscard]]
	auto paging::entry(const mem::phys_t top, const mem::virt_t virt, const bool create, const bool user, const igros_usize_t level) noexcept -> entry_t* {
		auto entries {paging::table(top)};
		// Walk down to table of requested level
		for (auto current {LEVELS - 1_usize}; current > level; --current) {
			// Check entries of current level
			if (nullptr == entries) [[unlikely]] {
				return nullptr;
			}
			auto &next {entries[paging::index(virt, current)]};
			if (0_usize == (next & flag(FLAGS::PRESENT))) {
				// Missing table
				if (!create) {
					return nullptr;
				}
				const auto page {paging::allocTable()};
				if (0_u64 == page) [[unlikely]] {
					return nullptr;
				}
				// Access rights are checked on every level: keep upper levels permissive
				next = static_cast<entry_t>(page) | flag(FLAGS::PRESENT) | flag(FLAGS::WRITABLE) | (user ? flag(FLAGS::USER) : 0_usize);
			} else if (0_usize != (next & flag(FLAGS::HUGE))) {
				// Address belongs to large page
				return nullptr;
			} else if (user) {
				next |= flag(FLAGS::USER);
			}
			entries = paging::table(next & ADDRESS_MASK);
		}
		// Entry of requested level
		return (nullptr != entries) ? &entries[paging::index(virt, level)] : nullptr;
	}

	// Entry value for physical address with flags (drops NX if not enabled)
	[[nodiscard]]
	auto paging::makeEntry(const mem::phys_t phys, const klib::kFlags<FLAGS> flags) noexcept -> entry_t {
		const auto rights {flags.value() & ~ADDRESS_MASK & ~flag(FLAGS::HUGE) & ~(flag(FLAGS::NO_EXECUTE) & ~noExecute)};
		return static_cast<entry_t>(phys) | rights | flag(FLAGS::PRESENT);
	}

	// Map 4 KB page in given hierarchy
	[[nodiscard]]
	auto paging::map(const mem::phys_t top, const mem::phys_t phys, const mem::virt_t virt, const klib::kFlags<FLAGS> flags) noexcept -> bool {
		// Page aligned addresses only
		if (!klib::kAlign::check<PAGE_SIZE>(phys) || !klib::kAlign::check<PAGE_SIZE>(mem::address(virt))) [[unlikely]] {
			return false;
		}
		// Physical address must fit into entry
		if (0_u64 != (phys & ~static_cast<mem::phys_t>(ADDRESS_MASK))) [[unlikely]] {
			return false;
		}
		const auto page {paging::entry(top, virt, true, flags.isSet(FLAGS::USER))};
		if (nullptr == page) [[unlikely]] {
			return false;
		}
		*page = paging::makeEntry(phys, flags);
		invalidate(virt);
		return true;
	}

	// Map large page in given hierarchy (fresh directory entry only)
	[[nodiscard]]
	auto paging::mapLarge(const mem::phys_t top, const mem::phys_t phys, const mem::virt_t virt, const klib::kFlags<FLAGS> flags) noexcept -> bool {
		// Large page aligned addresses only, physical address must fit into entry
		if (!klib::kAlign::check<LARGE_PAGE_SIZE>(phys) || !klib::kAlign::check<LARGE_PAGE_SIZE>(mem::address(virt)) ||
			(0_u64 != (phys & ~static_cast<mem::phys_t>(ADDRESS_MASK)))) [[unlikely]] {
			return false;
		}
		const auto large {paging::entry(top, virt, true, flags.isSet(FLAGS::USER), 1_usize)};
		// Don't replace page table (its pages would leak)
		if ((nullptr == large) || (0_usize != (*large & flag(FLAGS::PRESENT)) && (0_usize == (*large & flag(FLAGS::HUGE))))) [[unlikely]] {
			return false;
		}
		*large = paging::makeEntry(phys, flags) | flag(FLAGS::HUGE);
		invalidate(virt);
		return true;
	}


	// Map 4 KB page
	[[nodiscard]]
	auto paging::map(const mem::phys_t phys, const mem::virt_t virt, const klib::kFlags<FLAGS> flags) noexcept -> bool {
		return paging::map(paging::root(), phys, virt, flags);
	}

	// Unmap 4 KB page
	void paging::unmap(const mem::virt_t virt) noexcept {
		const auto page {paging::entry(paging::root(), virt, false, false)};
		if (nullptr != page) {
			*page = 0_usize;
			invalidate(virt);
		}
	}


	// Physical address of virtual address (INVALID if not mapped)
	[[nodiscard]]
	auto paging::translate(const mem::virt_t virt) noexcept -> mem::phys_t {
		const auto address	{static_cast<mem::phys_t>(mem::address(virt))};
		auto entries		{paging::table(paging::root())};
		for (auto step {0_usize}; step < LEVELS; ++step) {
			const auto level {LEVELS - 1_usize - step};
			// Check entries of current level
			if (nullptr == entries) [[unlikely]] {
				return INVALID;
			}
			const auto current {entries[paging::index(virt, level)]};
			if (0_usize == (current & flag(FLAGS::PRESENT))) {
				return INVALID;
			}
			// Page (or large page) covering address
			if ((0_usize == level) || (0_usize != (current & flag(FLAGS::HUGE)))) {
				const auto size {mem::phys_t {1} << (PAGE_SHIFT + INDEX_BITS * level)};
				return (static_cast<mem::phys_t>(current & ADDRESS_MASK) & ~(size - 1_u64)) | (address & (size - 1_u64));
			}
			entries = paging::table(current & ADDRESS_MASK);
		}
		return INVALID;
	}

	// Current top level table physical address
	[[nodiscard]]
	auto paging::root() noexcept -> mem::phys_t {
		return static_cast<mem::phys_t>(::readCR3() & ADDRESS_MASK);
	}


	// End of physical memory reachable through direct map
	[[nodiscard]]
	auto paging::directMapEnd() noexcept -> mem::phys_t {
		return mem::direct::end();
	}

	// Start of kernel dynamic mappings (right above direct map)
	[[nodiscard]]
	auto paging::dynamicStart() noexcept -> igros_byte_t* {
		return std::bit_cast<igros_byte_t*>(platform::Platform::kernelOffset() + static_cast<igros_usize_t>(DIRECT_MAP_LIMIT));
	}


	// Rights of kernel offset mapping of low memory physical page (kernel sections)
	[[nodiscard]]
	auto paging::kernelFlags(const mem::phys_t phys) noexcept -> klib::kFlags<FLAGS> {
		using platform::Platform;
		const auto virt {std::bit_cast<const igros_byte_t*>(static_cast<igros_usize_t>(phys) + Platform::kernelOffset())};
		// Code: read + execute
		if ((virt >= Platform::textStart()) && (virt < Platform::rodataStart())) {
			return klib::make_kflags<FLAGS>(FLAGS::PRESENT);
		}
		// Read-only data: read
		if ((virt >= Platform::rodataStart()) && (virt < Platform::dataStart())) {
			return klib::make_kflags<FLAGS>(FLAGS::PRESENT, FLAGS::NO_EXECUTE);
		}
		// Data, bss, page tables, low memory, video memory: read + write
		return klib::make_kflags<FLAGS>(FLAGS::PRESENT, FLAGS::WRITABLE, FLAGS::NO_EXECUTE);
	}


	// Build kernel page tables and switch to them
	void paging::init() noexcept {

		// Page fault handler
		except::install<except::NUMBER::PAGE_FAULT, paging::exHandler>();

		// No-execute protection (before any NX bit is written: it's reserved until enabled)
		noExecute = ::cpuEnableNoExecute() ? flag(FLAGS::NO_EXECUTE) : 0_usize;

		// Top level table (tables come from memory mapped by current page tables)
		const auto top {paging::allocTable()};
		if (0_u64 == top) [[unlikely]] {
			klib::kprintf("Paging:\t\tFAILED (no memory for page tables)");
			return;
		}

		// Low memory: at kernel offset with kernel section rights and identity (boot data);
		// identity null page stays unmapped to catch null pointer dereference,
		// boot stack guard page stays unmapped to catch kernel stack overflow
		const auto data		{klib::make_kflags<FLAGS>(FLAGS::PRESENT, FLAGS::WRITABLE, FLAGS::NO_EXECUTE)};
		const auto offset	{platform::Platform::kernelOffset()};
		const auto guard	{static_cast<mem::phys_t>(std::bit_cast<igros_usize_t>(platform::Platform::stackGuard()) - offset)};
		auto mapped		{true};
		for (auto phys {0_u64}; phys < mem::LOW_MEMORY_LIMIT; phys += PAGE_SIZE) {
			const auto page {static_cast<igros_usize_t>(phys)};
			if (guard != phys) {
				mapped = mapped && paging::map(top, phys, static_cast<mem::virt_t>(page + offset), paging::kernelFlags(phys));
			}
			if (0_u64 != phys) {
				mapped = mapped && paging::map(top, phys, static_cast<mem::virt_t>(page), data);
			}
		}

		// Rest of RAM at kernel offset (direct map) with large pages
		const auto end {std::max(mem::LOW_MEMORY_LIMIT, std::min(klib::kAlign::up<LARGE_PAGE_SIZE>(mem::phys::top()), DIRECT_MAP_LIMIT))};
		for (auto phys {mem::LOW_MEMORY_LIMIT}; phys < end; phys += LARGE_PAGE_SIZE) {
			mapped = mapped && paging::mapLarge(top, phys, static_cast<mem::virt_t>(static_cast<igros_usize_t>(phys) + offset), data);
		}
		if (!mapped) [[unlikely]] {
			klib::kprintf("Paging:\t\tFAILED (can't build kernel mapping)");
			return;
		}

		// Switch to new tables: whole direct map is reachable from now on
		::writeCR3(static_cast<igros_usize_t>(top));
		mem::direct::setEnd(end);
		// Kernel can't write to read-only pages (code and read-only data)
		::writeCR0(::readCR0() | CR0_WP);

		// Check mapping
		klib::kprintf(
			"Paging:\t\t%s (%zu levels, root at 0x%08llx, direct map %llu MB%s)",
			paging::selfTest() ? "OK" : "SELF-TEST FAILED",
			LEVELS,
			static_cast<unsigned long long>(top),
			static_cast<unsigned long long>(end >> 20),
			(0_usize != noExecute) ? ", NX" : ""
		);

	}


	// Check mapping on a scratch page
	[[nodiscard]]
	auto paging::selfTest() noexcept -> bool {
		// Scratch page in dynamic mappings area (right above direct map)
		const auto page	{paging::dynamicStart()};
		const auto virt	{mem::virtOf(page)};
		const auto phys	{mem::phys::alloc(mem::direct::end())};
		if (0_u64 == phys) [[unlikely]] {
			return false;
		}
		// Direct map alias of scratch page
		const auto alias {std::bit_cast<volatile igros_byte_t*>(paging::table(phys))};
		if (nullptr == alias) [[unlikely]] {
			mem::phys::free(phys);
			return false;
		}
		auto passed {
			(INVALID == paging::translate(virt))						&&
			(INVALID == paging::translate(mem::virt_t {0}))					&&
			paging::map(phys, virt, klib::make_kflags<FLAGS>(FLAGS::PRESENT, FLAGS::WRITABLE, FLAGS::NO_EXECUTE))	&&
			(phys + 0x123_u64 == paging::translate(virt + 0x123))
		};
		if (passed) {
			// Write through new mapping, read through direct map alias
			static_cast<volatile igros_byte_t*>(page)[0x123] = 0x5A_u8;
			passed = (0x5A_u8 == alias[0x123]);
			alias[0x456] = 0xA5_u8;
			passed = passed && (0xA5_u8 == static_cast<volatile igros_byte_t*>(page)[0x456]);
		}
		paging::unmap(virt);
		passed = passed && (INVALID == paging::translate(virt));
		mem::phys::free(phys);
		return passed;
	}


	// Page fault handler
	[[noreturn]]
	void paging::exHandler(const register_t* regs) noexcept {
		// Disable interrupts
		irq::disable();
		// Error code bits
		const auto error {static_cast<igros_usize_t>(regs->param)};
		klib::kprintf(
R"exception(
EXCEPTION [#%u]
Name:		%s
Address:	0x%016zx
Reason:		%s page, %s access, %s mode%s%s
)exception",
			static_cast<igros_dword_t>(regs->number),
			except::NAME[regs->number],
			::readCR2(),
			(0_usize != (error & 0x01_usize)) ? "protected"	: "missing",
			(0_usize != (error & 0x02_usize)) ? "write"	: "read",
			(0_usize != (error & 0x04_usize)) ? "user"	: "kernel",
			(0_usize != (error & 0x08_usize)) ? ", reserved bit set"	: "",
			(0_usize != (error & 0x10_usize)) ? ", instruction fetch"	: ""
		);
		// Dump registers and hang CPU
		cpu::dumpRegisters(regs);
		cpu::halt();
	}


}	// namespace igros::x86

