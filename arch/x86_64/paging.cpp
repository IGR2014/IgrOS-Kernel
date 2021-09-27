////////////////////////////////////////////////////////////////
//
//	Memory paging for x86_64
//
//	File:	paging.cpp
//	Date:	27 Sep 2021
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#include <array>

#include <platform.hpp>
#include <flags.hpp>

#include <arch/x86_64/msr.hpp>
#include <arch/x86_64/cr.hpp>
#include <arch/x86_64/irq.hpp>
#include <arch/x86_64/exceptions.hpp>
#include <arch/x86_64/paging.hpp>
#include <arch/x86_64/register.hpp>
#include <arch/x86_64/cpu.hpp>

#include <klib/kalign.hpp>
#include <klib/kmemory.hpp>
#include <klib/kprint.hpp>


// x86_64 namespace
namespace igros::x86_64 {


	// Free pages list
	table_t* paging::mFreePages = reinterpret_cast<table_t*>(&paging::mFreePages);


	// Kernel memory map structure
	struct PAGE_MAP_t {
		const page_t*	phys;
		const pointer_t	virt;
	};

	// Kernel memory map
	inline static const std::array<PAGE_MAP_t, 4ULL> PAGE_MAP {{
		// Identity map first 4MB of physical memory to first 4MB in virtual memory
		// 0Mb					->	0Mb
		{nullptr,				nullptr},
		// 2Mb					->	2Mb
		{std::add_pointer_t<page_t>(0x2000000),	std::add_pointer_t<void>(0x2000000)},
		// Also map first 4MB of physical memory to 128TB offset in virtual memory
		// 0Mb					->	128Tb + 0Mb
		{nullptr,				std::add_pointer_t<void>(0xFFFFFFFF80000000)},
		// 2Mb					->	128Tb + 2Mb
		{std::add_pointer_t<page_t>(0x2000000),	std::add_pointer_t<void>(0xFFFFFFFF82000000)}
	}};


	// Setup paging
	void paging::init() noexcept {

		// Install exception handler for page fault
		except::install(except::NUMBER::PAGE_FAULT, paging::exHandler);

		// Get kernel end address
		const auto kernelEnd = const_cast<byte_t*>(platform::KERNEL_END());
		// Initialize pages for page tables
		paging::heap(kernelEnd, PAGE_SIZE << 6);

		// Create flags
		constexpr auto flags = kflags<FLAGS> {
			FLAGS::WRITABLE,
			FLAGS::PRESENT
		};
		// Create page map level 4
		const auto pml4 = paging::makePML4();
		// Map memory
		for (const auto &m : PAGE_MAP) {
			// Map page tables
			paging::mapTable(pml4, m.phys, m.virt, flags);
		}
		// Map page directory to itself
		//paging::mapTable(pml4, reinterpret_cast<page_t*>(pml4), reinterpret_cast<pointer_t>(0xFFFFFFFFFFFFF000), flags);

		// Setup page directory
		// PD address bits ([0 .. 63] in cr3)
		paging::flush(pml4);
		// Enable Physical Address Extension
		paging::enablePAE();
		// Enable paging
		paging::enable();

	}


	// Enable paging
	void paging::enable() noexcept {
		// Set paging bit on in CR0
		inCR0(outCR0() | 0x0000000080000000);
	}

	// Disable paging
	void paging::disable() noexcept {
		// Set paging bit off in CR0
		inCR0(outCR0() & 0xFFFFFFFF7FFFFFFF);
	}


	// Enable Physical Address Extension
	void paging::enablePAE() noexcept {
		// Set paging bit on in CR0
		inCR4(outCR4() | 0x0000000000000020);
	}

	// Disable Physical Address Extension
	void paging::disablePAE() noexcept {
		// Set paging bit off in CR0
		inCR4(outCR4() & 0xFFFFFFFFFFFFFFDF);
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
		const auto page = static_cast<table_t*>(tempPhys);
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
			paging::mFreePages	= static_cast<table_t*>(addr->next);
			// Return pointer to free page
			return reinterpret_cast<page_t*>(addr);
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
		static_cast<table_t*>(page)->next = paging::mFreePages;
		paging::mFreePages = static_cast<table_t*>(page);
	}


