////////////////////////////////////////////////////////////////
//
//	Global descriptor table low-level operations
//
//	File:	gdt.cpp
//	Date:	22 Dec 2023
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


// IgrOS-Kernel arch x86_64
#include <arch/x86_64/gdt.hpp>


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus


	// Reset segments to apply new GDT
	void	gdtResetSegments() noexcept;

	// Load GDT
	void	gdtLoad(const igros::x86_64::gdt::pointer_t* gdtPtr) noexcept;
	// Store GDT
	auto	gdtStore() noexcept -> const igros::x86_64::gdt::pointer_t*;


#ifdef	__cplusplus

}

#endif	// __cplusplus


// x86_64 namespace
namespace igros::x86_64 {


	// Set GDT pointer
	void gdt::setPointer(const pointer_t &pointer) noexcept {
		// Load new GDT
		::gdtLoad(&pointer);
	}


	// Init GDT table
	void gdt::init() noexcept {

		// Global descriptors table (GDT)
		constinit static gdt::table_t table {
			// Empty entry (should be there!)
			gdt::setEntry<0x00000000, 0x00000000, GDT_ENTRY_EMPTY>(),
			// Kernel code
			gdt::setEntry<0x00000000, 0xFFFFFFFF, GDT_ENTRY_CODE_RING0>(),
			// Kernel data
			gdt::setEntry<0x00000000, 0xFFFFFFFF, GDT_ENTRY_DATA_RING0>(),
			// User code
			gdt::setEntry<0x00000000, 0xFFFFFFFF, GDT_ENTRY_CODE_RING3>(),
			// User data
			gdt::setEntry<0x00000000, 0xFFFFFFFF, GDT_ENTRY_DATA_RING3>(),
			// Empty entry (just in case)
			gdt::setEntry<0x00000000, 0x00000000, GDT_ENTRY_EMPTY>(),
		};

		// Pointer to GDT
		constinit static gdt::pointer_t pointer {
			gdt::calcSize(table),
			table.data()
		};

		// Load new GDT
		gdt::setPointer(pointer);

	}


}	// namespace igros::x86_^4

