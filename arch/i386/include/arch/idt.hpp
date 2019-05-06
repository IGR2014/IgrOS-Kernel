////////////////////////////////////////////////////////////////
//
//	Interrupt descriptor table low-level operations
//
//	File:	idt.hpp
//	Date:	06 May 2019
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
//	All rights reserved.
//
//


#ifndef IGROS_ARCH_IDT_HPP
#define IGROS_ARCH_IDT_HPP


#include <arch/types.hpp>


// Arch-dependent code zone
namespace arch {


	// Number of IDT entries
	static const dword_t	IDT_SIZE		= 256;


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

#pragma pack(pop)


	// Create IDT entry
	idtEntry_t	idtSetEntry(const dword_t&, const word_t&, const byte_t&);


#ifdef	__cplusplus

	extern "C" {

#endif	// __cplusplus


		// Load IDT
		void	idtLoad(const idtPointer_t*);


#ifdef	__cplusplus

	}

#endif	// __cplusplus


	// Calculate IDT size
	dword_t		idtCalcTableSize(const dword_t&);

	// Setup IDT
	void		idtSetup();


}	// namespace arch


#endif	// IGROS_ARCH_IDT_HPP

