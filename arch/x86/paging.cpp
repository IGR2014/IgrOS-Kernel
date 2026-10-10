////////////////////////////////////////////////////////////////
//
//	Paging (i386 2-level and x86_64 4-level page tables)
//
//	File:	paging.cpp
//	Date:	11 Oct 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


// C++
#include <bit>
// IgrOS-Kernel arch x86
#include <arch/x86/cr.hpp>
#include <arch/x86/exceptions.hpp>
#include <arch/x86/irq.hpp>
#include <arch/x86/native.hpp>
#include <arch/x86/paging.hpp>
// IgrOS-Kernel library
#include <klib/kmemory.hpp>
#include <klib/kprint.hpp>
// IgrOS-Kernel platform
#include <platform/platform.hpp>


// x86 namespace
namespace igros::x86 {


	// Entry flag value
	[[nodiscard]]
	constexpr static auto flag(const paging::FLAGS value) noexcept -> paging::entry_t {
		return static_cast<paging::entry_t>(value);
	}


	// Table at physical address (in low memory)
	[[nodiscard]]
	auto paging::table(const mem::phys_t phys) noexcept -> entry_t* {
		// Only low memory is reachable through kernel offset
		if (phys >= mem::LOW_MEMORY_LIMIT) [[unlikely]] {
			return nullptr;
		}
		return std::bit_cast<entry_t*>(static_cast<igros_usize_t>(phys) + platform::Platform::kernelOffset());
	}

	// Index of virtual address in table of given level (0 - page table)
	[[nodiscard]]
	auto paging::index(const void* const virt, const igros_usize_t level) noexcept -> igros_usize_t {
		return (std::bit_cast<igros_usize_t>(virt) >> (PAGE_SHIFT + INDEX_BITS * level)) & (ENTRIES - 1_usize);
	}


