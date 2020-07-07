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


	// Identity map kernel + map higher-half + self-map page directory
	void paging::init() noexcept {

		// Install exception handler for page fault
		except::install(except::NUMBER::PAGE_FAULT, paging::exHandler);

		// Initialize pages for page tables
		paging::heap(const_cast<byte_t*>(&_SECTION_KERNEL_END_), PAGE_SIZE << 6);

		// Create flags
		constexpr auto flags = flags_t::WRITABLE | flags_t::PRESENT;

		// Allocate page table
		auto pageTable = static_cast<table_t*>(paging::allocate());
		// Identity map first 4MB of physical memory to first 4Mb in virtual memory
		for (auto i = 0u; i < 1024u; i++) {
			// Get pointer to page
			auto page		= static_cast<flags_t>(i << PAGE_SHIFT);
			// Identity mapping
			pageTable->pages[i]	= reinterpret_cast<page_t*>(page | flags);
		}

		// Allocate page directory
		auto pageDir = static_cast<directory_t*>(paging::allocate());
		// Zero PD
		klib::kmemset(pageDir, 1024u, static_cast<const dword_t>(flags_t::CLEAR));

		// Identity map first 4MB of physical memory to first 4Mb in virtual memory
		pageDir->tables[0]	= reinterpret_cast<table_t*>(reinterpret_cast<dword_t>(pageTable) & 0x3FFFFFFF | static_cast<dword_t>(flags));
		// Also map first 4MB of physical memory to 3Gb offset in virtual memory
		pageDir->tables[768]	= reinterpret_cast<table_t*>(reinterpret_cast<dword_t>(pageTable) & 0x3FFFFFFF | static_cast<dword_t>(flags));
		// Map page directory to itself
		pageDir->tables[1023]	= reinterpret_cast<table_t*>(reinterpret_cast<dword_t>(pageDir) & 0x3FFFFFFF | static_cast<dword_t>(flags));

		// Setup page directory
		// PD address bits ([0 .. 31] in cr3)
		paging::setDirectory(pageDir);
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
		auto tempPtr	= reinterpret_cast<dword_t>(phys);
		auto tempSize	= size;
		// Check alignment
		if (0x00 != (tempPtr & PAGE_MASK)) {
			// Align pointer at page size alignment
			tempPtr		= ((tempPtr + PAGE_SIZE) & ~PAGE_MASK);
			// Adjust size
			tempSize	-= (tempPtr - reinterpret_cast<dword_t>(phys));
		}
		// Get number of pages
		auto numOfPages = (tempSize >> PAGE_SHIFT);
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
	[[nodiscard]] pointer_t paging::allocate() noexcept {
		// Check if pages exist
		if (paging::mFreePages->next != paging::mFreePages) {
			// Get free page
			auto addr		= paging::mFreePages;
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
		auto tempPage = reinterpret_cast<dword_t>(page);
		if (0x00 != (tempPage & PAGE_MASK)) {
			// Bad align detected
			return;
		}
		// Deallocate page back to heap free list
		static_cast<page_t*>(page)->next = paging::mFreePages;
		paging::mFreePages = static_cast<page_t*>(page);
	}


	// Map virtual page to physical page (explicit page directory)
	void paging::map(directory_t* dir, const page_t* phys, const pointer_t virt, const flags_t flags) noexcept {

		// Check alignment
		auto tempPhys = reinterpret_cast<dword_t>(phys);
		auto tempVirt = reinterpret_cast<dword_t>(virt);
		if (	(0x00 != (tempPhys & PAGE_MASK)
			|| (0x00 != (tempVirt & PAGE_MASK)))) {
			// Bad align detected
			return;
		}

		// Page directory entry index from virtual address
		auto pdIndex	= (reinterpret_cast<dword_t>(virt) >> 22) & 0x3FF;
		// Page table entry index from virtual address
		auto ptIndex	= (reinterpret_cast<dword_t>(virt) >> PAGE_SHIFT) & 0x3FF;

		// Get page table pointer
		auto &pageTable	= dir->tables[pdIndex];
		// Get page flags
		auto pageFlags	= static_cast<flags_t>(reinterpret_cast<dword_t>(pageTable)) & flags_t::FLAGS_MASK;
		// Check if page table is present or not
		if (flags_t::CLEAR == (pageFlags & flags_t::PRESENT)) {
			// Allocate page table
			auto pt = static_cast<table_t*>(paging::allocate());
			// Zero PT
			klib::kmemset(pt, 1024u, static_cast<dword_t>(flags_t::CLEAR));
			// Set flags
			pageTable = reinterpret_cast<table_t*>(reinterpret_cast<dword_t>(pt) & 0x3FFFFFFF | static_cast<dword_t>(flags & flags_t::FLAGS_MASK));
		}

		// Get page pointer
		auto &page	= pageTable->pages[ptIndex];
		// Map physical page
		page		= reinterpret_cast<page_t*>(reinterpret_cast<dword_t>(phys) & 0x3FFFFFFF | static_cast<dword_t>(flags & flags_t::FLAGS_MASK));

	}

	// Map virtual page to physical page
	void paging::map(const page_t* phys, const pointer_t virt, const flags_t flags) noexcept {
		// Get pointer to page directory
		auto pageDir = reinterpret_cast<directory_t*>(outCR3());
		// Map page to curent page directory
		paging::map(pageDir, phys, virt, flags);
		// Setup page directory
		// PD address bits ([0 .. 31] in cr3)
		paging::setDirectory(pageDir);
		// Enable paging
		paging::enable();
	}


	// Convert virtual address to physical address
	pointer_t paging::toPhys(const pointer_t virt) noexcept {

		// Page directory entry index from virtual address
		auto pdIndex	= (reinterpret_cast<dword_t>(virt) >> 22) & 0x3FF;
		// Page table entry index from virtual address
		auto ptIndex	= (reinterpret_cast<dword_t>(virt) >> PAGE_SHIFT) & 0x3FF;

		// Get pointer to page directory
		auto pageDir	= reinterpret_cast<const directory_t*>(outCR3());
		// Get page table pointer
		auto pageTable	= pageDir->tables[pdIndex];
		// Get page flags
		auto pageFlags	= static_cast<flags_t>(reinterpret_cast<dword_t>(pageTable)) & flags_t::FLAGS_MASK;
		// Check if page table is present or not
		if (flags_t::CLEAR == (pageFlags & flags_t::PRESENT)) {
			// Page or table is not present
			return nullptr;
		}

		// Get page pointer
		auto page	= pageTable->pages[ptIndex];
		// Get page flags
		pageFlags	= static_cast<flags_t>(reinterpret_cast<dword_t>(page)) & flags_t::FLAGS_MASK;
		// Check if page table is present or not
		if (flags_t::CLEAR == (pageFlags & flags_t::PRESENT)) {
			// Page or table is not present
			return nullptr;
		}

		// Get physical address of page from page table (20 MSB)
		auto pageAddr	= static_cast<flags_t>(reinterpret_cast<dword_t>(page)) & flags_t::PHYS_ADDR_MASK;
		// Get physical offset in psge from virtual address`s (12 LSB)
		auto pageOffset	= static_cast<flags_t>(reinterpret_cast<dword_t>(virt)) & flags_t::FLAGS_MASK;

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
	void paging::setDirectory(const directory_t* dir) noexcept {
		// Set page directory address to CR3
		inCR3(reinterpret_cast<dword_t>(dir) & 0x3FFFFFFF);
	}


}	// namespace igros::arch

