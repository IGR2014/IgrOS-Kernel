////////////////////////////////////////////////////////////////
//
//	Long mode low-level preparation operations
//
//	File:	longMode.hpp
//	Date:	20 Nov. 2017
//
//	Copyright (c) 2017, Igor Baklykov
//	All rights reserved.
//


#ifndef IGROS_ARCH_LONG_MODE_HPP
#define IGROS_ARCH_LONG_MODE_HPP


#include <include/types.hpp>


// Arch-dependent code zone
namespace arch {


	// Set GDT
	extern "C" {

		// Check if loaded with multiboot loader
		t_u8	checkMultiboot();
		// Check CPUID is extended
		t_u8	checkCPUID();
		// Check CPUID for long mode existance
		t_u8	checkLongMode();

		// Setup page tables
		void	setupPageTables();
		// Enable paging
		void	enablePaging();

	}


}	// namespace arch


#endif	// IGROS_ARCH_LONG_MODE_HPP

