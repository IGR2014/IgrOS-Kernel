////////////////////////////////////////////////////////////////
//
//	Memory paging for x86
//
//	File:	paging.cpp
//	Date:	07 Feb 2021
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#include <array>

#include <platform.hpp>

#include <arch/i386/cr.hpp>
#include <arch/i386/irq.hpp>
#include <arch/i386/exceptions.hpp>
#include <arch/i386/paging.hpp>
#include <arch/i386/register.hpp>
#include <arch/i386/cpu.hpp>

#include <klib/kalign.hpp>
#include <klib/kmemory.hpp>
#include <klib/kprint.hpp>


// Arch-dependent code zone
namespace igros::i386 {


	// Free pages list
	page_t* paging::mFreePages = reinterpret_cast<page_t*>(&paging::mFreePages);


	// Kernel memory map structure
	const struct PAGE_MAP_t {
		const page_t*	phys;
		const pointer_t	virt;
	};

	// Kernel memory map
	inline static const std::array<PAGE_MAP_t, 2ULL> PAGE_MAP {{
		// Identity map first 4MB of physical memory to first 4Mb in virtual memory
		// 0Mb		->	0Mb
		{nullptr,	nullptr},
		// Also map first 4MB of physical memory to 3Gb offset in virtual memory
		// 0Mb		->	3Gb + 0Mb
		{nullptr,	std::add_pointer_t<void>(0xC0000000)}
	}};


	// Identity map kernel + map higher-half + self-map page directory
	void paging::init() noexcept {

		// Install exception handler for page fault
		except::install(except::NUMBER::PAGE_FAULT, paging::exHandler);

		// Get kernel end address
		auto kernelEnd = const_cast<byte_t*>(platform::KERNEL_END());
		// Initialize pages for page tables
		paging::heap(kernelEnd, PAGE_SIZE << 6);

		// Create flags
		const auto flags = kflags<flags_t> {
			flags_t::WRITABLE,
			flags_t::PRESENT
		};
		// Create page directory
		const auto dir = paging::makeDirectory();
		// Map memory
		for (const auto &m : PAGE_MAP) {
			// Map page tables
			paging::mapTable(dir, m.phys, m.virt, flags);
		}
		// Map page directory to itself
		paging::mapPage(dir, reinterpret_cast<page_t*>(dir), reinterpret_cast<pointer_t>(0xFFFFF000), flags);

/*
		// Debug
		klib::kprintf(
			u8"0x%p\r\n",
			*dir
		);
*/

		// Setup page directory
		// PD address bits ([0 .. 31] in cr3)
		paging::flush(dir);
		// Disable PSE
		paging::disablePSE();
		// Enable paging
		paging::enable();

	}


	// Enable paging
	void paging::enable() noexcept {
		// Set paging bit on in CR0
		inCR0(outCR0() | 0x80000000);
	}

	// Disable paging
	void paging::disable() noexcept {
		// Set paging bit off in CR0
		inCR0(outCR0() & 0x7FFFFFFF);
	}


	// Enable Physical Address Extension
	void paging::enablePAE() noexcept {
		// Set PAE bit on in CR4
		inCR4(outCR4() | 0x00000020);
	}

	// Disable Physical Address Extension
	void paging::disablePAE() noexcept {
		// Set PAE bit off in CR4
		inCR4(outCR4() & 0xFFFFFFDF);
	}


	// Enable Page Size Extension
	void paging::enablePSE() noexcept {
		// Set PSE bit on in CR4
		inCR4(outCR4() | 0x00000010);
	}

	// Disable Page Size Extension
	void paging::disablePSE() noexcept {
		// Set PSE bit off in CR4
		inCR4(outCR4() & 0xFFFFFFEF);
	}


	// Initialize paging heap
	void paging::heap(const pointer_t phys, const std::size_t size) noexcept {

		// Temporary data
		const auto tempPhys = klib::kalignUp(phys, PAGE_SHIFT);
		const auto tempSize = size - (reinterpret_cast<std::size_t>(tempPhys) - reinterpret_cast<std::size_t>(phys));

		// Get number of pages
		const auto numOfPages = (tempSize >> PAGE_SHIFT);
		// Check input
		if (0ULL == numOfPages) {
			return;
		}

		// Convert to page pointer
		const auto page = static_cast<page_t*>(tempPhys);
		// Link first page to free pages list
		page[0ULL].next = paging::mFreePages;
		// Create linked list of free pages
		for (auto i = 1ULL; i < numOfPages; i++) {
			// Link each next page to previous
			page[i].next = &page[i - 1ULL];
		}
		// Make last page new list head
		paging::mFreePages = &page[numOfPages - 2ULL];

	}


