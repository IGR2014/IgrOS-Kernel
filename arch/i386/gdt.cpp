////////////////////////////////////////////////////////////////
//
//	Global descriptor table low-level operations
//
//	File:	boot.cpp
//	Date:	08 Oct 2019
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
//	All rights reserved.
//
//


#include <arch/gdt.hpp>


// Arch-dependent code zone
namespace arch {


	// Global descriptors table (GDT)
	static gdtEntry_t	gdtTable[GDT_SIZE];
	// Pointer to GDT
	static gdtPointer_t	gdt;


	// Create GDT entry
	gdtEntry_t gdtSetEntry(const dword_t &base, const dword_t &limit, const gdtFlags_t &flags) {

		return	gdtEntry_t {

				.limitLow	= word_t(limit & 0xFFFF),
				.baseLow	= word_t(base & 0xFFFF),
				.baseMid	= byte_t((base & 0xFF0000) >> 16),
				.access		= byte_t(flags & 0x00FF),
				.limitFlags	= byte_t(((limit & 0xF0000) >> 16) | (word_t(flags & 0x0F00) >> 4)),
				.baseHigh	= byte_t((base & 0xFF000000) >> 24)

			};

	}

	// Calculate GDT size
	constexpr dword_t gdtCalcTableSize(const dword_t &numOfEntries) {
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

