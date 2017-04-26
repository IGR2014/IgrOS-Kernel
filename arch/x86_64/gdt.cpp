//#include <include/types.hpp>
#include <include/gdt.hpp>


// Arch-dependent code zone
namespace arch {


	// Create GDT entry
	t_u64 gdtCreateEntry(t_u32 base, t_u32 limit, t_u16 flags) {

		t_u64 baseData	= 0L;	// Base address
		t_u64 limitData	= 0L;	// Page limit (size)
		t_u64 flagsData	= 0L;	// GDT flags

		// Fill page limit fields
		limitData = limit & GDT_LIMIT_MASK_HIGH;
		limitData = (limitData << 32) | (limit & GDT_LIMIT_MASK_LOW);

		// Fill base address fields
		baseData = base & GDT_BASE_MASK_HIGH;
		baseData = (baseData << 16) | (base & GDT_BASE_MASK_LOW);

		// Fill GDT flags fields
		flagsData = flags & GDT_FLAGS_MASK_HIGH;
		flagsData = (flagsData << 4) | (flags & GDT_FLAGS_MASK_LOW);

		return (flagsData << 40) | (baseData << 16) | limitData;

	}

	/*
	// Set entry to GDT table
	void gdtSetEntry(t_u16 position) {

		//

	}

	// Get entry data from GDT
	void gdtGetEntry(t_u16) {

		//

	}
	*/

}	// namespace arch