	// Allocate page
	[[nodiscard]]
	pointer_t paging::allocate() noexcept {
		// Check if pages exist
		if (paging::mFreePages->next != paging::mFreePages) {
			// Get free page
			const auto addr		= paging::mFreePages;
			// Update free pages list
			paging::mFreePages	= static_cast<page_t*>(addr->next);
			// Return pointer to free page
			return addr;
		}
		// Nothing to return
		return nullptr;
	}

	// Deallocate page
	void paging::deallocate(const pointer_t page) noexcept {
		// Check alignment
		if (!klib::kalignCheck(page, PAGE_SHIFT)) {
			return;
		}
		// Deallocate page back to heap free list
		static_cast<page_t*>(page)->next = paging::mFreePages;
		paging::mFreePages = static_cast<page_t*>(page);
	}


	// Make page directory
	[[nodiscard]]
	directory_t* paging::makeDirectory() noexcept {
		// Allocate page directory
		const auto dir = static_cast<directory_t*>(paging::allocate());
		// Zero enties of page directory
		klib::kmemset(dir, (sizeof(directory_t) >> 2), kflags(flags_t::CLEAR).value());
		// Return page directory
		return dir;
	}

	// Make page table
	[[nodiscard]]
	table_t* paging::makeTable() noexcept {
		// Allocate page table
		const auto table = static_cast<table_t*>(paging::allocate());
		// Zero enties of page table
		klib::kmemset(table, (sizeof(table_t) >> 2), kflags(flags_t::CLEAR).value());
		// Return page table
		return table;
	}


	// Check table flags
	[[nodiscard]]
	bool paging::checkFlags(const table_t* table, const kflags<flags_t> flags) noexcept {
		// Mask flags
		const auto maskedFlags	= flags & flags_t::FLAGS_MASK;
		// Get table flags
		const auto tableFlags	= kflags<flags_t> {
			reinterpret_cast<std::size_t>(table)
		};
		// Check flags
		return maskedFlags != (tableFlags & maskedFlags);
	}

	// Check page flags
	[[nodiscard]]
	bool paging::checkFlags(const page_t* page, const kflags<flags_t> flags) noexcept {
		// Mask flags
		const auto maskedFlags	= flags & flags_t::FLAGS_MASK;
		// Get page flags
		const auto pageFlags	= kflags<flags_t> {
			reinterpret_cast<std::size_t>(page)
		};
		// Check flags
		return maskedFlags != (pageFlags & maskedFlags);
	}


	// Map virtual page to physical page (whole page, explicit page directory)
	void paging::mapTable(directory_t* const dir, const page_t* phys, const pointer_t virt, const kflags<flags_t> flags) noexcept {

		// Check alignment
		if (	!klib::kalignCheck(phys, PAGE_SHIFT)	||
			!klib::kalignCheck(virt, PAGE_SHIFT)) {
			// Bad align detected
			return;
		}

		// Page directory entry index from virtual address
		const auto dirID = (reinterpret_cast<std::size_t>(virt) >> 22) & 0x3FF;

		// Get page table pointer
		auto &table = dir->tables[dirID];
		// Check if page table is present or not
		if (!paging::checkFlags(table, flags_t::PRESENT)) {
			// Create page table
			table = paging::makeTable();
		}

		// Get page pointer and map physical page
		for (auto i = 0u; i < (sizeof(page_t) >> 2); i++) {
			// Get phys page ID
			const auto page	= kflags<flags_t> {
				reinterpret_cast<std::size_t>(phys) + (i << PAGE_SHIFT),
				flags & flags_t::FLAGS_MASK
			};
			// Map page
			table->pages[i]	= reinterpret_cast<page_t*>(page.value());
		}

		// Get table flags
		const auto tableFlags = kflags<flags_t> {
			reinterpret_cast<std::size_t>(table) & 0x3FFFFFFF,
			flags & flags_t::FLAGS_MASK
		};
		// Set new page table flags
		table = reinterpret_cast<table_t*>(tableFlags.value());

	}

	// Map virtual page to physical page (whole page)
	void paging::mapTable(const page_t* phys, const pointer_t virt, const kflags<flags_t> flags) noexcept {
		// Get pointer to page directory
		const auto dir = reinterpret_cast<directory_t*>(outCR3());
		// Map page to curent page directory
		paging::mapTable(dir, phys, virt, flags);
		// Setup page directory
		// PD address bits ([0 .. 31] in cr3)
		paging::flush(dir);
		// Enable paging
		paging::enable();
	}


