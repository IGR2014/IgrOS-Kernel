////////////////////////////////////////////////////////////////
//
//	Memory paging for x86
//
//	File:	paging.cpp
//	Date:	02 Jul 2020
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
	page_t* paging::mFreePages = reinterpret_cast<page_t*>(&paging::mFreePages);


	// Setup paging
	void paging::init() noexcept {

		/*
		// Other page directories are unused
		for (auto i = 0u; i < 512u; ++i) {
			// Pages marked as clear
			pageMapLevel4Table[i] = static_cast<quad_t>(flags_t::CLEAR);
		}
		// Map first 1 GB of physical RAM to first 1 GB of virtual RAM
		pageMapLevel4Table[0]		 = reinterpret_cast<quad_t>(pageDirectoryPointer) & 0x7FFFFFFF;
		// Page marked as present and writable
		pageMapLevel4Table[0]		|= static_cast<quad_t>(flags_t::WRITABLE | flags_t::PRESENT);
		// Map first 1 GB of physical RAM to first 1 GB of virtual RAM
		pageMapLevel4Table[511]		 = reinterpret_cast<quad_t>(pageDirectoryPointer) & 0x7FFFFFFF;
		// Page marked as present and writable
		pageMapLevel4Table[511]		|= static_cast<quad_t>(flags_t::WRITABLE | flags_t::PRESENT);

		// Other page directories are unused
		for (auto j = 0u; j < 512u; ++j) {
			// Pages marked as clear
			pageDirectoryPointer[j] = static_cast<quad_t>(flags_t::CLEAR);
		}
		// Map first 1 GB of physical RAM to first 1 GB of virtual RAM
		pageDirectoryPointer[0]		 = reinterpret_cast<quad_t>(pageDirectory) & 0x7FFFFFFF;
		// Page marked as present and writable
		pageDirectoryPointer[0]		|= static_cast<quad_t>(flags_t::WRITABLE | flags_t::PRESENT);
		// Map first 1 GB of physical RAM to first 1 GB of virtual RAM
		pageDirectoryPointer[510]	 = reinterpret_cast<quad_t>(pageDirectory) & 0x7FFFFFFF;
		// Page marked as present and writable
		pageDirectoryPointer[510]	|= static_cast<quad_t>(flags_t::WRITABLE | flags_t::PRESENT);

		// Map all pages of first 1 GB to first page table
		// Note 2 MB pages!
		for (auto k = 0u; k < 2u; k++) {
			pageDirectory[k]	 = (k << 21);
			// Page marked as present and writable
			pageDirectory[k]	|= static_cast<quad_t>(flags_t::HUGE | flags_t::WRITABLE | flags_t::PRESENT);
		}
		*/

		// Install exception handler for page fault
		except::install(except::NUMBER::PAGE_FAULT, paging::exHandler);

		// Initialize pages for page tables
		paging::heap(const_cast<byte_t*>(&_SECTION_KERNEL_END_), PAGE_SIZE << 6);

		// Get old page map level 4
		auto pageMapLvl4Old	= reinterpret_cast<pml4_t*>(outCR3());
		// Allocate page map level 4
		auto pageMapLvl4	= reinterpret_cast<pml4_t*>(paging::allocate());
		// Copy old directory to new
		klib::kmemcpy(pageMapLvl4, pageMapLvl4Old, sizeof(pml4_t));
		// Setup page directory
		// PD address bits ([0 .. 31] in cr3)
		paging::setDirectory(pageMapLvl4);

		/*
		// Map first 4 MB of physical RAM to first 4 MB of virtual RAM
		for (auto j = 0u; j < 1024u; j++) {
			// Create page table index
			auto id = reinterpret_cast<const pointer_t>(j << PAGE_SHIFT);
			// Map page to 
			paging::map(reinterpret_cast<const page_t*>(id), id, flags_t::WRITABLE | flags_t::PRESENT);
		}
		*/

		// Map first 4 MB of physical RAM to first 4 MB of virtual RAM
		paging::map(nullptr, nullptr, flags_t::HUGE | flags_t::WRITABLE | flags_t::PRESENT);
		// Also map first 4MB of physical RAM to first 4MB after 3GB in virtual memory
		paging::map(nullptr, reinterpret_cast<const pointer_t>(0xFFFFFFFF80000000), flags_t::HUGE | flags_t::WRITABLE | flags_t::PRESENT);
		// Map page directory to itself
		paging::map(reinterpret_cast<const page_t*>(const_cast<pml4_t*>(pageMapLvl4)), reinterpret_cast<const pointer_t>(0xFFFFFFFFFFFFF000), flags_t::HUGE | flags_t::WRITABLE | flags_t::PRESENT);

		// Setup page directory
		// PD address bits ([0 .. 63] in cr3)
		paging::setDirectory(pageMapLvl4);
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
		auto tempPtr	= reinterpret_cast<quad_t>(phys);
		auto tempSize	= size;
		// Check alignment
		if (0x00 != (tempPtr & 4096ull)) {
			// Align pointer at page size alignment
			tempPtr		= ((tempPtr >> 12) << 12);
			// Adjust size
			tempSize	-= (tempPtr - reinterpret_cast<quad_t>(phys));
		}
		// Get number of pages
		auto numOfPages = (tempSize >> 12);
		// Check input
		if (0ull == numOfPages) {
			return;
		}

		// Convert to page pointer
		auto page	= reinterpret_cast<page_t*>(tempPtr);
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
	[[nodiscard]] page_t* paging::allocate() noexcept {
		// Check if pages exist
		if (paging::mFreePages->next != paging::mFreePages) {
			// Get free page
			auto addr		= paging::mFreePages;
			// Update free pages list
			paging::mFreePages	= addr->next;
			// Return pointer to free page
			return addr;
		}
		// Nothing to return
		return nullptr;
	}

	// Deallocate page
	void paging::deallocate(const page_t* page) noexcept {
		// Check alignment
		auto tempPage = reinterpret_cast<quad_t>(page);
		if (0x00 != (tempPage & PAGE_MASK)) {
			// Bad align detected
			return;
		}
		// Deallocate page back to heap free list
		const_cast<page_t*>(page)->next = paging::mFreePages;
		paging::mFreePages		= const_cast<page_t*>(page);
	}


	// Map virtual page to physical page
	void paging::map(const page_t* phys, const pointer_t virt, const flags_t flags) noexcept {

		// Check alignment
		auto tempPhys = reinterpret_cast<quad_t>(phys);
		auto tempVirt = reinterpret_cast<quad_t>(virt);
		if (	(0x00 != (tempPhys & PAGE_MASK)
			|| (0x00 != (tempVirt & PAGE_MASK)))) {
			// Bad align detected
			return;
		}

		// Page map level 4 table table index from virtual address
		auto pml4Index	= (reinterpret_cast<quad_t>(virt) & 0xFF8000000000) >> 39;
		// Page directory pointer table index from virtual address
		auto pdpIndex	= (reinterpret_cast<quad_t>(virt) & 0x7FC0000000) >> 30;
		// Page directory table entry index from virtual address
		auto pdIndex	= (reinterpret_cast<quad_t>(virt) & 0x3FE00000) >> 21;

		// Get pointer to pml4
		auto pageMapL4	= reinterpret_cast<pml4_t*>(outCR3());
		// Get page directory pointer
		auto &pageDirP	= pageMapL4->pointers[pml4Index];
		// Get page flags
		auto pageFlags	= static_cast<flags_t>(reinterpret_cast<quad_t>(pageDirP)) & flags_t::FLAGS_MASK;
		// Check if page directory pointer is present or not
		if (flags_t::CLEAR == (pageFlags & flags_t::PRESENT)) {
			// Allocate page table
			pageDirP = reinterpret_cast<directoryPointer_t*>(reinterpret_cast<quad_t>(paging::allocate()) & 0x7FFFFFFF);
		}

		// Get page directory
		auto &pageDir	= pageDirP->directories[pdpIndex];
		// Get page flags
		pageFlags	= static_cast<flags_t>(reinterpret_cast<quad_t>(pageDir)) & flags_t::FLAGS_MASK;
		// Check if page directory is present or not
		if (flags_t::CLEAR == (pageFlags & flags_t::PRESENT)) {
			// Allocate page table
			pageDir = reinterpret_cast<directory_t*>(reinterpret_cast<quad_t>(paging::allocate()) & 0x7FFFFFFF);
		}

		// Get page table pointer
		auto &page	= pageDir->tables[pdIndex];
		// Map physical page
		page		= reinterpret_cast<table_t*>(reinterpret_cast<quad_t>(phys) | static_cast<quad_t>(flags & flags_t::FLAGS_MASK));

		// Setup page directory
		// PD address bits ([0 .. 63] in cr3)
		paging::setDirectory(pageMapL4);
		// Enable Physical Address Extension
		paging::enablePAE();
		// Enable paging
		paging::enable();

	}


	// Convert virtual address to physical address
	pointer_t paging::toPhys(const pointer_t virt) noexcept {

		// Page map level 4 table table index from virtual address
		auto pml4Index	= (reinterpret_cast<quad_t>(virt) & 0xFF8000000000) >> 39;
		// Page directory pointer table index from virtual address
		auto pdpIndex	= (reinterpret_cast<quad_t>(virt) & 0x7FC0000000) >> 30;
		// Page directory table entry index from virtual address
		auto pdIndex	= (reinterpret_cast<quad_t>(virt) & 0x3FE00000) >> 21;

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

