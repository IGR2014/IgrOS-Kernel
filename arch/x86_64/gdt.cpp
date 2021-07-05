////////////////////////////////////////////////////////////////
//
//	Global descriptor table low-level operations
//
//	File:	gdt.cpp
//	Date:	07 Feb 2021
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#include <arch/x86_64/gdt.hpp>


// x86_64 namespace
namespace igros::x86_64 {


	// Global descriptors table (GDT)
	std::array<gdtEntryx86_64_t, gdt::GDT_SIZE> gdt::table {
		gdt::setEntry(0x00000000, 0x00000000, GDT_ENTRY_EMPTY),		// Empty entry (should be there!)
		gdt::setEntry(0x00000000, 0xFFFFFFFF, GDT_ENTRY_CODE_RING0),	// Kernel code
		gdt::setEntry(0x00000000, 0xFFFFFFFF, GDT_ENTRY_DATA_RING0),	// Kernel data
		gdt::setEntry(0x00000000, 0xFFFFFFFF, GDT_ENTRY_CODE_RING3),	// User code
		gdt::setEntry(0x00000000, 0xFFFFFFFF, GDT_ENTRY_DATA_RING3)	// User data
	};

	// Pointer to GDT
	gdtPointerx86_64_t gdt::pointer {
		gdt::calcSize(),
		gdt::table.cbegin()
	};


}	// namespace igros::x86_^4

