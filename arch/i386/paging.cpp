////////////////////////////////////////////////////////////////
//
//	Memory paging for x86
//
//	File:	paging.cpp
//	Date:	16 Mar 2023
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


// C++
#include <array>
#include <bit>
// IgrOS-Kernel arch platform
#include <arch/platform/platform.hpp>
// IgrOS-Kernel arch i386
#include <arch/i386/cpu.hpp>
#include <arch/i386/cr.hpp>
#include <arch/i386/exceptions.hpp>
#include <arch/i386/irq.hpp>
#include <arch/i386/paging.hpp>
#include <arch/i386/register.hpp>
// IgrOS-Kernel library
#include <klib/kalign.hpp>
#include <klib/kmemory.hpp>
#include <klib/kprint.hpp>


// Arch-dependent code zone
namespace igros::i386 {


	// Free pages list
	page_t* paging::mFreePages	{std::bit_cast<page_t*>(&paging::mFreePages)};


	// Kernel memory map structure
	struct PAGE_MAP_t {
		const page_t*		phys;
		const igros_pointer_t	virt;
	};

	// Kernel memory map
	static const auto PAGE_MAP {std::array<PAGE_MAP_t, 2_usize> {{
		// Identity map first 4MB of physical memory to first 4Mb in virtual memory
		// 0Mb		->	0Mb
		{nullptr,	nullptr},
		// Also map first 4MB of physical memory to 3Gb offset in virtual memory
		// 0Mb		->	3Gb + 0Mb
		{nullptr,	std::bit_cast<const igros_pointer_t>(0xC0000000_usize)}
	}}};


	// Identity map kernel + map higher-half + self-map page directory
	void paging::init() noexcept {

		// Install exception handler for page fault
		except::install<except::NUMBER::PAGE_FAULT, paging::exHandler>();

		// Get kernel end address
		constexpr auto kernelEnd {const_cast<igros_byte_t*>(platform::KERNEL_END())};
		// Initialize pages for page tables
		paging::heap(kernelEnd, PAGE_SIZE << 6);

		// Create flags
		const auto flags	{klib::make_kflags<FLAGS>(FLAGS::WRITABLE, FLAGS::PRESENT)};
		// Create page directory
		const auto dir		{paging::makeDirectory()};
		// Map memory
		for (const auto &m : PAGE_MAP) {
			// Map page tables
			paging::mapTable(dir, m.phys, m.virt, flags);
		}
		// Map page directory to itself
		paging::mapPage(dir, std::bit_cast<page_t*>(dir), std::bit_cast<igros_pointer_t>(0xFFFFF000_usize), flags);

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
		::inCR0(::outCR0() | 0x80000000_u32);
	}

	// Disable paging
	void paging::disable() noexcept {
		// Set paging bit off in CR0
		::inCR0(::outCR0() & 0x7FFFFFFF_u32);
	}


	// Enable Physical Address Extension
	void paging::enablePAE() noexcept {
		// Set PAE bit on in CR4
		::inCR4(::outCR4() | 0x00000020_u32);
	}

	// Disable Physical Address Extension
	void paging::disablePAE() noexcept {
		// Set PAE bit off in CR4
		::inCR4(::outCR4() & 0xFFFFFFDF_u32);
	}


	// Enable Page Size Extension
	void paging::enablePSE() noexcept {
		// Set PSE bit on in CR4
		::inCR4(::outCR4() | 0x00000010_u32);
	}

	// Disable Page Size Extension
	void paging::disablePSE() noexcept {
		// Set PSE bit off in CR4
		::inCR4(::outCR4() & 0xFFFFFFEF_u32);
	}


	// Initialize paging heap
	void paging::heap(const igros_pointer_t phys, const igros_usize_t size) noexcept {

		// Temporary data
		const auto tempPhys	{klib::kalignUp(phys, PAGE_SHIFT)};
		const auto tempSize	{size - (std::bit_cast<igros_usize_t>(tempPhys) - std::bit_cast<igros_usize_t>(phys))};

		// Get number of pages
		const auto numOfPages	{tempSize >> PAGE_SHIFT};
		// Check input
		if (0_usize == numOfPages) {
			return;
		}

		// Convert to page pointer
		const auto page		{static_cast<page_t*>(tempPhys)};
		// Link first page to free pages list
		page[0_usize].next	= paging::mFreePages;
		// Create linked list of free pages
		for (auto i {1_usize}; i < numOfPages; i++) {
			// Link each next page to previous
			page[i].next = &page[i - 1_usize];
		}
		// Make last page new list head
		paging::mFreePages = &page[numOfPages - 2_usize];

	}


