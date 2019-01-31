////////////////////////////////////////////////////////////////
//
//	Global descriptor table low-level operations
//
//	File:	boot.cpp
//	Date:	13 Aug. 2018
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
//	All rights reserved.
//
//


#include <include/gdt.hpp>
#include <include/vgaConsole.hpp>


// Arch-dependent code zone
namespace arch {


	// Global descriptors table (GDT)
	static gdtEntry_t	gdtTable[GDT_SIZE];
	// Pointer to GDT
	static gdtPointer_t	gdt;


	// Create GDT entry
	gdtEntry_t gdtSetEntry(const dword_t &base, const dword_t &limit, const gdtFlags_t &flags) {

		gdtEntry_t entry;

		entry.limitLow		= limit & 0xFFFF;
		entry.baseLow		= base & 0xFFFF;
		entry.baseMid		= (base & 0xFF0000) >> 16;
		entry.access		= static_cast<byte_t>(flags & static_cast<gdtFlags_t>(0x00FF));
		entry.limitFlags	= ((limit & 0xF0000) >> 16) | (static_cast<word_t>(flags & static_cast<gdtFlags_t>(0x0F00)) >> 4);
		entry.baseHigh		= (base & 0xFF000000) >> 24;

		return entry;

	}

	// Calculate GDT size
	dword_t gdtCalcTableSize(const dword_t &numOfEntries) {

		return (numOfEntries * sizeof(gdtEntry_t)) - 1;

	}

	// Setup GDT
	void gdtSetup() {

		// GDT setup
		// Empty entry (should be there!)
		gdtTable[0x00]	= gdtSetEntry(0x00000000, 0x00000000, GDT_ENTRY_EMPTY);
		// Kernel code
		gdtTable[0x01]	= gdtSetEntry(0x00000000, 0xFFFFFFFF, GDT_ENTRY_CODE_RING0);
		// Kernel data
		gdtTable[0x02]	= gdtSetEntry(0x00000000, 0xFFFFFFFF, GDT_ENTRY_DATA_RING0);
		// User code
		gdtTable[0x03]	= gdtSetEntry(0x00000000, 0xFFFFFFFF, GDT_ENTRY_CODE_RING3);
		// User data
		gdtTable[0x04]	= gdtSetEntry(0x00000000, 0xFFFFFFFF, GDT_ENTRY_DATA_RING3);

		// Set GDT size and data pointer
		gdt.size	= gdtCalcTableSize(GDT_SIZE);
		gdt.pointer	= gdtTable;

		// Load new GDT
		gdtLoad(&gdt);
	
	}


}	// namespace arch

