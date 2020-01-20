////////////////////////////////////////////////////////////////
//
//	Global descriptor table low-level operations
//
//	File:	gdt.cpp
//	Date:	20 Jan 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#include <arch/gdt.hpp>


// Arch-dependent code zone
namespace arch {


	// Global descriptors table (GDT)
	gdtEntry_t	gdt::table[GDT_SIZE];
	// Pointer to GDT
	gdtPointer_t	gdt::pointer;


}	// namespace arch