	// Allocate page
	[[nodiscard]]
	igros_pointer_t paging::allocate() noexcept {
		// Check if pages exist
		if (paging::mFreePages->next != paging::mFreePages) {
			// Get free page
			const auto addr		{paging::mFreePages};
			// Update free pages list
			paging::mFreePages	= static_cast<page_t*>(addr->next);
			// Return pointer to free page
			return addr;
		}
		// Nothing to return
		return nullptr;
	}

	// Deallocate page
	void paging::deallocate(const igros_pointer_t page) noexcept {
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
		const auto dir {static_cast<directory_t*>(paging::allocate())};
		// Zero enties of page directory
		klib::kmemset(dir, (sizeof(directory_t) >> 2), klib::kFlags(FLAGS::CLEAR).value());
		// Return page directory
		return dir;
	}

	// Make page table
	[[nodiscard]]
	table_t* paging::makeTable() noexcept {
		// Allocate page table
		const auto table {static_cast<table_t*>(paging::allocate())};
		// Zero enties of page table
		klib::kmemset(table, (sizeof(table_t) >> 2), klib::kFlags(FLAGS::CLEAR).value());
		// Return page table
		return table;
	}


	// Check table flags
	[[nodiscard]]
	bool paging::checkFlags(const table_t* table, const klib::kFlags<FLAGS> flags) noexcept {
		// Mask flags
		const auto maskedFlags	{flags & FLAGS::FLAGS_MASK};
		// Get table flags
		const auto tableFlags	{klib::make_kflags<FLAGS>(std::bit_cast<igros_usize_t>(table))};
		// Check flags
		return maskedFlags != (tableFlags & maskedFlags);
	}

	// Check page flags
	[[nodiscard]]
	bool paging::checkFlags(const page_t* page, const klib::kFlags<FLAGS> flags) noexcept {
		// Mask flags
		const auto maskedFlags	{flags & FLAGS::FLAGS_MASK};
		// Get page flags
		const auto pageFlags	{klib::make_kflags<FLAGS>(std::bit_cast<igros_usize_t>(page))};
		// Check flags
		return maskedFlags != (pageFlags & maskedFlags);
	}


	// Map virtual page to physical page (whole page, explicit page directory)
	void paging::mapTable(directory_t* const dir, const page_t* phys, const igros_pointer_t virt, const klib::kFlags<FLAGS> flags) noexcept {

		// Check alignment
		if (
			!klib::kalignCheck(phys, PAGE_SHIFT)	||
			!klib::kalignCheck(virt, PAGE_SHIFT)
		) {
			// Bad align detected
			return;
		}

		// Page directory entry index from virtual address
		const auto dirID	{(std::bit_cast<igros_usize_t>(virt) >> 22) & 0x3FF_usize};

		// Get page table pointer
		auto &table		{dir->tables[dirID]};
		// Check if page table is present or not
		if (!paging::checkFlags(table, klib::make_kflags<FLAGS>(FLAGS::PRESENT))) {
			// Create page table
			table = paging::makeTable();
		}

		// Get page pointer and map physical page
		for (auto i {0_usize}; i < (sizeof(page_t) >> 2); i++) {
			// Get phys page ID
			const auto page	{klib::make_kflags<FLAGS>(std::bit_cast<igros_usize_t>(phys) + (i << PAGE_SHIFT),	flags & FLAGS::FLAGS_MASK)};
			// Map page
			table->pages[i]	= std::bit_cast<page_t*>(page.value());
		}

		// Get table flags
		const auto tableFlags	{klib::make_kflags<FLAGS>(std::bit_cast<igros_usize_t>(table) & 0x3FFFFFFF_usize,	flags & FLAGS::FLAGS_MASK)};
		// Set new page table flags
		table = std::bit_cast<table_t*>(tableFlags.value());

	}

	// Map virtual page to physical page (whole page)
	void paging::mapTable(const page_t* phys, const igros_pointer_t virt, const klib::kFlags<FLAGS> flags) noexcept {
		// Get pointer to page directory
		const auto dir {std::bit_cast<directory_t*>(::outCR3())};
		// Map page to curent page directory
		paging::mapTable(dir, phys, virt, flags);
	}


