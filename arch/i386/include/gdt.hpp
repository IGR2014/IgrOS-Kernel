////////////////////////////////////////////////////////////////
//
//	Global descriptor table low-level operations
//
//	File:	gdt.hpp
//	Date:	23 Jun. 2018
//
//	Copyright (c) 2018, Igor Baklykov
//	All rights reserved.
//


#ifndef IGROS_ARCH_GDT_HPP
#define IGROS_ARCH_GDT_HPP


// GDT access page
#define	GDT_SEG_TYPE_ACCESS	0x0001

// GDT page types
#define	GDT_SEG_TYPE_RODATA	0x0000
#define	GDT_SEG_TYPE_DATA	0x0002
#define	GDT_SEG_TYPE_ROSTACK	0x0004
#define	GDT_SEG_TYPE_STACK	0x0006
#define	GDT_SEG_TYPE_EXCODE	0x0008
#define	GDT_SEG_TYPE_CODE	0x000A
#define	GDT_SEG_TYPE_EXCONF	0x000C
#define	GDT_SEG_TYPE_CONF	0x000E

// GDT rings
#define GDT_SEG_RING0		0x0010
#define GDT_SEG_RING1		0x0030
#define GDT_SEG_RING2		0x0050
#define GDT_SEG_RING3		0x0070

// GDT use ring number
#define	GDT_SEG_RING_USE	0x0080

// GDT types
#define	GDT_SEG_L		0x0200
#define	GDT_SEG_SZ		0x0400
#define	GDT_SEG_G		0x0800


// Kernel code page
#define	GDT_CODE_RING0		(GDT_SEG_G | \
				 GDT_SEG_SZ | \
				 GDT_SEG_RING_USE | \
				 GDT_SEG_RING0 | \
				 GDT_SEG_TYPE_CODE)

// Kernel data page
#define	GDT_DATA_RING0		(GDT_SEG_G | \
				 GDT_SEG_SZ | \
				 GDT_SEG_RING_USE | \
				 GDT_SEG_RING0 | \
				 GDT_SEG_TYPE_DATA)

// User code page
#define	GDT_CODE_RING3		(GDT_SEG_G | \
				 GDT_SEG_SZ | \
				 GDT_SEG_RING_USE | \
				 GDT_SEG_RING3 | \
				 GDT_SEG_TYPE_CODE)

// User data page
#define	GDT_DATA_RING3		(GDT_SEG_G | \
				 GDT_SEG_SZ | \
				 GDT_SEG_RING_USE | \
				 GDT_SEG_RING3 | \
				 GDT_SEG_TYPE_DATA)

// Number of GDT entries
#define GDT_SIZE		5


#include <include/types.hpp>


// Arch-dependent code zone
namespace arch {


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

#pragma pop


	// Set GDT entry
	gdtEntry_t		gdtSetEntry(const dword_t&, const dword_t&, const word_t&);

	// Load GDT
	extern "C" void		gdtLoad(const gdtPointer_t*);

	// Calculate GDT size
	dword_t			gdtCalcTableSize(const dword_t&);

	// Setup GDT
	void			gdtSetup();


}	// namespace arch


#endif	// IGROS_ARCH_GDT_HPP

