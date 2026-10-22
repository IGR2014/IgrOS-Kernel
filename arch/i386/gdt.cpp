////////////////////////////////////////////////////////////////
//
//	Global descriptor table low-level operations
//
//	File:	gdt.cpp
//	Date:	19 Dec 2023
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


// C++
#include <bit>
// IgrOS-Kernel arch i386
#include <arch/i386/gdt.hpp>
#include <arch/i386/tss.hpp>


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus


	// Reset segments to apply new GDT
	void	gdtResetSegments() noexcept;

	// Load GDT
	void	gdtLoad(const igros::i386::gdt::pointer_t* const gdtPtr) noexcept;
	// Store GDT
	void	gdtStore(igros::i386::gdt::pointer_t* const gdtPtr) noexcept;

	// Load task register
	void	tssLoad(const igros::igros_word_t selector) noexcept;
	

#ifdef	__cplusplus

}

#endif	// __cplusplus


// i386 namespace
namespace igros::i386 {


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
			// Task state segments (filled below: bases are known at run time only)
			gdt::setEntry<0x00000000, 0x00000000, GDT_ENTRY_EMPTY>(),
			gdt::setEntry<0x00000000, 0x00000000, GDT_ENTRY_EMPTY>(),
		};

		// Pointer to GDT
		constinit static gdt::pointer_t pointer {
			gdt::calcSize(table),
			table.data()
		};

		// Task state segments: kernel task and double fault task
		tss::init(KERNEL_CODE, KERNEL_DATA);
		const auto entry = [](const tss::segment_t &segment) noexcept {
			return gdt::tssEntry(static_cast<igros_dword_t>(std::bit_cast<igros_usize_t>(&segment)), static_cast<igros_dword_t>(sizeof(segment) - 1_usize));
		};
		table[TSS / sizeof(entry_t)]			= entry(tss::kernel());
		table[TSS_DOUBLE_FAULT / sizeof(entry_t)]	= entry(tss::doubleFault());

		// Load new GDT
		gdt::setPointer(pointer);
		// Load task register (CPU marks TSS descriptor busy: GDT must stay writable)
		::tssLoad(TSS);

	}


}	// namespace igros::i386

