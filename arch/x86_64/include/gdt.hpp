#ifndef GDT_H
#define GDT_H


#define GDT_LIMIT_MASK_HIGH	0x0F0000
#define GDT_LIMIT_MASK_LOW	0x00FFFF

#define GDT_BASE_MASK_HIGH	0xFF000000
#define GDT_BASE_MASK_LOW	0x00FFFFFF

#define GDT_FLAGS_MASK_HIGH	0x0F00
#define GDT_FLAGS_MASK_LOW	0x00FF


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
	struct gdtPtr {

		t_u16	size;
		t_u32	pointer;
	
	};
	#pragma pop

	// Create GDT entry
	t_u64 gdtCreateEntry(t_u32, t_u32, t_u16);

	// Set entry to GDT table
	//void gdtSetEntry(t_u16);

	// Get entry data from GDT
	//void gdtGetEntry(t_u16);

	// Set GDT
	extern "C" void gdtLoad(t_u32);

}	// arch

#endif	// GDT_H

