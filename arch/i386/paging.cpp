////////////////////////////////////////////////////////////////
//
//	Memory paging for x86
//
//	File:	paging.cpp
//	Date:	20 Jan 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#include <arch/cr.hpp>
#include <arch/exceptions.hpp>
#include <arch/paging.hpp>
#include <arch/taskRegs.hpp>

#include <klib/kprint.hpp>


// Arch-dependent code zone
namespace arch {


#ifdef	__cplusplus

	// Workaround because of unimplemented yet physical memory allocator
	extern "C" {

#endif	// __cplusplus


		// Page directory
		extern dword_t	pageDirectory[1024];
		// Page table
		extern dword_t	pageTable[1024];


#ifdef	__cplusplus

	}	// extern "C"

#endif	// __cplusplus


	// Setup paging
	void paging::init() noexcept {

		// Other page directories are unused
		for (auto i = 0u; i < 1024u; i++) {
			// Pages marked as clear
			pageDirectory[i] = dword_t(FLAGS::CLEAR);
		}
		// Map first 4 MB of physical RAM to first 4 MB of virtual RAM
		pageDirectory[0]	 = dword_t(pageTable) & 0x3FFFFFFF;
		pageDirectory[0]	|= dword_t(FLAGS::WRITABLE | FLAGS::PRESENT);

		// Map all pages of first 4MB to first page table
		for (auto j = 0u; j < 1024u; j++) {
			pageTable[j]	 = (j << 12);
			pageTable[j]	|= dword_t(FLAGS::WRITABLE | FLAGS::PRESENT);
		}
		// Also map first 4MB of physical RAM to first 4MB after 3GB in virtual memory
		// (This should be useful for higher-half kernel)
		pageDirectory[768]	 = dword_t(pageTable) & 0x3FFFFFFF;
		// Page marked as present and writable
		pageDirectory[768]	|= dword_t(FLAGS::WRITABLE | FLAGS::PRESENT);
		// Map page table itself to the last page of virtual memory
		pageDirectory[1023]	 = dword_t(pageDirectory) & 0x3FFFFFFF;
		// Page marked as present and writable
		pageDirectory[1023]	|= dword_t(FLAGS::WRITABLE | FLAGS::PRESENT);

		// Install exception handler for page fault
		exHandlerInstall(PAGE_FAULT, paging::exHandler);

		// Setup page directory
		// PD address bits ([0 .. 31] in cr3)
		paging::setDirectory(pageDirectory);

	}


	// Convert virtual address to physical address
	pointer_t paging::toPhys(const pointer_t virtAddr) noexcept {

		// Page directory entry index from virtual address
		auto pdEntryIndex	= (dword_t(virtAddr) & 0xFFC00000) >> 22;
		// Page table entry index from virtual address
		auto ptEntryIndex	= (dword_t(virtAddr) & 0x3FF000) >> 12;
		// Physical pointer to page table
		auto pageTablePtr	= pageDirectory[pdEntryIndex];
		auto pageFlags		= FLAGS(pageTablePtr) & FLAGS::FLAGS_MASK;
		// Check if page table is present or not
		if (FLAGS::CLEAR == (pageFlags & FLAGS::PRESENT)) {
			// Page or table is not present
			return nullptr;
		}

		// Physical pointer to page
		auto pagePtr	= reinterpret_cast<dword_t*>(FLAGS(pageTablePtr) & FLAGS::PHYS_ADDR_MASK)[ptEntryIndex];
		pageFlags	= FLAGS(pagePtr) & FLAGS::FLAGS_MASK;
		// Check if page is present or not
		if (FLAGS::CLEAR == (pageFlags & FLAGS::PRESENT)) {
			// Page or table is not present
			return nullptr;
		}

		// Get physical address of page from page table (20 MSB)
		auto physPageAddr	= FLAGS(pagePtr) & FLAGS::PHYS_ADDR_MASK;
		// Get physical offset from virtual address`s (12 LSB)
		auto physPageOffset	= FLAGS(dword_t(virtAddr)) & FLAGS::FLAGS_MASK;
		// Return physical address
		return pointer_t(physPageAddr | physPageOffset);

	}


	// Page Fault Exception handler
	void paging::exHandler(const taskRegs_t* regs) noexcept {

		// Write Multiboot magic error message message
		klib::kprintf(	u8"EXCEPTION [#%d]\t-> (%s)\r\n"
				u8"CAUSED BY:\t%s%s%s\r\n"
				u8"FROM:\t\t%s space\r\n"
				u8"WHEN:\t\tattempting to %s\r\n"
				u8"ADDRESS:\t0x%p\r\n"
				u8"WHICH IS:\tnot %s\r\n",
				exNumber_t::PAGE_FAULT,
				exName[exNumber_t::PAGE_FAULT],
				((regs->param & 0x18) == 0) ? u8"ACCESS VIOLATION"	: u8"",
				((regs->param & 0x10) == 0) ? u8""			: u8"INSTRUCTION FETCH",
				((regs->param & 0x08) == 0) ? u8""			: u8"RESERVED BIT SET",
				((regs->param & 0x04) == 0) ? u8"KERNEL"		: u8"USER",
				((regs->param & 0x02) == 0) ? u8"READ"			: u8"WRITE",
				outCR2(),
				((regs->param & 0x01) == 0) ? u8"PRESENT"		: u8"PRIVILEGED");

		// Hang here
		while (true) {};

	}


	// Set page directory
	void paging::setDirectory(const pointer_t dir) noexcept {
		inCR3(dword_t(dir) & 0x3FFFFFFF);
	}


}	// namespace arch

