////////////////////////////////////////////////////////////////
//
//	Global descriptor table low-level operations
//
//	File:	gdt.hpp
//	Date:	22 Dec 2023
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// C++
#include <array>
// IgrOS-Kernel arch
#include <arch/types.hpp>
// IgrOS-Kernel library
#include <klib/kFlags.hpp>


// x86_64 namespace
namespace igros::x86_64 {


	// GDT structure
	class gdt final {

		// GDT flags enum
		enum flags_t : igros_word_t {
			// GDT empty segment
			GDT_SEG_EMPTY		= 0x0000_u16,
			// GDT access page
			GDT_SEG_TYPE_ACCESS	= 0x0001_u16,
			// GDT page types
			GDT_SEG_TYPE_RODATA	= GDT_SEG_EMPTY,
			GDT_SEG_TYPE_DATA	= 0x0002_u16,
			GDT_SEG_TYPE_ROSTACK	= 0x0004_u16,
			GDT_SEG_TYPE_STACK	= 0x0006_u16,
			GDT_SEG_TYPE_EXCODE	= 0x0008_u16,
			GDT_SEG_TYPE_CODE	= 0x000A_u16,
			GDT_SEG_TYPE_EXCONF	= 0x000C_u16,
			GDT_SEG_TYPE_CONF	= 0x000E_u16,
			// GDT rings
			GDT_SEG_RING0		= 0x0010_u16,
			GDT_SEG_RING1		= 0x0030_u16,
			GDT_SEG_RING2		= 0x0050_u16,
			GDT_SEG_RING3		= 0x0070_u16,
			// GDT use ring number
			GDT_SEG_RING_USE	= 0x0080_u16,
			// GDT types
			GDT_SEG_L		= 0x0200_u16,
			GDT_SEG_SZ		= 0x0400_u16,
			GDT_SEG_G		= 0x0800_u16,
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
		constexpr static auto	GDT_ENTRY_EMPTY			{flags_t::GDT_SEG_EMPTY};
		// Kernel code page (4 Mb page, ring 0, code)
		constexpr static auto	GDT_ENTRY_CODE_RING0		{flags_t::GDT_SEG_RING0_CODE};
		// Kernel data page (4 Mb page, ring 0, data)
		constexpr static auto	GDT_ENTRY_DATA_RING0		{flags_t::GDT_SEG_RING0_DATA};
		// User code page (4 Mb page, ring 3, code)
		constexpr static auto	GDT_ENTRY_CODE_RING3		{flags_t::GDT_SEG_RING3_CODE};
		// User data page (4 Mb page, ring 3, data)
		constexpr static auto	GDT_ENTRY_DATA_RING3		{flags_t::GDT_SEG_RING3_DATA};

		// Number of GDT entries
		constexpr static auto	GDT_SIZE			{6_usize};

		// Copy c-tor
		gdt(const gdt &other) = delete;
		// Copy assignment
		auto	operator=(const gdt &other) -> gdt& = delete;

		// Move c-tor
		gdt(gdt &&other) = delete;
		// Move assignment
		auto	operator=(gdt &&other) -> gdt& = delete;


	public:

#pragma pack(push, 1)

		// GDT table entry
		struct entry_t {
			igros_word_t	limitLow;
			igros_word_t	baseLow;
			igros_byte_t	baseMid;
			igros_byte_t	access;
			igros_byte_t	limitFlags;
			igros_byte_t	baseHigh;
		};

		// GDT table pointer
		struct pointer_t {
			igros_word_t	size;		// GDT size
			const entry_t*	pointer;	// GDT pointer
		};

#pragma pack(pop)

		// GDT table
		using table_t = std::array<gdt::entry_t, gdt::GDT_SIZE>;

		// Default c-tor
		gdt() noexcept = default;

		// Set GDT entry
		template<igros_dword_t BASE, igros_dword_t LIMIT, flags_t FLAGS>
		[[nodiscard]]
		constexpr static auto	setEntry() noexcept -> entry_t;
		// Calc GDT size
		[[nodiscard]]
		constexpr static auto	calcSize(const table_t &table) noexcept -> igros_word_t;

		// Set GDT pointer
		static void		setPointer(const pointer_t &pointer) noexcept;

		// Init GDT table
		static void		init() noexcept;


	};


	// Set GDT entry
	template<igros_dword_t BASE, igros_dword_t LIMIT, gdt::flags_t FLAGS>
	[[nodiscard]]
	constexpr auto gdt::setEntry() noexcept -> entry_t {
		return entry_t {
			.limitLow	= static_cast<igros_word_t>(LIMIT & 0xFFFF_u32),
			.baseLow	= static_cast<igros_word_t>(BASE & 0xFFFF_u32),
			.baseMid	= static_cast<igros_byte_t>((BASE & 0xFF0000_u32) >> 16),
			.access		= static_cast<igros_byte_t>(FLAGS & 0x00FF_u16),
			.limitFlags	= static_cast<igros_byte_t>(((LIMIT & 0xF0000_u32) >> 16) | (static_cast<igros_word_t>(FLAGS & 0x0F00_u16) >> 4)),
			.baseHigh	= static_cast<igros_byte_t>((BASE & 0xFF000000_u32) >> 24)
		};
	}

	// Calculate GDT size
	constexpr auto gdt::calcSize(const table_t &table) noexcept -> igros_word_t {
		// Size equals to (Num of entries * Entry size) - 1
		return static_cast<igros_word_t>(table.size() * sizeof(entry_t)) - 1_u16;
	}


}	// namespace igros::x86_64

