////////////////////////////////////////////////////////////////
//
//	Interrupt descriptor table low-level operations
//
//	File:	idt.cpp
//	Date:	17 Apr. 2018
//
//	Copyright (c) 2018, Igor Baklykov
//	All rights reserved.
//


#include <include/idt.hpp>


namespace arch {


	// Create IDT entry
	idtEntry idtSetEntry(const t_u32 &offset, const t_u16 &selector, const t_u8 &type) {

		idtEntry entry;	

		entry.offsetLow		= offset & 0xFFFF;
		entry.selector		= selector;
		entry.reserved		= 0x00;
		entry.type		= type;
		entry.offsetHigh	= (offset & 0xFFFF0000) >> 16;

		return entry;

	}

	// Calculate IDT size
	t_u32 idtCalcTableSize(const t_u32 &numOfEntries) {

		return (numOfEntries * sizeof(idtEntry)) - 1;

	}


}	// namespace arch