	// Page table entry of virtual address in given hierarchy (create - allocate missing tables)
	[[nodiscard]]
	auto paging::entry(const mem::phys_t top, const void* const virt, const bool create, const bool user) noexcept -> entry_t* {
		auto entries {paging::table(top)};
		// Walk down to page table
		for (auto level {LEVELS - 1_usize}; (nullptr != entries) && (level > 0_usize); --level) {
			auto &next {entries[paging::index(virt, level)]};
			if (0_usize == (next & flag(FLAGS::PRESENT))) {
				// Missing table
				if (!create) {
					return nullptr;
				}
				const auto page {mem::phys::alloc(mem::LOW_MEMORY_LIMIT)};
				if (0_u64 == page) [[unlikely]] {
					return nullptr;
				}
				klib::kmemset(paging::table(page), ENTRIES, entry_t {0});
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
		// Page table entry
		return (nullptr != entries) ? &entries[paging::index(virt, 0_usize)] : nullptr;
	}

	// Map 4 KB page in given hierarchy
	[[nodiscard]]
	auto paging::map(const mem::phys_t top, const mem::phys_t phys, const void* const virt, const klib::kFlags<FLAGS> flags) noexcept -> bool {
		// Page aligned addresses only
		if ((0_u64 != (phys & (PAGE_SIZE - 1_usize))) || (0_usize != (std::bit_cast<igros_usize_t>(virt) & (PAGE_SIZE - 1_usize)))) [[unlikely]] {
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
		*page = static_cast<entry_t>(phys) | (flags.value() & ~ADDRESS_MASK & ~flag(FLAGS::HUGE)) | flag(FLAGS::PRESENT);
		::pageInvalidate(virt);
		return true;
	}


	// Map 4 KB page
	[[nodiscard]]
	auto paging::map(const mem::phys_t phys, const void* const virt, const klib::kFlags<FLAGS> flags) noexcept -> bool {
		return paging::map(paging::root(), phys, virt, flags);
	}

	// Unmap 4 KB page
	void paging::unmap(const void* const virt) noexcept {
		const auto page {paging::entry(paging::root(), virt, false, false)};
		if (nullptr != page) {
			*page = 0_usize;
			::pageInvalidate(virt);
		}
	}


	// Physical address of virtual address (INVALID if not mapped)
	[[nodiscard]]
	auto paging::translate(const void* const virt) noexcept -> mem::phys_t {
		const auto address	{static_cast<mem::phys_t>(std::bit_cast<igros_usize_t>(virt))};
		auto entries		{paging::table(paging::root())};
		for (auto level {LEVELS - 1_usize}; nullptr != entries; --level) {
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
		return static_cast<mem::phys_t>(::outCR3() & ADDRESS_MASK);
	}


	// Build kernel page tables and switch to them
	void paging::init() noexcept {

		// Page fault handler
		except::install<except::NUMBER::PAGE_FAULT, paging::exHandler>();

		// Top level table
		const auto top {mem::phys::alloc(mem::LOW_MEMORY_LIMIT)};
		if (0_u64 == top) [[unlikely]] {
			klib::kprintf("Paging:\t\tFAILED (no memory for page tables)");
			return;
		}
		klib::kmemset(paging::table(top), ENTRIES, entry_t {0});

		// Low memory: at kernel offset (kernel, page tables, VGA) and identity (boot data);
		// identity null page stays unmapped to catch null pointer dereference
		const auto flags	{klib::make_kflags<FLAGS>(FLAGS::PRESENT, FLAGS::WRITABLE)};
		const auto offset	{platform::Platform::kernelOffset()};
		auto mapped		{true};
		for (auto phys {0_u64}; phys < mem::LOW_MEMORY_LIMIT; phys += PAGE_SIZE) {
			const auto page {static_cast<igros_usize_t>(phys)};
			mapped = mapped && paging::map(top, phys, std::bit_cast<const void*>(page + offset), flags);
			if (0_u64 != phys) {
				mapped = mapped && paging::map(top, phys, std::bit_cast<const void*>(page), flags);
			}
		}
		if (!mapped) [[unlikely]] {
			klib::kprintf("Paging:\t\tFAILED (can't build kernel mapping)");
			return;
		}

		// Switch to new tables
		::inCR3(static_cast<igros_usize_t>(top));

		// Check mapping
		klib::kprintf(
			"Paging:\t\t%s (%zu levels, root at 0x%08llx)",
			paging::selfTest() ? "OK" : "SELF-TEST FAILED",
			LEVELS,
			static_cast<unsigned long long>(top)
		);

	}


	// Check mapping on a scratch page
	[[nodiscard]]
	auto paging::selfTest() noexcept -> bool {
		// Scratch page above kernel low memory mapping
		const auto virt	{std::bit_cast<igros_byte_t*>(platform::Platform::kernelOffset() + 0x10000000_usize)};
		const auto phys	{mem::phys::alloc(mem::LOW_MEMORY_LIMIT)};
		if (0_u64 == phys) [[unlikely]] {
			return false;
		}
		// Low memory alias of scratch page
		const auto alias {std::bit_cast<volatile igros_byte_t*>(paging::table(phys))};
		if (nullptr == alias) [[unlikely]] {
			mem::phys::free(phys);
			return false;
		}
		auto passed {
			(INVALID == paging::translate(virt))						&&
			(INVALID == paging::translate(nullptr))					&&
			paging::map(phys, virt, klib::make_kflags<FLAGS>(FLAGS::PRESENT, FLAGS::WRITABLE))	&&
			(phys + 0x123_u64 == paging::translate(virt + 0x123))
		};
		if (passed) {
			// Write through new mapping, read through low memory alias
			static_cast<volatile igros_byte_t*>(virt)[0x123] = 0x5A_u8;
			passed = (0x5A_u8 == alias[0x123]);
			alias[0x456] = 0xA5_u8;
			passed = passed && (0xA5_u8 == static_cast<volatile igros_byte_t*>(virt)[0x456]);
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
Address:	0x%016llx
Reason:		%s page, %s access, %s mode%s%s
)exception",
			static_cast<igros_dword_t>(regs->number),
			except::NAME[regs->number],
			static_cast<unsigned long long>(::outCR2()),
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

