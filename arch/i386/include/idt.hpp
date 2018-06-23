////////////////////////////////////////////////////////////////
//
//	Interrupt descriptor table low-level operations
//
//	File:	idt.hpp
//	Date:	23 Jun. 2018
//
//	Copyright (c) 2018, Igor Baklykov
//	All rights reserved.
//


#ifndef IGROS_ARCH_IDT_HPP
#define IGROS_ARCH_IDT_HPP


// Number of IDT entries
#define IDT_SIZE		256


#include <include/types.hpp>


// Arch-dependent code zone
namespace arch {


#pragma pack(push, 1)

	// IDT entry
	struct idtEntry_t {

		word_t		offsetLow;		// Offset lower part (0..15)
		word_t		selector;		// Selectod from GDT/LDT
		byte_t		reserved;		// Must be zero
		byte_t		type;			// Type attributes
		word_t		offsetHigh;		// Offset higher part (16..31)
	
	};

	// IDT pointer
	struct idtPointer_t {

		word_t		size;			// IDT size
		idtEntry_t*	pointer;		// IDT pointer
	
	};

#pragma pop


	// Create IDT entry
	idtEntry_t		idtSetEntry(const dword_t&, const word_t&, const byte_t&);

	// Load IDT
	extern "C" void		idtLoad(const idtPointer_t*);

	// Calculate IDT size
	dword_t			idtCalcTableSize(const dword_t&);

	// Setup IDT
	void			idtSetup();


}	// namespace arch


#endif	// IGROS_ARCH_IDT_HPP

