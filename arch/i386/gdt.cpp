////////////////////////////////////////////////////////////////
//
//	Global descriptor table low-level operations
//
//	File:	boot.cpp
//	Date:	07 Feb 2021
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#include <arch/i386/gdt.hpp>


// i386 namespace
namespace igros::i386 {


	// Global descriptors table (GDT)
	std::array<gdtEntryi386_t, gdti386::GDT_SIZE> gdti386::table {
		gdti386::setEntry(0x00000000, 0x00000000, GDT_ENTRY_EMPTY),		// Empty entry (should be there!)
		gdti386::setEntry(0x00000000, 0xFFFFFFFF, GDT_ENTRY_CODE_RING0),	// Kernel code
		gdti386::setEntry(0x00000000, 0xFFFFFFFF, GDT_ENTRY_DATA_RING0),	// Kernel data
		gdti386::setEntry(0x00000000, 0xFFFFFFFF, GDT_ENTRY_CODE_RING3),	// User code
		gdti386::setEntry(0x00000000, 0xFFFFFFFF, GDT_ENTRY_DATA_RING3)		// User data
	};

	// Pointer to GDT
	gdtPointeri386_t gdti386::pointer {
		gdti386::calcSize(),
		gdti386::table.cbegin()
	};


}	// namespace igros::i386

