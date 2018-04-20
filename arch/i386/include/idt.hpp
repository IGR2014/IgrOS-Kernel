////////////////////////////////////////////////////////////////
//
//	Interrupt descriptor table low-level operations
//
//	File:	idt.hpp
//	Date:	17 Apr. 2018
//
//	Copyright (c) 2018, Igor Baklykov
//	All rights reserved.
//


#ifndef IGROS_ARCH_IDT_HPP
#define IGROS_ARCH_IDT_HPP


#include <include/types.hpp>


// Arch-dependent code zone
namespace arch {


#pragma pack(push, 1)

	// IDT entry
	struct idtEntry {

		t_u16		offsetLow;		// Offset lower part (0..15)
		t_u16		selector;		// Selectod from GDT/LDT
		t_u8		reserved;		// Must be zero
		t_u8		type;			// Type attributes
		t_u16		offsetHigh;		// Offset higher part (16..31)
	
	};

	// IDT pointer
	struct idtPointer {

		t_u16		size;			// IDT size
		idtEntry*	pointer;		// IDT pointer
	
	};

#pragma pop


	// Create IDT entry
	idtEntry idtSetEntry(const t_u32&, const t_u16&, const t_u8&);

	// Load IDT
	extern "C" void idtLoad(const idtPointer*);

	// Calculate IDT size
	t_u32 idtCalcTableSize(const t_u32&);


}	// namespace arch


#endif	// IGROS_ARCH_IDT_HPP

