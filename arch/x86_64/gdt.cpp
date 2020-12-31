////////////////////////////////////////////////////////////////
//
//	Global descriptor table low-level operations
//
//	File:	gdt.cpp
//	Date:	11 Jul 2020
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#include <gdt.hpp>


// x86_64 namespace
namespace igros::x86_64 {


	// Global descriptors table (GDT)
	gdtEntry_t	gdt::table[GDT_SIZE];
	// Pointer to GDT
	gdtPointer_t	gdt::pointer;


}	// namespace igros::x86_^4

