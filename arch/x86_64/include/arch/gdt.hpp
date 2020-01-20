////////////////////////////////////////////////////////////////
//
//	Global descriptor table low-level operations
//
//	File:	gdt.hpp
//	Date:	20 Jan 2020
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


	// GDT structure
	class gdt final {

		// GDT flags enum
		enum FLAGS : word_t {
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
		constexpr static FLAGS GDT_ENTRY_EMPTY		= FLAGS::GDT_SEG_EMPTY;
		// Kernel code page (4 Mb page, ring 0, code)
		constexpr static FLAGS GDT_ENTRY_CODE_RING0	= FLAGS::GDT_SEG_RING0_CODE;
		// Kernel data page (4 Mb page, ring 0, data)
		constexpr static FLAGS GDT_ENTRY_DATA_RING0	= FLAGS::GDT_SEG_RING0_DATA;
		// User code page (4 Mb page, ring 3, code)
		constexpr static FLAGS GDT_ENTRY_CODE_RING3	= FLAGS::GDT_SEG_RING3_CODE;
		// User data page (4 Mb page, ring 3, data)
		constexpr static FLAGS GDT_ENTRY_DATA_RING3	= FLAGS::GDT_SEG_RING3_DATA;

		// Number of GDT entries
		constexpr static dword_t GDT_SIZE		= 5u;

		// Global descriptors table (GDT)
		static gdtEntry_t	table[GDT_SIZE];
		// Pointer to GDT
		static gdtPointer_t	pointer;


	public:

		// Default c-tor
		gdt() noexcept = default;

		// Copy c-tor
		gdt(const gdt &other) = delete;
		// Copy assignment
		gdt& operator=(const gdt &other) = delete;

		// Move c-tor
		gdt(gdt &&other) = delete;
		// Move assignment
		gdt& operator=(gdt &&other) = delete;

		// Set GDT entry
		constexpr static void	setEntry(const byte_t ID, const dword_t base, const dword_t &limit, const FLAGS flags) noexcept;
		// Calc GDT size
		[[nodiscard]] constexpr static dword_t	calcSize() noexcept;

		// Init GDT table
		constexpr static void	init() noexcept;


	};


	// Set GDT entry
	constexpr void gdt::setEntry(const byte_t ID, const dword_t base, const dword_t &limit, const FLAGS flags) noexcept {
		table[ID] = {
			.limitLow	= word_t(limit & 0xFFFF),
			.baseLow	= word_t(base & 0xFFFF),
			.baseMid	= byte_t((base & 0xFF0000) >> 16),
			.access		= byte_t(flags & 0x00FF),
			.limitFlags	= byte_t(((limit & 0xF0000) >> 16) | (word_t(flags & 0x0F00) >> 4)),
			.baseHigh	= byte_t((base & 0xFF000000) >> 24)
		};
	}

	// Calculate GDT size
	constexpr dword_t gdt::calcSize() noexcept {
		return (GDT_SIZE * sizeof(gdtEntry_t)) - 1u;
	}


#ifdef	__cplusplus

	extern "C" {

#endif	// __cplusplus


		// Reset segments to apply new GDT
		constexpr void	gdtResetSegments() noexcept;

		// Load GDT
		constexpr void	gdtLoad(const gdtPointer_t* gdtPtr) noexcept;


#ifdef	__cplusplus

	}

#endif	// __cplusplus


	// Init GDT table
	constexpr void gdt::init() noexcept {
		// Empty entry (should be there!)
		gdt::setEntry(0x00, 0x00000000, 0x00000000, GDT_ENTRY_EMPTY);
		// Kernel code
		gdt::setEntry(0x01, 0x00000000, 0xFFFFFFFF, GDT_ENTRY_CODE_RING0);
		// Kernel data
		gdt::setEntry(0x02, 0x00000000, 0xFFFFFFFF, GDT_ENTRY_DATA_RING0);
		// User code
		gdt::setEntry(0x03, 0x00000000, 0xFFFFFFFF, GDT_ENTRY_CODE_RING3);
		// User data
		gdt::setEntry(0x04, 0x00000000, 0xFFFFFFFF, GDT_ENTRY_DATA_RING3);
		// Set GDT size and data pointer
		pointer.size	= gdt::calcSize();
		pointer.pointer	= table;
		// Load new GDT
		gdtLoad(&pointer);
	}


}	// namespace arch


#endif	// IGROS_ARCH_GDT_HPP