	// Map virtual page to physical page (explicit page directory)
	void paging::mapPage(directory_t* const dir, const page_t* phys, const pointer_t virt, const kflags<flags_t> flags) noexcept {

		// Check alignment
		if (	!klib::kalignCheck(phys, PAGE_SHIFT)	||
			!klib::kalignCheck(virt, PAGE_SHIFT)) {
			// Bad align detected
			return;
		}

		// Page directory entry index from virtual address
		const auto dirID = (reinterpret_cast<std::size_t>(virt) >> PAGE_DIRECTORY_SHIFT) & PAGE_ENTRY_MASK;
		// Page table entry index from virtual address
		const auto tabID = (reinterpret_cast<std::size_t>(virt) >> PAGE_TABLE_SHIFT) & PAGE_ENTRY_MASK;

		// Get page table pointer
		auto &table = dir->tables[dirID];
		// Check if page table is present or not
		if (!paging::checkFlags(table, flags_t::PRESENT)) {
			// Create page table
			table = paging::makeTable();
		}

		// Get page
		auto &page = table->pages[tabID];

		// Get table flags
		const auto tableFlags = kflags<flags_t> {
			reinterpret_cast<std::size_t>(table) & 0x3FFFFFFF,
			flags & flags_t::FLAGS_MASK
		};
		// Get page flags
		const auto pageFlags = kflags<flags_t> {
			reinterpret_cast<std::size_t>(page) & 0x3FFFFFFF,
			flags & flags_t::FLAGS_MASK
		};

		// Insert page table
		table	= reinterpret_cast<table_t*>(tableFlags.value());
		// Insert page pointer
		page	= reinterpret_cast<page_t*>(pageFlags.value());

	}

	// Map virtual page to physical page (single page)
	void paging::mapPage(const page_t* phys, const pointer_t virt, const kflags<flags_t> flags) noexcept {
		// Get pointer to page directory
		const auto dir = reinterpret_cast<directory_t*>(outCR3());
		// Map page to curent page directory
		paging::mapPage(dir, phys, virt, flags);
		// Setup page directory
		// PD address bits ([0 .. 31] in cr3)
		paging::flush(dir);
		// Enable paging
		paging::enable();

	}


	// Convert virtual address to physical address
	[[nodiscard]]
	pointer_t paging::translate(const pointer_t virt) noexcept {

		// Page directory entry index from virtual address
		const auto dirID = (reinterpret_cast<std::size_t>(virt) >> PAGE_DIRECTORY_SHIFT) & PAGE_ENTRY_MASK;
		// Page table entry index from virtual address
		const auto tabID = (reinterpret_cast<std::size_t>(virt) >> PAGE_TABLE_SHIFT) & PAGE_ENTRY_MASK;

		// Get pointer to page directory
		const auto dir = reinterpret_cast<const directory_t*>(outCR3());

		// Get page table pointer
		const auto table = dir->tables[dirID];
		// Check if page table is present or not
		if (!paging::checkFlags(table, flags_t::PRESENT)) {
			// Page or table is not present
			return nullptr;
		}

		// Get page pointer
		const auto page = table->pages[tabID];
		// Check if page table is present or not
		if (!paging::checkFlags(page, flags_t::PRESENT)) {
			// Page or table is not present
			return nullptr;
		}

		// Get physical address of page from page table (20 MSB)
		const auto address	= kflags<flags_t> {
			reinterpret_cast<std::size_t>(page)
		};
		// Get physical offset in psge from virtual address`s (12 LSB)
		const auto offset	= kflags<flags_t> {
			reinterpret_cast<std::size_t>(virt)
		};
		// Return physical address
		return reinterpret_cast<pointer_t>(((address & flags_t::PHYS_ADDR_MASK) | (offset & flags_t::FLAGS_MASK)).value());

	}


	// Page Fault Exception handler
	void paging::exHandler(const register_t* regs) noexcept {

		// Disable IRQ
		irqi386::disable();

		// Write Multiboot magic error message message
		klib::kprintf(
			u8"EXCEPTION [#%d]\t-> (%s)\r\n"
			u8"Caused by:\t%s%s%s\r\n"
			u8"From:\t\t%s space\r\n"
			u8"When:\t\tattempting to %s\r\n"
			u8"Address:\t0x%p\r\n"
			u8"Which is:\tnot %s\r\n",
			static_cast<dword_t>(except::NUMBER::PAGE_FAULT),
			except::NAME[static_cast<dword_t>(except::NUMBER::PAGE_FAULT)],
			((regs->param & 0x18) == 0u) ? u8"ACCESS VIOLATION"	: u8"",
			((regs->param & 0x10) == 0u) ? u8""			: u8"INSTRUCTION FETCH",
			((regs->param & 0x08) == 0u) ? u8""			: u8"RESERVED BIT SET",
			((regs->param & 0x04) == 0u) ? u8"KERNEL"		: u8"USER",
			((regs->param & 0x02) == 0u) ? u8"READ"			: u8"WRITE",
			reinterpret_cast<const pointer_t>(outCR2()),
			((regs->param & 0x01) == 0u) ? u8"PRESENT"		: u8"PRIVILEGED"
		);

		// Hang here
		cpuHalt();

	}


	// Set page directory
	inline void paging::flush(const directory_t* const dir) noexcept {
		// Set page directory address to CR3
		inCR3(reinterpret_cast<std::uintptr_t>(dir) & 0x3FFFFFFF);
	}


}	// namespace igros::arch

