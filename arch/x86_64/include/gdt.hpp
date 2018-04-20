////////////////////////////////////////////////////////////////
//
//	Global descriptor table low-level operations
//
//	File:	gdt.hpp
//	Date:	17 Apr. 2018
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
				 GDT_SEG_L | \
				 GDT_SEG_RING_USE | \
				 GDT_SEG_RING0 | \
				 GDT_SEG_TYPE_CODE)

// Kernel data page
#define	GDT_DATA_RING0		(GDT_SEG_G | \
				 GDT_SEG_L | \
				 GDT_SEG_RING_USE | \
				 GDT_SEG_RING0 | \
				 GDT_SEG_TYPE_DATA)

// User code page
#define	GDT_CODE_RING3		(GDT_SEG_G | \
				 GDT_SEG_L | \
				 GDT_SEG_RING_USE | \
				 GDT_SEG_RING3 | \
				 GDT_SEG_TYPE_CODE)

// User data page
#define	GDT_DATA_RING3		(GDT_SEG_G | \
				 GDT_SEG_L | \
				 GDT_SEG_RING_USE | \
				 GDT_SEG_RING3 | \
				 GDT_SEG_TYPE_DATA)


#include <include/types.hpp>


// Arch-dependent code zone
namespace arch {


#pragma pack(push, 1)

	// GDT entry
	struct gdtEntry {

		t_u16		limitLow;
		t_u16		baseLow;
		t_u8		baseMid;
		t_u8		access;
		t_u8		limitFlags;
		t_u8		baseHigh;

	};

	// GDT pointer
	struct gdtPointer {

		t_u16		size;			// GDT size
		gdtEntry*	pointer;		// GDT pointer
	
	};

#pragma pop


	// Set GDT entry
	gdtEntry gdtSetEntry(const t_u32&, const t_u32&, const t_u16&);

	extern "C" {

		// Load GDT
		void	gdtLoad(const gdtPointer*);

		// Setup page tables
		void	setupPageTables();
		// Enable paging
		void	enablePaging();

	}

	// Calculate GDT size
	t_u32 gdtCalcTableSize(const t_u32&);


}	// namespace arch


#endif	// IGROS_ARCH_GDT_HPP

