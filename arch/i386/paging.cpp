////////////////////////////////////////////////////////////////
//
//	Paging operations
//
//	File:	paging.cpp
//	Date:	18 Jun. 2018
//
//	Copyright (c) 2018, Igor Baklykov
//	All rights reserved.
//


#include <include/paging.hpp>


// Arch-dependent code zone
namespace arch {


	// Workaround becouse of unimplemented yet physical memory allocator
	extern "C" {


		// Page directory
		extern t_u32p	pageDirectory;
		// Page table
		extern t_u32p	pageTable;


	}	// extern "C"


	// Setup paging
	void pagingSetup() {

		// Map first 4 MB of physical RAM to first 4 MB of virtual RAM
		pageDirectory[0]	= reinterpret_cast<t_u32>(pageTable) | 3;

		// Other page directories are unused
		for (unsigned int i = 1; i < 1024; ++i) {

			pageDirectory[i] = 0x00000000;

		}

		// Also map first 4MB of physical RAM to first 4MB after 3GB in virtual memory
		// (This shoul be useful for higher-half kernel)
		pageDirectory[768]	= reinterpret_cast<t_u32>(pageTable) | 3;
		// Map page table itself to the last page of virtual memory
		pageDirectory[1023]	= reinterpret_cast<t_u32>(pageDirectory);

		// Map all pages of first 4MB to first page table
		for (unsigned int j = 0; j < 1024; ++j) {

			pageTable[j] = (j * 0x1000) | 3;

		}

		// Setup page directory
		arch::pagingSetupPD(pageDirectory);
		// Enable paging
		arch::pagingEnable();

	}


}	// namespace arch

