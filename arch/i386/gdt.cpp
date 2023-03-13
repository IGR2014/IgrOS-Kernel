////////////////////////////////////////////////////////////////
//
//	Global descriptor table low-level operations
//
//	File:	boot.cpp
//	Date:	13 Mar 2023
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


// IgrOS-Kernel arch i386
#include <arch/i386/gdt.hpp>


// i386 namespace
namespace igros::i386 {


	// Global descriptors table (GDT)
	constinit std::array<gdtEntry_t, gdt::GDT_SIZE> gdt::table {
		gdt::setEntry<0x00000000, 0x00000000, GDT_ENTRY_EMPTY>(),		// Empty entry (should be there!)
		gdt::setEntry<0x00000000, 0xFFFFFFFF, GDT_ENTRY_CODE_RING0>(),		// Kernel code
		gdt::setEntry<0x00000000, 0xFFFFFFFF, GDT_ENTRY_DATA_RING0>(),		// Kernel data
		gdt::setEntry<0x00000000, 0xFFFFFFFF, GDT_ENTRY_CODE_RING3>(),		// User code
		gdt::setEntry<0x00000000, 0xFFFFFFFF, GDT_ENTRY_DATA_RING3>()		// User data
	};

	// Pointer to GDT
	constinit gdtPointer_t gdt::pointer {
		gdt::calcSize(),
		gdt::table.cbegin()
	};


}	// namespace igros::i386

