////////////////////////////////////////////////////////////////
//
//	Global descriptor table low-level operations
//
//	File:	boot.cpp
//	Date:	13 Feb 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#include <gdt.hpp>


// Arch-dependent code zone
namespace arch {


	// Global descriptors table (GDT)
	gdtEntry_t	gdt::table[GDT_SIZE];
	// Pointer to GDT
	gdtPointer_t	gdt::pointer;


}	// namespace arch