	// Make PML4
	[[nodiscard]]
	pml4_t* paging::makePML4() noexcept {
		// Allocate page map level 4
		const auto pml4 = static_cast<pml4_t*>(paging::allocate());
		// Zero enties of page map level 4
		klib::kmemset(pml4, (sizeof(pml4_t) >> 3), kflags(FLAGS::CLEAR).value());
		// Return page map level 4
		return pml4;
	}

	// Make page directory pointer
	[[nodiscard]]
	directoryPointer_t* paging::makeDirectoryPointer() noexcept {
		// Allocate page directory pointer
		const auto dirPtr = static_cast<directoryPointer_t*>(paging::allocate());
		// Zero enties of page directory pointer
		klib::kmemset(dirPtr, (sizeof(directoryPointer_t) >> 3), kflags(FLAGS::CLEAR).value());
		// Return page directory pointer
		return dirPtr;
	}

	// Make page directory
	[[nodiscard]]
	directory_t* paging::makeDirectory() noexcept {
		// Allocate page directory
		const auto dir = static_cast<directory_t*>(paging::allocate());
		// Zero enties of page directory
		klib::kmemset(dir, (sizeof(directory_t) >> 3), kflags(FLAGS::CLEAR).value());
		// Return page directory
		return dir;
	}

	// Make page table
	[[nodiscard]]
	table_t* paging::makeTable() noexcept {
		// Allocate page table
		const auto table = static_cast<table_t*>(paging::allocate());
		// Zero enties of page table
		klib::kmemset(table, (sizeof(table_t) >> 3), kflags(FLAGS::CLEAR).value());
		// Return page table
		return table;
	}


	// Check directory pointer flags
	[[nodiscard]]
	bool paging::checkFlags(const directoryPointer_t* dirPtr, const kflags<FLAGS> flags) noexcept {
		// Mask flags
		const auto maskedFlags = flags & FLAGS::FLAGS_MASK;
		// Get directory pointer flags
		const auto dirPtrFlags = kflags<FLAGS> {
			reinterpret_cast<std::size_t>(dirPtr)
		};
		// Check flags
		return maskedFlags != (dirPtrFlags & maskedFlags);
	}

	// Check directory flags
	[[nodiscard]]
	bool paging::checkFlags(const directory_t* dir, const kflags<FLAGS> flags) noexcept {
		// Mask flags
		const auto maskedFlags	= flags & FLAGS::FLAGS_MASK;
		// Get directory flags
		const auto dirFlags	= kflags<FLAGS> {
			reinterpret_cast<std::size_t>(dir)
		};
		// Check flags
		return maskedFlags != (dirFlags & maskedFlags);
	}

	// Check table flags
	[[nodiscard]]
	bool paging::checkFlags(const table_t* table, const kflags<FLAGS> flags) noexcept {
		// Mask flags
		const auto maskedFlags	= flags & FLAGS::FLAGS_MASK;
		// Get table flags
		const auto tableFlags	= kflags<FLAGS> {
			reinterpret_cast<std::size_t>(table)
		};
		// Check flags
		return maskedFlags != (tableFlags & maskedFlags);
	}

	// Check page flags
	[[nodiscard]]
	bool paging::checkFlags(const page_t* page, const kflags<FLAGS> flags) noexcept {
		// Mask flags
		const auto maskedFlags	= flags & FLAGS::FLAGS_MASK;
		// Get page flags
		const auto pageFlags	= kflags<FLAGS> {
			reinterpret_cast<std::size_t>(page)
		};
		// Check flags
		return maskedFlags != (pageFlags & maskedFlags);
	}


	// Map virtual page to physical page (whole pml4, explicit pml4)
	void paging::mapPML4(pml4_t* const pml4, const page_t* phys, const pointer_t virt, const kflags<FLAGS> flags) noexcept {

		// Check alignment
		if (
			!klib::kalignCheck(phys, PAGE_SHIFT)	||
			!klib::kalignCheck(virt, PAGE_SHIFT)
		) {
			// Bad align detected
			return;
		}

		// Get page pointer and map physical page
		for (auto i = 0U; i < (sizeof(pml4_t) >> 3); i++) {
			// Get phys directory ID
			const auto dirPtr = kflags<FLAGS> {
				reinterpret_cast<std::size_t>(phys) + (i << PAGE_SHIFT),
				flags & FLAGS::FLAGS_MASK
			};
			// Map page
			pml4->pointers[i] = reinterpret_cast<directoryPointer_t*>(dirPtr.value());
		}

	}

