////////////////////////////////////////////////////////////////
//
//	Memory paging for x86
//
//	File:	paging.cpp
//	Date:	07 Jul 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#include <msr.hpp>
#include <cr.hpp>
#include <irq.hpp>
#include <exceptions.hpp>
#include <paging.hpp>
#include <taskRegs.hpp>
#include <cpu.hpp>

#include <klib/kprint.hpp>
#include <klib/kmemory.hpp>


// Kernel start and end
extern const igros::byte_t _SECTION_KERNEL_START_;
extern const igros::byte_t _SECTION_KERNEL_END_;


// Arch-dependent code zone
namespace igros::arch {


	// Free pages list
	table_t* paging::mFreePages = reinterpret_cast<table_t*>(&paging::mFreePages);


	// Setup paging
	void paging::init() noexcept {

		// Install exception handler for page fault
		except::install(except::NUMBER::PAGE_FAULT, paging::exHandler);

		// Initialize pages for page tables
		paging::heap(const_cast<byte_t*>(&_SECTION_KERNEL_END_), 4096ull << 6);

		// Create flags
		constexpr auto flags = flags_t::WRITABLE | flags_t::PRESENT;

		// Allocate page directory
		auto pageDirectory = static_cast<directory_t*>(paging::allocate());
		// 
		for (auto i = 0ull; i < 2ull; i++) {
			// Create page table index
			auto page = static_cast<flags_t>(i << PAGE_SHIFT);
			// Map page to 
			pageDirectory->tables[i] = reinterpret_cast<table_t*>(page | flags_t::HUGE | flags);
		}

		// Allocate page directory ptr
		auto pageDirectoryPtr = static_cast<directoryPointer_t*>(paging::allocate());
		// Zero PD
		klib::kmemset(pageDirectoryPtr, 512u, static_cast<const quad_t>(flags_t::CLEAR));

		// 
		pageDirectoryPtr->directories[0]	= reinterpret_cast<directory_t*>(reinterpret_cast<quad_t>(pageDirectory) & 0x7FFFFFFF | static_cast<quad_t>(flags));
		// Map page directory to itself
		pageDirectoryPtr->directories[510]	= reinterpret_cast<directory_t*>(reinterpret_cast<quad_t>(pageDirectory) & 0x7FFFFFFF | static_cast<quad_t>(flags));

		// Allocate page map level 4
		auto pml4 = reinterpret_cast<pml4_t*>(paging::allocate());
		// Zero PD
		klib::kmemset(pml4, 512u, static_cast<const quad_t>(flags_t::CLEAR));

		// 
		pml4->pointers[0]	= reinterpret_cast<directoryPointer_t*>(reinterpret_cast<quad_t>(pageDirectoryPtr) & 0x7FFFFFFF | static_cast<quad_t>(flags));
		// 
		pml4->pointers[511]	= reinterpret_cast<directoryPointer_t*>(reinterpret_cast<quad_t>(pageDirectoryPtr) & 0x7FFFFFFF | static_cast<quad_t>(flags));

		// Setup page directory
		// PD address bits ([0 .. 63] in cr3)
		paging::setDirectory(pml4);
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
		auto tempPhys	= reinterpret_cast<quad_t>(phys);
		auto tempSize	= size;
		// Check alignment
		if (0x00 != (tempPhys & 4095ull)) {
			// Align pointer at page size alignment
			tempPhys	= ((tempPhys + 4096ull) & 0xFFFFFFFFFFFFF000);
			// Adjust size
			tempSize	-= (tempPhys - reinterpret_cast<quad_t>(phys));
		}
		// Get number of pages
		auto numOfPages = (tempSize >> 12);
		// Check input
		if (0ull == numOfPages) {
			return;
		}

		// Convert to page pointer
		auto page	= reinterpret_cast<table_t*>(tempPhys);
		// Link first page to free pages list
		page[0ull].next	= paging::mFreePages;
		// Create linked list of free pages
		for (auto i = 1ull; i < numOfPages; i++) {
			// Link each next page to previous
			page[i].next = &page[i - 1ull];
		}
		// Make last page new list head
		paging::mFreePages = &page[numOfPages - 2ull];

	}


