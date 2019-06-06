////////////////////////////////////////////////////////////////
//
//	Memory paging for x86
//
//	File:	paging.cpp
//	Date:	06 Jun 2019
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


	// Setup page directory
	void pagingSetupPD(const pointer_t pageDirAddr) {

		// Write page directory address to CR3
		inCR3(reinterpret_cast<quad_t>(pageDirAddr));

	}

	// Flush page directory
	void pagingFlushPD() {

		// Get current value of CR3
		quad_t cr3 = outCR3();
		// Rewrite it again to CR3
		inCR3(cr3);

	}


	// Enable paging
	void pagingEnable() {

		// Get current value of CR0
		quad_t cr0 = outCR0();
		// Set bit 31 to "1"
		cr0 |= (1 << 31);
		// Rewrite CR0 with new value
		inCR0(cr0);

	}


	// Get address which is caused Page Fault Exception
	quad_t pagingGetFaultAddres() {

		// Simply read CR2 value inside ISR
		return outCR2();

	}


	// Setup paging
	void pagingSetup() {

		// Other page directories are unused
		for (dword_t i = 0; i < 512; ++i) {

			// Pages marked as clear
			pageMapLevel4Table[i] = quad_t(pagingFlags_t::CLEAR);

		}

		// Map first 1 GB of physical RAM to first 1 GB of virtual RAM
		pageMapLevel4Table[0]	 = quad_t(pageDirectoryPointer);
		// Page marked as present and writable
		pageMapLevel4Table[0]	|= quad_t(pagingFlags_t::WRITABLE | pagingFlags_t::PRESENT);

		// Map first 1 GB of physical RAM to first 1 GB of virtual RAM
		pageDirectoryPointer[0]	 = quad_t(pageDirectory);
		// Page marked as present and writable
		pageDirectoryPointer[0]	|= quad_t(pagingFlags_t::WRITABLE | pagingFlags_t::PRESENT);

		// Map all pages of first 1 GB to first page table
		// Note 2 MB pages!
		for (sdword_t j = 0; j < 512; ++j) {

			pageDirectory[j]	 = (j << 21);
			// Page marked as present and writable
			pageDirectory[j]	|= quad_t(pagingFlags_t::WRITABLE | pagingFlags_t::PRESENT | pagingFlags_t::HUGE);

		}

		// Also map first 4MB of physical RAM to first 4MB after 3GB in virtual memory
		// (This should be useful for higher-half kernel)
		//pageDirectory[768]	= reinterpret_cast<quad_t>(pageTable);
		//pagingSetPDFlags(&pageDirectory[768],	pagingFlags_t::PAGE_WRITABLE | pagingFlags_t::PAGE_PRESENT);

		// Map page table itself to the last page of virtual memory
		//pageMapLevel4Table[511]	= reinterpret_cast<quad_t>(pageTable);
		//pagingSetPDFlags(&pageMapLevel4Table[511],	pagingFlags_t::PAGE_WRITABLE | pagingFlags_t::PAGE_PRESENT);

		// Install exception handler for page fault
		exHandlerInstall(PAGE_FAULT, pagingFaultExceptionHandler);

		// Setup page directory
		pagingSetupPD(pageMapLevel4Table);

		// 2 MB paging
		// Get current value of CR4
		quad_t cr4 = outCR4();
		// Set bit 5 to "1"
		cr4 |= (1 << 5);
		// Rewrite CR4 with new value
		inCR4(cr4);

		// Long mode paging
		asm volatile("movl $0xC0000080, %ecx\n"
			     "rdmsr\n"
			     "orl 1<<8, %eax\n"
			     "wrmsr\n");

		// Enable paging
		pagingEnable();

	}


	// Convert virtual address to physical address
	pointer_t pagingVirtToPhys(const pointer_t virtAddr) {

		// Page map level 4 table table index from virtual address
		quad_t pml4EntryIndex	= (quad_t(virtAddr) & 0xFF8000000000) >> 39;
		// Page directory pointer table index from virtual address
		quad_t pdpEntryIndex	= (quad_t(virtAddr) & 0x7FC0000000) >> 30;
		// Page directory table entry index from virtual address
		quad_t pdEntryIndex	= (quad_t(virtAddr) & 0x3FE00000) >> 21;
		// Page table entry index from virtual address
		//quad_t ptEntryIndex	= (quad_t(virtAddr) & 0x1FF000) >> 12;

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
		klib::kitoa(text, 64, static_cast<dword_t>(pagingGetFaultAddres()), klib::base::HEX);
		vmemWrite(text);
		vmemWrite("\r\nWHICH IS:\tNON-");
		vmemWrite(((regs->param & 0x01) == 0) ? "PRESENT\r\n" : "PRIVILEGED\r\n");
		vmemWrite("\r\n");

		// Hang here
		while (true) {};

	}


}	// namespace arch