	// Map virtual page to physical page (whole pml4)
	void paging::mapPML4(const page_t* phys, const pointer_t virt, const kflags<FLAGS> flags) noexcept {
		// Get pointer to page map level 4
		const auto pml4 = reinterpret_cast<pml4_t*>(outCR3());
		// Map page to curent page map level 4
		paging::mapPML4(pml4, phys, virt, flags);
	}


	// Map virtual page to physical page (single directory pointer, explicit pml4)
	void paging::mapDirectoryPointer(pml4_t* const pml4, const page_t* phys, const pointer_t virt, const kflags<FLAGS> flags) noexcept {

		// Check alignment
		if (
			!klib::kalignCheck(phys, PAGE_SHIFT)	||
			!klib::kalignCheck(virt, PAGE_SHIFT)
		) {
			// Bad align detected
			return;
		}

		// Page map level 4 table table index from virtual address
		const auto pml4ID = (reinterpret_cast<std::size_t>(virt) >> 39) & 0x1FF;

		// Get page directory pointer
		auto &dirPtr = pml4->pointers[pml4ID];
		// Check if page directory pointer is present or not
		if (!paging::checkFlags(dirPtr, static_cast<kflags<FLAGS>>(FLAGS::PRESENT))) {
			// Allocate page directory pointer
			dirPtr = paging::makeDirectoryPointer();
		}

		// Get page pointer and map physical page
		for (auto i = 0U; i < (sizeof(directoryPointer_t) >> 3); i++) {
			// Get phys directory ID
			const auto directory = kflags<FLAGS> {
				reinterpret_cast<std::size_t>(phys) + (i << PAGE_SHIFT),
				flags & FLAGS::FLAGS_MASK
			};
			// Map page
			dirPtr->directories[i] = reinterpret_cast<directory_t*>(directory.value());
		}

		// Directory pointer flags
		const auto dirPtrFlags = kflags<FLAGS> {
			reinterpret_cast<std::size_t>(dirPtr) & 0x7FFFFFFF,
			flags & FLAGS::FLAGS_MASK
		};
		// Insert page directory pointer
		dirPtr = reinterpret_cast<directoryPointer_t*>(dirPtrFlags.value());

	}

	// Map virtual page to physical page (single directory pointer)
	void paging::mapDirectoryPointer(const page_t* phys, const pointer_t virt, const kflags<FLAGS> flags) noexcept {
		// Get pointer to page map level 4
		const auto pml4 = reinterpret_cast<pml4_t*>(outCR3());
		// Map page to curent page map level 4
		paging::mapDirectoryPointer(pml4, phys, virt, flags);
	}


	// Map virtual page to physical page (single directory, explicit pml4)
	void paging::mapDirectory(pml4_t* const pml4, const page_t* phys, const pointer_t virt, const kflags<FLAGS> flags) noexcept {

		// Check alignment
		if (
			!klib::kalignCheck(phys, PAGE_SHIFT)	||
			!klib::kalignCheck(virt, PAGE_SHIFT)
		) {
			// Bad align detected
			return;
		}

		// Page map level 4 table table index from virtual address
		const auto pml4ID	= (reinterpret_cast<std::size_t>(virt) >> 39) & 0x1FF;
		// Page directory pointer table index from virtual address
		const auto dirPtrID	= (reinterpret_cast<std::size_t>(virt) >> 30) & 0x1FF;

		// Get page directory pointer
		auto &dirPtr = pml4->pointers[pml4ID];
		// Check if page directory pointer is present or not
		if (!paging::checkFlags(dirPtr, static_cast<kflags<FLAGS>>(FLAGS::PRESENT))) {
			// Allocate page directory pointer
			dirPtr = paging::makeDirectoryPointer();
		}

		// Get page directory
		auto &dir = dirPtr->directories[dirPtrID];
		// Check if page directory is present or not
		if (!paging::checkFlags(dir, static_cast<kflags<FLAGS>>(FLAGS::PRESENT))) {
			// Allocate page table
			dir = paging::makeDirectory();
		}

		// Get page pointer and map physical page
		for (auto i = 0U; i < (sizeof(page_t) >> 3); i++) {
			// Get phys page ID
			const auto page	= kflags<FLAGS> {
				reinterpret_cast<std::size_t>(phys) + (i << PAGE_SHIFT),
				flags & FLAGS::FLAGS_MASK
			};
			// Map page
			dir->tables[i]	= reinterpret_cast<table_t*>(page.value());
		}

		// Get directory pointer flags
		const auto dirPtrFlags = kflags<FLAGS> {
			reinterpret_cast<std::size_t>(dirPtr) & 0x7FFFFFFF,
			flags & FLAGS::FLAGS_MASK
		};
		// Get directory flags
		const auto dirFlags = kflags<FLAGS> {
			reinterpret_cast<std::size_t>(dir) & 0x7FFFFFFF,
			flags & FLAGS::FLAGS_MASK
		};
		// Insert page directory pointer
		dirPtr	= reinterpret_cast<directoryPointer_t*>(dirPtrFlags.value());
		// Insert page directory
		dir	= reinterpret_cast<directory_t*>(dirFlags.value());

	}

