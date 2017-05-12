#ifndef IDT_H
#define IDT_H


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

	// IDT table pointer
	struct idtPointer {

		t_u16		size;			// IDT size
		idtEntry*	pointer;		// IDT pointer
	
	};
#pragma pop

	// Create IDT entry
	idtEntry idtSetEntry(const t_u32&, const t_u16&, const t_u8&);

	// Load IDT
	extern "C" void idtLoad(const idtPointer*);

}	// arch

#endif	// IDT_H

