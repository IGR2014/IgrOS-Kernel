#ifndef GDT_H
#define GDT_H


#define GDT_LIMIT_MASK_HIGH	0x0F0000
#define GDT_LIMIT_MASK_LOW	0x00FFFF

#define GDT_BASE_MASK_HIGH	0xFF000000
#define GDT_BASE_MASK_LOW	0x00FFFFFF

#define GDT_FLAGS_MASK_HIGH	0x0F00
#define GDT_FLAGS_MASK_LOW	0x00FF


#include <include/types.hpp>


namespace arch {

	// Create GDT entry
	t_u64 gdtCreateEntry(t_u32, t_u32, t_u16);

	// Set entry to GDT table
	void gdtSetEntry(t_u16);

	// Get entry data from GDT
	void gdtGetEntry(t_u16);

	// Set GDT
	extern "C" void gdtLoad(t_u32);

}	// arch

#endif	// GDT_H