	// Map virtual page to physical page (single directory)
	void paging::mapDirectory(const page_t* phys, const pointer_t virt, const kflags<FLAGS> flags) noexcept {
		// Get pointer to page map level 4
		const auto pml4 = reinterpret_cast<pml4_t*>(outCR3());
		// Map page to curent page map level 4
		paging::mapDirectory(pml4, phys, virt, flags);
	}


	// Map virtual page to physical page (single table, explicit pml4)
	void paging::mapTable(pml4_t* const pml4, const page_t* phys, const pointer_t virt, const kflags<FLAGS> flags) noexcept {

		// Check alignment
		if (
			!klib::kalignCheck(phys, PAGE_SHIFT)	||
			!klib::kalignCheck(virt, PAGE_SHIFT)
		) {
			// Bad align detected
			return;
		}

		// Page map level 4 table table index from virtual address
		const auto pml4ID	= (reinterpret_cast<std::size_t>(virt) >> 39) & 0x1FF;
		// Page directory pointer table index from virtual address
		const auto dirPtrID	= (reinterpret_cast<std::size_t>(virt) >> 30) & 0x1FF;
		// Page directory table entry index from virtual address
		const auto dirID	= (reinterpret_cast<std::size_t>(virt) >> 21) & 0x1FF;

		// Get page directory pointer
		auto &dirPtr = pml4->pointers[pml4ID];
		// Check if page directory pointer is present or not
		if (!paging::checkFlags(dirPtr, static_cast<kflags<FLAGS>>(FLAGS::PRESENT))) {
			// Allocate page directory pointer
			dirPtr = paging::makeDirectoryPointer();
		}

		// Get page directory
		auto &dir = dirPtr->directories[dirPtrID];
		// Check if page directory is present or not
		if (!paging::checkFlags(dir, static_cast<kflags<FLAGS>>(FLAGS::PRESENT))) {
			// Allocate page directory
			dir = paging::makeDirectory();
		}

		// Get page table
		auto &table = dir->tables[dirID];
		// Check if page directory is present or not
		if (!paging::checkFlags(table, static_cast<kflags<FLAGS>>(FLAGS::PRESENT))) {
			// Allocate page table
			table = paging::makeTable();
		}

		// Get page pointer and map physical page
		for (auto i = 0U; i < (sizeof(page_t) >> 3); i++) {
			// Get phys page ID
			auto page = kflags<FLAGS> {
				reinterpret_cast<std::size_t>(phys) + (i << PAGE_SHIFT),
				flags & FLAGS::FLAGS_MASK
			};
			// Map page
			table->pages[i] = reinterpret_cast<page_t*>(page.value());
		}

		// Get directory pointer flags
		const auto dirPtrFlags = kflags<FLAGS> {
			reinterpret_cast<std::size_t>(dirPtr) & 0x7FFFFFFF,
			flags & FLAGS::FLAGS_MASK
		};
		// Get directory flags
		const auto dirFlags = kflags<FLAGS> {
			reinterpret_cast<std::size_t>(dir) & 0x7FFFFFFF,
			flags & FLAGS::FLAGS_MASK
		};
		// Get table flags
		const auto tableFlags = kflags<FLAGS> {
			reinterpret_cast<std::size_t>(table) & 0x7FFFFFFF,
			flags & FLAGS::FLAGS_MASK
		};
		// Insert page directory pointer
		dirPtr	= reinterpret_cast<directoryPointer_t*>(dirPtrFlags.value());
		// Insert page directory
		dir	= reinterpret_cast<directory_t*>(dirFlags.value());
		// Insert page table
		table	= reinterpret_cast<table_t*>(tableFlags.value());

		//
		//klib::kprintf("Table mapped:\t0x%p -> 0x%p [%3d, %3d, %3d, %3c]", phys, virt, pml4ID, dirPtrID, dirID, '-');

	}

