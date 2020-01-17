////////////////////////////////////////////////////////////////
//
//	Global descriptor table low-level operations
//
//	File:	gdt.hpp
//	Date:	18 Sep 2019
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#pragma once
#ifndef IGROS_ARCH_GDT_HPP
#define IGROS_ARCH_GDT_HPP


#include <flags.hpp>

#include <arch/types.hpp>


// Arch-dependent code zone
namespace arch {


	// GDT flags enum
	enum gdtFlags_t : word_t {

		// GDT empty segment
		GDT_SEG_EMPTY		= 0x0000,

		// GDT access page
		GDT_SEG_TYPE_ACCESS	= 0x0001,

		// GDT page types
		GDT_SEG_TYPE_RODATA	= GDT_SEG_EMPTY,
		GDT_SEG_TYPE_DATA	= 0x0002,
		GDT_SEG_TYPE_ROSTACK	= 0x0004,
		GDT_SEG_TYPE_STACK	= 0x0006,
		GDT_SEG_TYPE_EXCODE	= 0x0008,
		GDT_SEG_TYPE_CODE	= 0x000A,
		GDT_SEG_TYPE_EXCONF	= 0x000C,
		GDT_SEG_TYPE_CONF	= 0x000E,

		// GDT rings
		GDT_SEG_RING0		= 0x0010,
		GDT_SEG_RING1		= 0x0030,
		GDT_SEG_RING2		= 0x0050,
		GDT_SEG_RING3		= 0x0070,

		// GDT use ring number
		GDT_SEG_RING_USE	= 0x0080,

		// GDT types
		GDT_SEG_L		= 0x0200,
		GDT_SEG_SZ		= 0x0400,
		GDT_SEG_G		= 0x0800,

		// GDT entries
		GDT_SEG_RING0_CODE	= GDT_SEG_G		|
					  GDT_SEG_L		|
					  GDT_SEG_RING_USE	|
					  GDT_SEG_RING0		|
					  GDT_SEG_TYPE_CODE,

		GDT_SEG_RING0_DATA	= GDT_SEG_G		|
					  GDT_SEG_L		|
					  GDT_SEG_RING_USE	|
					  GDT_SEG_RING0		|
					  GDT_SEG_TYPE_DATA,

		GDT_SEG_RING3_CODE	= GDT_SEG_G		|
					  GDT_SEG_L		|
					  GDT_SEG_RING_USE	|
					  GDT_SEG_RING3		|
					  GDT_SEG_TYPE_CODE,

		GDT_SEG_RING3_DATA	= GDT_SEG_G		|
					  GDT_SEG_L		|
					  GDT_SEG_RING_USE	|
					  GDT_SEG_RING3		|
					  GDT_SEG_TYPE_DATA,

	};


	// Empty GDT entry (the first one)
	static const gdtFlags_t	GDT_ENTRY_EMPTY		= gdtFlags_t::GDT_SEG_EMPTY;

	// Kernel code page (4 Mb page, ring 0, code)
	static const gdtFlags_t	GDT_ENTRY_CODE_RING0	= gdtFlags_t::GDT_SEG_RING0_CODE;

	// Kernel data page (4 Mb page, ring 0, data)
	static const gdtFlags_t	GDT_ENTRY_DATA_RING0	= gdtFlags_t::GDT_SEG_RING0_DATA;

	// User code page (4 Mb page, ring 3, code)
	static const gdtFlags_t	GDT_ENTRY_CODE_RING3	= gdtFlags_t::GDT_SEG_RING3_CODE;

	// User data page (4 Mb page, ring 3, data)
	static const gdtFlags_t	GDT_ENTRY_DATA_RING3	= gdtFlags_t::GDT_SEG_RING3_DATA;


	// Number of GDT entries
	static const dword_t	GDT_SIZE		= 5;


#pragma pack(push, 1)

	// GDT entry
	struct gdtEntry_t {

		word_t		limitLow;
		word_t		baseLow;
		byte_t		baseMid;
		byte_t		access;
		byte_t		limitFlags;
		byte_t		baseHigh;

	};

	// GDT pointer
	struct gdtPointer_t {

		word_t		size;			// GDT size
		gdtEntry_t*	pointer;		// GDT pointer
	
	};

#pragma pack(pop)


	// Set GDT entry
	gdtEntry_t	gdtSetEntry(const dword_t&, const dword_t&, const gdtFlags_t&);


#ifdef	__cplusplus

	extern "C" {

#endif	// __cplusplus


		// Load GDT
		void	gdtLoad(const gdtPointer_t*);


#ifdef	__cplusplus

	}

#endif	// __cplusplus


	// Calculate GDT size
	constexpr dword_t	gdtCalcTableSize(const dword_t&);

	// Setup GDT
	void			gdtSetup();


}	// namespace arch


#endif	// IGROS_ARCH_GDT_HPP