	// Map virtual page to physical page (explicit page directory)
	void paging::mapPage(directory_t* const dir, const page_t* phys, const igros_pointer_t virt, const klib::kFlags<FLAGS> flags) noexcept {

		// Check alignment
		if (
			!klib::kalignCheck(phys, PAGE_SHIFT)	||
			!klib::kalignCheck(virt, PAGE_SHIFT)
		) {
			// Bad align detected
			return;
		}

		// Page directory entry index from virtual address
		const auto dirID	{(std::bit_cast<igros_usize_t>(virt) >> PAGE_DIRECTORY_SHIFT)	& PAGE_ENTRY_MASK};
		// Page table entry index from virtual address
		const auto tabID	{(std::bit_cast<igros_usize_t>(virt) >> PAGE_TABLE_SHIFT)	& PAGE_ENTRY_MASK};

		// Get page table pointer
		auto &table		{dir->tables[dirID]};
		// Check if page table is present or not
		if (!paging::checkFlags(table, klib::make_kflags<FLAGS>(FLAGS::PRESENT))) {
			// Create page table
			table = paging::makeTable();
		}

		// Get page
		auto &page		{table->pages[tabID]};

		// Get table flags
		const auto tableFlags	{klib::make_kflags<FLAGS>(std::bit_cast<igros_usize_t>(table) & 0x3FFFFFFF_usize,	flags & FLAGS::FLAGS_MASK)};
		// Get page flags
		const auto pageFlags	{klib::make_kflags<FLAGS>(std::bit_cast<igros_usize_t>(page) & 0x3FFFFFFF_usize,	flags & FLAGS::FLAGS_MASK)};

		// Insert page table
		table	= std::bit_cast<table_t*>(tableFlags.value());
		// Insert page pointer
		page	= std::bit_cast<page_t*>(pageFlags.value());

	}

	// Map virtual page to physical page (single page)
	void paging::mapPage(const page_t* phys, const igros_pointer_t virt, const klib::kFlags<FLAGS> flags) noexcept {
		// Get pointer to page directory
		const auto dir {std::bit_cast<directory_t*>(::outCR3())};
		// Map page to curent page directory
		paging::mapPage(dir, phys, virt, flags);
	}


	// Convert virtual address to physical address
	[[nodiscard]]
	igros_pointer_t paging::translate(const igros_pointer_t virt) noexcept {

		// Page directory entry index from virtual address
		const auto dirID	{(std::bit_cast<igros_usize_t>(virt) >> PAGE_DIRECTORY_SHIFT)	& PAGE_ENTRY_MASK};
		// Page table entry index from virtual address
		const auto tabID	{(std::bit_cast<igros_usize_t>(virt) >> PAGE_TABLE_SHIFT)	& PAGE_ENTRY_MASK};

		// Get pointer to page directory
		const auto dir		{std::bit_cast<const directory_t*>(::outCR3())};

		// Get page table pointer
		const auto table	{dir->tables[dirID]};
		// Check if page table is present or not
		if (!paging::checkFlags(table, klib::make_kflags<FLAGS>(FLAGS::PRESENT))) {
			// Page or table is not present
			return nullptr;
		}

		// Get page pointer
		const auto page		{table->pages[tabID]};
		// Check if page table is present or not
		if (!paging::checkFlags(page, klib::make_kflags<FLAGS>(FLAGS::PRESENT))) {
			// Page or table is not present
			return nullptr;
		}

		// Get physical address of page from page table (20 MSB)
		const auto address	{klib::make_kflags<FLAGS>(std::bit_cast<igros_usize_t>(page))};
		// Get physical offset in psge from virtual address`s (12 LSB)
		const auto offset	{klib::make_kflags<FLAGS>(std::bit_cast<igros_usize_t>(virt))};
		// Return physical address
		return std::bit_cast<igros_pointer_t>(((address & FLAGS::PHYS_ADDR_MASK) | (offset & FLAGS::FLAGS_MASK)).value());

	}


	// Page Fault Exception handler
	[[noreturn]]
	void paging::exHandler(const register_t* regs) noexcept {
		// Disable IRQ
		irq::disable();
		// Write Multiboot magic error message message
		klib::kprintf(
R"exception(
EXCEPTION [#%d]
Name:		%s
Caused by:	%s%s%s
From:		%s space
When:		attempting to %s
Address:	0x%p
Which is:	not %s
)exception",
			regs->number,
			except::NAME[regs->number],
			((regs->param & 0x18_u32) == 0_u32) ? "ACCESS VIOLATION"	: "",
			((regs->param & 0x10_u32) == 0_u32) ? ""			: "INSTRUCTION FETCH",
			((regs->param & 0x08_u32) == 0_u32) ? ""			: "RESERVED BIT SET",
			((regs->param & 0x04_u32) == 0_u32) ? "KERNEL"			: "USER",
			((regs->param & 0x02_u32) == 0_u32) ? "READ"			: "WRITE",
			std::bit_cast<const igros_pointer_t>(::outCR2()),
			((regs->param & 0x01_u32) == 0_u32) ? "PRESENT"			: "PRIVILEGED"
		);
		// Dump registres
		cpu::dumpRegisters(regs);
		// Hang here
		cpu::halt();
	}


	// Set page directory
	inline void paging::flush(const directory_t* const dir) noexcept {
		// Set page directory address to CR3
		::inCR3(std::bit_cast<igros_usize_t>(dir) & 0x3FFFFFFF_usize);
	}


}	// namespace igros::arch