	// Map virtual page to physical page (single table)
	void paging::mapTable(const page_t* phys, const pointer_t virt, const kflags<FLAGS> flags) noexcept {
		// Get pointer to page map level 4
		const auto pml4 = reinterpret_cast<pml4_t*>(outCR3());
		// Map page to curent page map level 4
		paging::mapTable(pml4, phys, virt, flags);
	}


	// Map virtual page to physical page (single page, explicit page directory)
	void paging::mapPage(pml4_t* const pml4, const page_t* phys, const pointer_t virt, const kflags<FLAGS> flags) noexcept {

		// Check alignment
		if (
			!klib::kalignCheck(phys, PAGE_SHIFT)	||
			!klib::kalignCheck(virt, PAGE_SHIFT)
		) {
			// Bad align detected
			return;
		}

		// Page map level 4 table table index from virtual address
		const auto pml4ID	= (reinterpret_cast<std::size_t>(virt) >> 39) & 0x1FF;
		// Page directory pointer table index from virtual address
		const auto dirPtrID	= (reinterpret_cast<std::size_t>(virt) >> 30) & 0x1FF;
		// Page directory table entry index from virtual address
		const auto dirID	= (reinterpret_cast<std::size_t>(virt) >> 21) & 0x1FF;
		// Page table table entry index from virtual address
		const auto tabID	= (reinterpret_cast<std::size_t>(virt) >> PAGE_SHIFT) & 0x1FF;

		// Get page directory pointer
		auto &dirPtr = pml4->pointers[pml4ID];
		// Check if page directory pointer is present or not
		if (!paging::checkFlags(dirPtr, static_cast<kflags<FLAGS>>(FLAGS::PRESENT))) {
			// Allocate page directory pointer
			dirPtr = paging::makeDirectoryPointer();
		}

		// Get page directory
		auto &dir = dirPtr->directories[dirPtrID];
		// Check if page directory is present or not
		if (!paging::checkFlags(dir, static_cast<kflags<FLAGS>>(FLAGS::PRESENT))) {
			// Allocate page directory
			dir = paging::makeDirectory();
		}

		// Get page table
		auto &table = dir->tables[dirID];
		// Check if page directory is present or not
		if (!paging::checkFlags(table, static_cast<kflags<FLAGS>>(FLAGS::PRESENT))) {
			// Allocate page table
			table = paging::makeTable();
		}

		// Get page
		auto &page = table->pages[tabID];

		// Get directory pointer flags
		const auto dirPtrFlags = kflags<FLAGS> {
			reinterpret_cast<std::size_t>(dirPtr) & 0x7FFFFFFF,
			flags & FLAGS::FLAGS_MASK
		};
		// Get directory flags
		const auto dirFlags = kflags<FLAGS> {
			reinterpret_cast<std::size_t>(dir) & 0x7FFFFFFF,
			flags & FLAGS::FLAGS_MASK
		};
		// Get table flags
		const auto tableFlags = kflags<FLAGS> {
			reinterpret_cast<std::size_t>(table) & 0x7FFFFFFF,
			flags & FLAGS::FLAGS_MASK
		};
		// Get page flags
		const auto pageFlags = kflags<FLAGS> {
			reinterpret_cast<std::size_t>(page) & 0x7FFFFFFF,
			flags & FLAGS::FLAGS_MASK
		};

		// Insert page directory pointer
		dirPtr	= reinterpret_cast<directoryPointer_t*>(dirPtrFlags.value());
		// Insert page directory
		dir	= reinterpret_cast<directory_t*>(dirFlags.value());
		// Insert page table
		table	= reinterpret_cast<table_t*>(tableFlags.value());
		// Insert page pointer
		page	= reinterpret_cast<page_t*>(pageFlags.value());

	}

