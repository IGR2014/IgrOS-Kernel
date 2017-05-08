#ifndef GDT_H
#define GDT_H


#define	GDT_SEG_TYPE_ACCESS	0x01

#define	GDT_SEG_TYPE_RODATA	0x00
#define	GDT_SEG_TYPE_DATA	0x02
#define	GDT_SEG_TYPE_ROSTACK	0x04
#define	GDT_SEG_TYPE_STACK	0x06
#define	GDT_SEG_TYPE_EXCODE	0x08
#define	GDT_SEG_TYPE_CODE	0x0A
#define	GDT_SEG_TYPE_EXCONF	0x0C
#define	GDT_SEG_TYPE_CONF	0x0E

#define GDT_SEG_RING0		0x0010
#define GDT_SEG_RING1		0x0030
#define GDT_SEG_RING2		0x0050
#define GDT_SEG_RING3		0x0070

#define	GDT_SEG_RING_USE	0x0080

#define	GDT_SEG_L		0x0200
#define	GDT_SEG_SZ		0x0400
#define	GDT_SEG_G		0x0800


#define	GDT_CODE_RING0		(GDT_SEG_G |\
				GDT_SEG_L | \
				GDT_SEG_RING_USE | \
				GDT_SEG_RING0 | \
				GDT_SEG_TYPE_CODE)

#define	GDT_DATA_RING0		(GDT_SEG_G |\
				GDT_SEG_L | \
				GDT_SEG_RING_USE | \
				GDT_SEG_RING0 | \
				GDT_SEG_TYPE_DATA)

#define	GDT_CODE_RING3		(GDT_SEG_G |\
				GDT_SEG_L | \
				GDT_SEG_RING_USE | \
				GDT_SEG_RING3 | \
				GDT_SEG_TYPE_CODE)

#define	GDT_DATA_RING3		(GDT_SEG_G |\
				GDT_SEG_L | \
				GDT_SEG_RING_USE | \
				GDT_SEG_RING3 | \
				GDT_SEG_TYPE_DATA)


#include <include/types.hpp>


// Arch-dependent code zone
namespace arch {

#pragma pack(push, 1)
	struct gdtEntry {

		t_u16		limitLow;
		t_u16		baseLow;
		t_u8		baseMid;
		t_u8		access;
		t_u8		limitFlags;
		t_u8		baseHigh;

	};

	struct gdtPointer {

		t_u16		size;
		gdtEntry*	pointer;
	
	};
#pragma pop

	// Set GDT entry
	gdtEntry gdtSetEntry(const t_u32&, const t_u32&, const t_u16&);

	// Load GDT
	extern "C" void gdtLoad(const gdtPointer*);

}	// arch

#endif	// GDT_H

