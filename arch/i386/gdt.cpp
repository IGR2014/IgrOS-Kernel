////////////////////////////////////////////////////////////////
//
//	Global descriptor table low-level operations
//
//	File:	boot.cpp
//	Date:	11 Jul 2020
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#include <arch/i386/gdt.hpp>


// i386 namespace
namespace igros::i386 {


	// Global descriptors table (GDT)
	gdtEntry_t	gdt::table[GDT_SIZE];
	// Pointer to GDT
	gdtPointer_t	gdt::pointer;


}	// namespace igros::i386

