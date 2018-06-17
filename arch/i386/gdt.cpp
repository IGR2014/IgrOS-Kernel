////////////////////////////////////////////////////////////////
//
//	Global descriptor table low-level operations
//
//	File:	boot.cpp
//	Date:	18 Jun. 2018
//
//	Copyright (c) 2018, Igor Baklykov
//	All rights reserved.
//


#include <include/gdt.hpp>


// Arch-dependent code zone
namespace arch {


	// Global descriptors table (GDT)
	static gdtEntry		gdtTable[GDT_SIZE];
	// Pointer to GDT
	static gdtPointer	gdt;


	// Create GDT entry
	gdtEntry gdtSetEntry(const t_u32 &base, const t_u32 &limit, const t_u16 &flags) {

		gdtEntry entry;

		entry.limitLow		= limit & 0xFFFF;
		entry.baseLow		= base & 0xFFFF;
		entry.baseMid		= (base & 0xFF0000) >> 16;
		entry.access		= flags & 0xFF;
		entry.limitFlags	= ((limit & 0xF0000) >> 16) | ((flags & 0x0F00) >> 4);
		entry.baseHigh		= (base & 0xFF000000) >> 24;

		return entry;

	}

	// Calculate GDT size
	t_u32 gdtCalcTableSize(const t_u32 &numOfEntries) {

		return (numOfEntries * sizeof(gdtEntry)) - 1;

	}

	// Setup GDT
	void gdtSetup() {

		// GDT setup
		gdtTable[0x00]	= gdtSetEntry(0x00000000, 0x00000000, 0x0000);
		gdtTable[0x01]	= gdtSetEntry(0x00000000, 0xFFFFFFFF, GDT_CODE_RING0);
		gdtTable[0x02]	= gdtSetEntry(0x00000000, 0xFFFFFFFF, GDT_DATA_RING0);
		gdtTable[0x03]	= gdtSetEntry(0x00000000, 0xFFFFFFFF, GDT_CODE_RING3);
		gdtTable[0x04]	= gdtSetEntry(0x00000000, 0xFFFFFFFF, GDT_DATA_RING3);

		// Set GDT size and data pointer
		gdt.size	= gdtCalcTableSize(GDT_SIZE);
		gdt.pointer	= gdtTable;

		// Load new GDT
		gdtLoad(&gdt);
	
	}


}	// namespace arch

