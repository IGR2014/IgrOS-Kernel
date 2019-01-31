////////////////////////////////////////////////////////////////
//
//	Memory paging for x86
//
//	File:	paging.cpp
//	Date:	01 Feb. 2019
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
//	All rights reserved.
//
//


#include <include/cr.hpp>
#include <include/paging.hpp>
#include <include/taskRegs.hpp>
#include <include/exceptions.hpp>
#include <include/vgaConsole.hpp>


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


	// Setup page directory
	void pagingSetupPD(const pointer_t pageDirAddr) {

		// Write page directory address to CR3
		inCR3(reinterpret_cast<dword_t>(pageDirAddr));

	}

	// Flush page directory
	void pagingFlushPD() {

		// Get current value of CR3
		dword_t cr3 = outCR3();
		// Rewrite it again to CR3
		inCR3(cr3);

	}


	// Enable paging
	void pagingEnable() {

		// Get current value of CR0
		dword_t cr0 = outCR0();
		// Set bit 31 to "1"
		cr0 |= (1 << 31);
		// Rewrite CR0 with new value
		inCR0(cr0);

	}


	// Get address which is caused Page Fault Exception
	dword_t pagingGetFaultAddres() {

		// Simply read CR2 value inside ISR
		return outCR2();

	}


	// Set page directory flags
	void pagingSetPDFlags(pointer_t pageDirEntry, const pagingFlags_t flags) {

		// Page directory dirty flag does not exists
		// It's reserved as 0
		*static_cast<pagingFlags_t*>(pageDirEntry) = static_cast<pagingFlags_t>(*static_cast<pagingFlags_t*>(pageDirEntry) | (flags & ~pagingFlags_t::PAGE_DIRTY));
	
	}

	// Set page table flags
	void pagingSetPTFlags(pointer_t pageTableEntry, const pagingFlags_t flags) {

		// Page directory size flag does not exists
		// It's reserved as 0
		*static_cast<pagingFlags_t*>(pageTableEntry) = static_cast<pagingFlags_t>(*static_cast<pagingFlags_t*>(pageTableEntry) | (flags & ~pagingFlags_t::PAGE_SIZE));

	}


	// Setup paging
	void pagingSetup() {

		// Other page directories are unused
		for (dword_t i = 0; i < 1024; ++i) {

			pageDirectory[i] = 0x00000000;
			pagingSetPDFlags(&pageDirectory[i], pagingFlags_t::PAGE_CLEAR);

		}

		// Map first 4 MB of physical RAM to first 4 MB of virtual RAM
		pageDirectory[0]	= reinterpret_cast<dword_t>(pageTable);
		pagingSetPDFlags(&pageDirectory[0],	static_cast<pagingFlags_t>(pagingFlags_t::PAGE_WRITABLE | pagingFlags_t::PAGE_PRESENT));

		// Also map first 4MB of physical RAM to first 4MB after 3GB in virtual memory
		// (This should be useful for higher-half kernel)
		pageDirectory[768]	= reinterpret_cast<dword_t>(pageTable);
		pagingSetPDFlags(&pageDirectory[768],	static_cast<pagingFlags_t>(pagingFlags_t::PAGE_WRITABLE | pagingFlags_t::PAGE_PRESENT));

		// Map page table itself to the last page of virtual memory
		pageDirectory[1023]	= reinterpret_cast<dword_t>(pageDirectory);
		pagingSetPDFlags(&pageDirectory[1023],	static_cast<pagingFlags_t>(pagingFlags_t::PAGE_WRITABLE | pagingFlags_t::PAGE_PRESENT));

		// Map all pages of first 4MB to first page table
		for (sdword_t j = 0; j < 1024; ++j) {

			pageTable[j]	= (j << 12);
			pagingSetPTFlags(&pageTable[j],	static_cast<pagingFlags_t>(pagingFlags_t::PAGE_WRITABLE | pagingFlags_t::PAGE_PRESENT));

		}

		// Setup page directory
		pagingSetupPD(pageDirectory);

		// Install exception handler for page fault
		exHandlerInstall(PAGE_FAULT, pagingFaultExceptionHandler);

		// Enable paging
		pagingEnable();

	}


	// Convert virtual address to physical address
	pointer_t pagingVirtToPhys(const pointer_t virtAddr) {

		// Page directory entry index from virtual address
		dword_t pdEntryIndex	= reinterpret_cast<dword_t>(virtAddr) >> 22;
		// Page table entry index from virtual address
		dword_t ptEntryIndex	= (reinterpret_cast<dword_t>(virtAddr) >> 12) & 0x3FF;

		// Physical pointer to page table
		dword_t pageTablePtr	= pageDirectory[pdEntryIndex];

		// Check if page table is present or not
		if ((static_cast<pagingFlags_t>(pageTablePtr) & pagingFlags_t::PAGE_PRESENT) != pagingFlags_t::PAGE_CLEAR) {

			// Physical pointer to page
			dword_t pagePtr = reinterpret_cast<dword_t*>(pageTablePtr & ~0xFFF)[ptEntryIndex];

			// Check if page is present or not
			if ((static_cast<pagingFlags_t>(pagePtr) & pagingFlags_t::PAGE_PRESENT) != pagingFlags_t::PAGE_CLEAR) {

				// Get physical address of page from page table (20 MSB)
				dword_t physPageAddr	= pagePtr & ~0xFFF;
				// Get physical offset from virtual address`s (12 LSB)
				dword_t physPageOffset	= reinterpret_cast<dword_t>(virtAddr) & 0xFFF;

				// Return physical address
				return reinterpret_cast<pointer_t>(physPageAddr | physPageOffset);

			}

		}

		// Page or table is not present
		return nullptr;

	}


	// Page Fault Exception handler
	void pagingFaultExceptionHandler(const taskRegs_t* regs) {

		vgaConsoleWrite("CAUSED BY:\t");
		((regs->param & 0x08) == 0) ? vgaConsoleWrite("") : vgaConsoleWrite("RESERVED BIT SET");
		((regs->param & 0x10) == 0) ? vgaConsoleWrite("") : vgaConsoleWrite("INSTRUCTION FETCH");
		vgaConsoleWriteLine("");
		vgaConsoleWrite("FROM:\t\t");
		((regs->param & 0x04) == 0) ? vgaConsoleWrite("KERNEL") : vgaConsoleWrite("USER");
		vgaConsoleWriteLine(" space");
		vgaConsoleWrite("WHEN:\t\tattempting to ");
		((regs->param & 0x02) == 0) ? vgaConsoleWrite("READ") : vgaConsoleWrite("WRITE");
		vgaConsoleWriteLine("");
		vgaConsoleWrite("ADDRESS:\t");
		vgaConsoleWriteHex(pagingGetFaultAddres());
		vgaConsoleWrite(" which is NON-");
		((regs->param & 0x01) == 0) ? vgaConsoleWrite("PRESENT") : vgaConsoleWrite("PRIVILEGED");
		vgaConsoleWriteLine("");

		// Hang here
		while (true) {};

	}


}	// namespace arch