	// Map virtual page to physical page (single page)
	void paging::mapPage(const page_t* phys, const pointer_t virt, const kflags<FLAGS> flags) noexcept {
		// Get pointer to page map level 4
		const auto pml4 = reinterpret_cast<pml4_t*>(outCR3());
		// Map page to curent page map level 4
		paging::mapPage(pml4, phys, virt, flags);
	}


	// Convert virtual address to physical address
	[[nodiscard]]
	pointer_t paging::translate(const pointer_t virt) noexcept {

		// Page map level 4 table table index from virtual address
		const auto pml4ID	= (reinterpret_cast<std::size_t>(virt) >> 39) & 0x1FF;
		// Page directory pointer table index from virtual address
		const auto dirPtrID	= (reinterpret_cast<std::size_t>(virt) >> 30) & 0x1FF;
		// Page directory table entry index from virtual address
		const auto dirID	= (reinterpret_cast<std::size_t>(virt) >> 21) & 0x1FF;
		// Page table table entry index from virtual address
		const auto tabID	= (reinterpret_cast<std::size_t>(virt) >> PAGE_SHIFT) & 0x1FF;

		// Get pointer to pml4
		const auto pml4		= reinterpret_cast<const pml4_t*>(outCR3());

		// Get page directory pointer
		const auto dirPtr = pml4->pointers[pml4ID];
		// Check if page directory pointer is present or not
		if (!paging::checkFlags(dirPtr, static_cast<kflags<FLAGS>>(FLAGS::PRESENT))) {
			// Page or table is not present
			return nullptr;
		}

		// Get page directory
		const auto dir = dirPtr->directories[dirPtrID];
		// Check if page directory is present or not
		if (!paging::checkFlags(dir, static_cast<kflags<FLAGS>>(FLAGS::PRESENT))) {
			// Page or table is not present
			return nullptr;
		}

		// Get page table pointer
		const auto table = dir->tables[dirID];
		// Check if page table is present or not
		if (!paging::checkFlags(table, static_cast<kflags<FLAGS>>(FLAGS::PRESENT))) {
			// Page or table is not present
			return nullptr;
		}

		// Get page table pointer
		const auto page = table->pages[tabID];
		// Check if page is present or not
		if (!paging::checkFlags(page, static_cast<kflags<FLAGS>>(FLAGS::PRESENT))) {
			// Page or table is not present
			return nullptr;
		}

		// Get physical address of page from page table (43 MSB)
		const auto address	= kflags<FLAGS> {
			reinterpret_cast<std::size_t>(page)
		};
		// Get physical offset in psge from virtual address`s (21 LSB)
		const auto offset	= kflags<FLAGS> {
			reinterpret_cast<std::size_t>(virt)
		};
		// Return physical address
		return reinterpret_cast<pointer_t>(((address & FLAGS::PHYS_ADDR_MASK) | (offset & FLAGS::FLAGS_MASK)).value());

	}


	// Page Fault Exception handler
	void paging::exHandler(const register_t* regs) noexcept {

		// Disable IRQ
		irq::disable();

		// Write Multiboot magic error message message
		klib::kprintf(
			"EXCEPTION [#%d]\t-> (%s)\r\n"
			"CAUSED BY:\t%s%s%s\r\n"
			"FROM:\t\t%s space\r\n"
			"WHEN:\t\tattempting to %s\r\n"
			"ADDRESS:\t0x%p\r\n"
			"WHICH IS:\tnot %s\r\n",
			regs->number,
			except::NAME[regs->number],
			((regs->param & 0x18) == 0U) ? "ACCESS VIOLATION"	: "",
			((regs->param & 0x10) == 0U) ? ""			: "INSTRUCTION FETCH",
			((regs->param & 0x08) == 0U) ? ""			: "RESERVED BIT SET",
			((regs->param & 0x04) == 0U) ? "KERNEL"			: "USER",
			((regs->param & 0x02) == 0U) ? "READ"			: "WRITE",
			reinterpret_cast<const pointer_t>(outCR2()),
			((regs->param & 0x01) == 0U) ? "PRESENT"		: "PRIVILEGED"
		);

		// Hang here
		cpuHalt();

	}


	// Set page directory
	void paging::flush(const pml4_t* const dir) noexcept {
		// Set page directory address to CR3
		inCR3(reinterpret_cast<std::ptrdiff_t>(dir) & 0x7FFFFFFF);
	}


}	// namespace igros::x86_64

