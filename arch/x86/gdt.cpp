////////////////////////////////////////////////////////////////
//
//	Global descriptor table low-level operations
//
//	File:	boot.cpp
//	Date:	17 Apr. 2018
//
//	Copyright (c) 2018, Igor Baklykov
//	All rights reserved.
//


#include <include/gdt.hpp>


// Arch-dependent code zone
namespace arch {


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

}	// namespace arch

