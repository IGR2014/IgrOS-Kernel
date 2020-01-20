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


#include <arch/msr.hpp>
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


		extern quad_t	pageMapLevel4Table[512];
		// Page directory pointer table
		extern quad_t	pageDirectoryPointer[512];
		// Page directory table
		extern quad_t	pageDirectory[512];
		// Page table table
		extern quad_t	pageTable[512];


#ifdef	__cplusplus

	}	// extern "C"

#endif	// __cplusplus


	// Setup paging
	void paging::init() {

		// Other page directories are unused
		for (auto i = 0u; i < 512u; ++i) {
			// Pages marked as clear
			pageMapLevel4Table[i] = quad_t(FLAGS::CLEAR);
		}
		// Map first 1 GB of physical RAM to first 1 GB of virtual RAM
		pageMapLevel4Table[0]		 = quad_t(pageDirectoryPointer) & 0x7FFFFFFF;
		// Page marked as present and writable
		pageMapLevel4Table[0]		|= quad_t(FLAGS::WRITABLE | FLAGS::PRESENT);
		// Map first 1 GB of physical RAM to first 1 GB of virtual RAM
		pageMapLevel4Table[511]		 = quad_t(pageDirectoryPointer) & 0x7FFFFFFF;
		// Page marked as present and writable
		pageMapLevel4Table[511]		|= quad_t(FLAGS::WRITABLE | FLAGS::PRESENT);

		// Other page directories are unused
		for (auto j = 0u; j < 512u; ++j) {
			// Pages marked as clear
			pageDirectoryPointer[j] = quad_t(FLAGS::CLEAR);
		}
		// Map first 1 GB of physical RAM to first 1 GB of virtual RAM
		pageDirectoryPointer[0]		 = quad_t(pageDirectory) & 0x7FFFFFFF;
		// Page marked as present and writable
		pageDirectoryPointer[0]		|= quad_t(FLAGS::WRITABLE | FLAGS::PRESENT);
		// Map first 1 GB of physical RAM to first 1 GB of virtual RAM
		pageDirectoryPointer[510]	 = quad_t(pageDirectory) & 0x7FFFFFFF;
		// Page marked as present and writable
		pageDirectoryPointer[510]	|= quad_t(FLAGS::WRITABLE | FLAGS::PRESENT);

		// Map all pages of first 1 GB to first page table
		// Note 2 MB pages!
		for (auto k = 0u; k < 2u; k++) {
			pageDirectory[k]	 = (k << 21);
			// Page marked as present and writable
			pageDirectory[k]	|= quad_t(FLAGS::HUGE | FLAGS::WRITABLE | FLAGS::PRESENT);
		}

		// Install exception handler for page fault
		exHandlerInstall(PAGE_FAULT, paging::exHandler);

		// Setup page directory
		// PD address bits ([0 .. 63] in cr3)
		paging::setDirectory(pageMapLevel4Table);

	}


	// Convert virtual address to physical address
	pointer_t paging::toPhys(const pointer_t virtAddr) {

		// Page map level 4 table table index from virtual address
		auto pml4EntryIndex	= (quad_t(virtAddr) & 0xFF8000000000) >> 39;
		// Page directory pointer table index from virtual address
		auto pdpEntryIndex	= (quad_t(virtAddr) & 0x7FC0000000) >> 30;
		// Page directory table entry index from virtual address
		auto pdEntryIndex	= (quad_t(virtAddr) & 0x3FE00000) >> 21;
		// Physical pointer to page table
		auto pageDirectoryPtr	= pageMapLevel4Table[pml4EntryIndex];
		auto pageFlags		= FLAGS(pageDirectoryPtr);
		// Check if page table is present or not
		if (FLAGS::CLEAR == (pageFlags & FLAGS::PRESENT)) {
			// Page or table is not present
			return nullptr;
		}

		// Physical pointer to page table
		auto pageTablePtr	= reinterpret_cast<quad_t*>(FLAGS(pageDirectoryPtr) & FLAGS::PHYS_ADDR_MASK)[pdpEntryIndex];
		pageFlags		= FLAGS(pageTablePtr);
		// Check if page table is present or not
		if (FLAGS::CLEAR == (pageFlags & FLAGS::PRESENT)) {
			// Page or table is not present
			return nullptr;
		}

		// Physical pointer to page
		auto pagePtr	= reinterpret_cast<quad_t*>(FLAGS(pageTablePtr) & FLAGS::PHYS_ADDR_MASK)[pdEntryIndex];
		pageFlags	= FLAGS(pagePtr);
		// Check if page is present or not
		if (FLAGS::CLEAR == (pageFlags & FLAGS::PRESENT)) {
			// Page or table is not present
			return nullptr;
		}

		// Get physical address of page from page table (52 MSB)
		auto physPageAddr	= (pagePtr & ~0xFFFFF);
		// Get physical offset from virtual address`s (12 LSB)
		auto physPageOffset	= quad_t(virtAddr) & 0xFFFFF;
		// Return physical address
		return pointer_t(physPageAddr | physPageOffset);

	}


	// Page Fault Exception handler
	void paging::exHandler(const taskRegs_t* regs) {

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
		inCR3(quad_t(dir) & 0x7FFFFFFF);
	}


}	// namespace arch