	// Allocate page
	[[nodiscard]] pointer_t paging::allocate() noexcept {
		// Check if pages exist
		if (paging::mFreePages->next != paging::mFreePages) {
			// Get free page
			auto addr		= paging::mFreePages;
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
		auto tempPage = reinterpret_cast<quad_t>(page);
		if (0x00 != (tempPage & PAGE_MASK)) {
			// Bad align detected
			return;
		}
		// Deallocate page back to heap free list
		reinterpret_cast<table_t*>(page)->next = paging::mFreePages;
		paging::mFreePages = reinterpret_cast<table_t*>(page);
	}


	// Map virtual page to physical page
	void paging::map(pml4_t* pml4, const page_t* phys, const pointer_t virt, const flags_t flags) noexcept {

		// Check alignment
		auto tempPhys = reinterpret_cast<quad_t>(phys);
		auto tempVirt = reinterpret_cast<quad_t>(virt);
		if (	(0x00 != (tempPhys & PAGE_MASK)
			|| (0x00 != (tempVirt & PAGE_MASK)))) {
			// Bad align detected
			return;
		}

		// Page map level 4 table table index from virtual address
		auto pml4Index	= (reinterpret_cast<quad_t>(virt) >> 39) & 0x1FF;
		// Page directory pointer table index from virtual address
		auto pdpIndex	= (reinterpret_cast<quad_t>(virt) >> 30) & 0x1FF;
		// Page directory table entry index from virtual address
		auto pdIndex	= (reinterpret_cast<quad_t>(virt) >> 21) & 0x1FF;

		// Get page directory pointer
		auto &pageDirP	= pml4->pointers[pml4Index];
		// Get page flags
		auto pageFlags	= static_cast<flags_t>(reinterpret_cast<quad_t>(pageDirP)) & flags_t::FLAGS_MASK;
		// Check if page directory pointer is present or not
		if (flags_t::CLEAR == (pageFlags & flags_t::PRESENT)) {
			// Allocate page table
			pageDirP = reinterpret_cast<directoryPointer_t*>(paging::allocate());//reinterpret_cast<quad_t>(paging::allocate()) | static_cast<quad_t>(flags));
		}

		// Get page directory
		auto &pageDir	= pageDirP->directories[pdpIndex];
		// Get page flags
		pageFlags	= static_cast<flags_t>(reinterpret_cast<quad_t>(pageDir)) & flags_t::FLAGS_MASK;
		// Check if page directory is present or not
		if (flags_t::CLEAR == (pageFlags & flags_t::PRESENT)) {
			// Allocate page table
			pageDir = reinterpret_cast<directory_t*>(paging::allocate());//reinterpret_cast<quad_t>(paging::allocate()) | static_cast<quad_t>(flags));
		}

		// Get page table pointer
		auto &page	= pageDir->tables[pdIndex];
		// Map physical page
		page		= reinterpret_cast<table_t*>(reinterpret_cast<quad_t>(phys) | static_cast<quad_t>(flags & flags_t::FLAGS_MASK));

	}

	// Map virtual page to physical page
	void paging::map(const page_t* phys, const pointer_t virt, const flags_t flags) noexcept {
		// Get pointer to pml4
		auto pml4 = reinterpret_cast<pml4_t*>(outCR3());
		// Map page
		paging::map(pml4, phys, virt, flags);
		// Setup page directory
		// PD address bits ([0 .. 63] in cr3)
		paging::setDirectory(pml4);
		// Enable Physical Address Extension
		paging::enablePAE();
		// Enable paging
		paging::enable();

	}


	// Convert virtual address to physical address
	pointer_t paging::toPhys(const pointer_t virt) noexcept {

		// Page map level 4 table table index from virtual address
		auto pml4Index	= (reinterpret_cast<quad_t>(virt) >> 39) & 0x1FF;
		// Page directory pointer table index from virtual address
		auto pdpIndex	= (reinterpret_cast<quad_t>(virt) >> 30) & 0x1FF;
		// Page directory table entry index from virtual address
		auto pdIndex	= (reinterpret_cast<quad_t>(virt) >> 21) & 0x1FF;

		// Get pointer to pml4
		auto pageMapL4	= reinterpret_cast<const pml4_t*>(outCR3());
		// Get page directory pointer
		auto pageDirPtr	= pageMapL4->pointers[pml4Index];
		// Get page flags
		auto pageFlags	= static_cast<flags_t>(reinterpret_cast<quad_t>(pageDirPtr)) & flags_t::FLAGS_MASK;
		// Check if page directory pointer is present or not
		if (flags_t::CLEAR == (pageFlags & flags_t::PRESENT)) {
			// Page or table is not present
			return nullptr;
		}

		// Get page directory
		auto pageDir	= pageDirPtr->directories[pdpIndex];
		// Get page flags
		pageFlags	= static_cast<flags_t>(reinterpret_cast<quad_t>(pageDir)) & flags_t::FLAGS_MASK;
		// Check if page directory is present or not
		if (flags_t::CLEAR == (pageFlags & flags_t::PRESENT)) {
			// Page or table is not present
			return nullptr;
		}

		// Get page table pointer
		auto page	= pageDir->tables[pdIndex];
		// Get page flags
		pageFlags	= static_cast<flags_t>(reinterpret_cast<quad_t>(page)) & flags_t::FLAGS_MASK;
		// Check if page table is present or not
		if (flags_t::CLEAR == (pageFlags & flags_t::PRESENT)) {
			// Page or table is not present
			return nullptr;
		}

		// Get physical address of page from page table (52 MSB)
		auto pageAddr	= static_cast<flags_t>(reinterpret_cast<quad_t>(page)) & flags_t::PHYS_ADDR_MASK;
		// Get physical offset in psge from virtual address`s (21 LSB)
		auto pageOffset	= static_cast<flags_t>(reinterpret_cast<quad_t>(virt)) & flags_t::FLAGS_MASK;

		// Return physical address
		return reinterpret_cast<pointer_t>(pageAddr | pageOffset);

	}


	// Page Fault Exception handler
	void paging::exHandler(const taskRegs_t* regs) noexcept {

		// Disable IRQ
		irq::disable();

		// Write Multiboot magic error message message
		klib::kprintf(	u8"EXCEPTION [#%d]\t-> (%s)\r\n"
				u8"CAUSED BY:\t%s%s%s\r\n"
				u8"FROM:\t\t%s space\r\n"
				u8"WHEN:\t\tattempting to %s\r\n"
				u8"ADDRESS:\t0x%p\r\n"
				u8"WHICH IS:\tnot %s\r\n",
				except::NUMBER::PAGE_FAULT,
				except::NAME[static_cast<dword_t>(except::NUMBER::PAGE_FAULT)],
				((regs->param & 0x18) == 0u) ? u8"ACCESS VIOLATION"	: u8"",
				((regs->param & 0x10) == 0u) ? u8""			: u8"INSTRUCTION FETCH",
				((regs->param & 0x08) == 0u) ? u8""			: u8"RESERVED BIT SET",
				((regs->param & 0x04) == 0u) ? u8"KERNEL"		: u8"USER",
				((regs->param & 0x02) == 0u) ? u8"READ"			: u8"WRITE",
				outCR2(),
				((regs->param & 0x01) == 0u) ? u8"PRESENT"		: u8"PRIVILEGED");

		// Hang here
		cpuHalt();

	}


	// Set page directory
	void paging::setDirectory(const pml4_t* dir) noexcept {
		// Set page directory address to CR3
		inCR3(reinterpret_cast<quad_t>(dir) & 0x7FFFFFFF);
	}


}	// namespace igros::arch

