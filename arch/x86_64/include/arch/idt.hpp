////////////////////////////////////////////////////////////////
//
//	Interrupt descriptor table low-level operations
//
//	File:	idt.hpp
//	Date:	13 Jun 2019
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
//	All rights reserved.
//
//


#pragma once
#ifndef IGROS_ARCH_IDT_HPP
#define IGROS_ARCH_IDT_HPP


#include <type_traits>

#include <arch/types.hpp>
#include <arch/taskRegs.hpp>


// Arch-dependent code zone
namespace arch {


	// IDT ISR pointer
	using idtISRPtr_t = std::add_pointer<void()>::type;


	// Number of IDT entries
	static const dword_t	IDT_SIZE		= 256;


#pragma pack(push, 1)

	// IDT entry
	struct idtEntry_t {

		word_t		offsetLow;		// Offset lower part (0..15)
		word_t		selector;		// Selectod from GDT/LDT
		byte_t		ist;			// Interrupt service table
		byte_t		type;			// Type attributes
		word_t		offsetMiddle;		// Offset middle part (16..31)
		dword_t		offsetHigh;		// Offset higher part (32..63)
		dword_t		reserved2;		// Must be zero
	
	};

	// IDT pointer
	struct idtPointer_t {

		word_t		size;			// IDT size
		idtEntry_t*	pointer;		// IDT pointer
	
	};

#pragma pack(pop)


	// Create IDT entry
	constexpr idtEntry_t	idtSetEntry(const idtISRPtr_t, const word_t&, const byte_t&);


#ifdef	__cplusplus

	extern "C" {

#endif	// __cplusplus


		// Load IDT
		void	idtLoad(const idtPointer_t*);


#ifdef	__cplusplus

	}

#endif	// __cplusplus


	// Calculate IDT size
	constexpr dword_t	idtCalcTableSize(const dword_t&);

	// Setup IDT
	void			idtSetup();


}	// namespace arch


#endif	// IGROS_ARCH_IDT_HPP

