////////////////////////////////////////////////////////////////
//
//	Memory paging for x86
//
//	File:	paging.cpp
//	Date:	13 Jun 2019
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
//	All rights reserved.
//
//


#include <arch/cr.hpp>
#include <arch/exceptions.hpp>
#include <arch/paging.hpp>
#include <arch/taskRegs.hpp>

#include <drivers/vmem.hpp>

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
	void pagingSetup() {

		// Other page directories are unused
		for (dword_t i = 0; i < 512; ++i) {

			// Pages marked as clear
			pageMapLevel4Table[i] = quad_t(pagingFlags_t::CLEAR);

		}

		// Map first 1 GB of physical RAM to first 1 GB of virtual RAM
		pageMapLevel4Table[0]		 = quad_t(pageDirectoryPointer) & 0x7FFFFFFF;
		// Page marked as present and writable
		pageMapLevel4Table[0]		|= quad_t(pagingFlags_t::WRITABLE | pagingFlags_t::PRESENT);

		// Map first 1 GB of physical RAM to first 1 GB of virtual RAM
		pageMapLevel4Table[511]		 = quad_t(pageDirectoryPointer) & 0x7FFFFFFF;
		// Page marked as present and writable
		pageMapLevel4Table[511]		|= quad_t(pagingFlags_t::WRITABLE | pagingFlags_t::PRESENT);

		// Other page directories are unused
		for (dword_t j = 0; j < 512; ++j) {

			// Pages marked as clear
			pageDirectoryPointer[j] = quad_t(pagingFlags_t::CLEAR);

		}

		// Map first 1 GB of physical RAM to first 1 GB of virtual RAM
		pageDirectoryPointer[0]		 = quad_t(pageDirectory) & 0x7FFFFFFF;
		// Page marked as present and writable
		pageDirectoryPointer[0]		|= quad_t(pagingFlags_t::WRITABLE | pagingFlags_t::PRESENT);

		// Map first 1 GB of physical RAM to first 1 GB of virtual RAM
		pageDirectoryPointer[510]	 = quad_t(pageDirectory) & 0x7FFFFFFF;
		// Page marked as present and writable
		pageDirectoryPointer[510]	|= quad_t(pagingFlags_t::WRITABLE | pagingFlags_t::PRESENT);

		// Map all pages of first 1 GB to first page table
		// Note 2 MB pages!
		for (sdword_t k = 0; k < 2; ++k) {

			pageDirectory[k]	 = (k << 21);
			// Page marked as present and writable
			pageDirectory[k]	|= quad_t(pagingFlags_t::HUGE | pagingFlags_t::WRITABLE | pagingFlags_t::PRESENT);

		}

		// Install exception handler for page fault
		exHandlerInstall(PAGE_FAULT, pagingFaultExceptionHandler);

		// Setup page directory
		// PD address bits ([0 .. 63] in cr3)
		inCR3(quad_t(pageMapLevel4Table) & 0x7FFFFFFF);

		// Enable Page Size Extension
		// Set PAE bit ([5] in cr4)
		inCR4(outCR4() | 0x00000020);

		// Long mode paging
		asm volatile("movq $0xC0000080, %rcx\n"
			     "rdmsr\n"
			     "orq $0x00000100, %rax\n"
			     "wrmsr\n");

		// Enable paging
		// Set PE bit ([31] in cr0)
		inCR0(outCR0() | 0x80000000);

	}


	// Convert virtual address to physical address
	pointer_t pagingVirtToPhys(const pointer_t virtAddr) {

		// Page map level 4 table table index from virtual address
		quad_t pml4EntryIndex	= (quad_t(virtAddr) & 0xFF8000000000) >> 39;
		// Page directory pointer table index from virtual address
		quad_t pdpEntryIndex	= (quad_t(virtAddr) & 0x7FC0000000) >> 30;
		// Page directory table entry index from virtual address
		quad_t pdEntryIndex	= (quad_t(virtAddr) & 0x3FE00000) >> 21;

		// Physical pointer to page table
		quad_t		pageDirectoryPtr	= pageMapLevel4Table[pml4EntryIndex];
		pagingFlags_t	pageFlags		= pagingFlags_t(pageDirectoryPtr);

		// Check if page table is present or not
		if ((pageFlags & pagingFlags_t::PRESENT) == pagingFlags_t::CLEAR) {

			// Page or table is not present
			return nullptr;

		}

		// Physical pointer to page table
		quad_t	pageTablePtr	= reinterpret_cast<quad_t*>(pageDirectoryPtr & ~0xFFF)[pdpEntryIndex];
		pageFlags		= pagingFlags_t(pageTablePtr);

		// Check if page table is present or not
		if ((pageFlags & pagingFlags_t::PRESENT) == pagingFlags_t::CLEAR) {

			// Page or table is not present
			return nullptr;

		}

		// Physical pointer to page
		quad_t pagePtr	= reinterpret_cast<quad_t*>(pageTablePtr & ~0xFFF)[pdEntryIndex];
		pageFlags	= pagingFlags_t(pagePtr);

		// Check if page is present or not
		if ((pageFlags & pagingFlags_t::PRESENT) == pagingFlags_t::CLEAR) {

			// Page or table is not present
			return nullptr;

		}

		// Get physical address of page from page table (52 MSB)
		quad_t physPageAddr	= (pagePtr & ~0xFFFFF);
		// Get physical offset from virtual address`s (12 LSB)
		quad_t physPageOffset	= quad_t(virtAddr) & 0xFFFFF;

		// Return physical address
		return pointer_t(physPageAddr | physPageOffset);

	}


	// Page Fault Exception handler
	void pagingFaultExceptionHandler(const taskRegs_t* regs) {

		// Print buffer
		sbyte_t text[64];

		// Exception info
		vmemWrite("CAUSED BY:\t");
		vmemWrite(((regs->param & 0x18) == 0) ? "ACCESS VIOLATION" : "");
		vmemWrite(((regs->param & 0x10) == 0) ? "" : "INSTRUCTION FETCH");
		vmemWrite(((regs->param & 0x08) == 0) ? "" : "RESERVED BIT SET");
		vmemWrite("\r\nFROM:\t\t");
		vmemWrite(((regs->param & 0x04) == 0) ? "KERNEL" : "USER");
		vmemWrite(" space\r\n");
		vmemWrite("WHEN:\t\tattempting to ");
		vmemWrite(((regs->param & 0x02) == 0) ? "READ" : "WRITE");
		vmemWrite("\r\nADDRESS:\t0x");
		klib::kitoa(text, 64, outCR2(), klib::base::HEX);
		vmemWrite(text);
		vmemWrite("\r\nWHICH IS:\tNON-");
		vmemWrite(((regs->param & 0x01) == 0) ? "PRESENT\r\n" : "PRIVILEGED\r\n");
		vmemWrite("\r\n");

		// Hang here
		while (true) {};

	}


}	// namespace arch

