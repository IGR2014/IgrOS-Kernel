////////////////////////////////////////////////////////////////
//
//	Global descriptor table low-level operations
//
//	File:	gdt.hpp
//	Date:	08 Feb 2021
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <array>

#include <flags.hpp>

#include <arch/x86_64/types.hpp>


// x86_64 namespace
namespace igros::x86_64 {


#pragma pack(push, 1)

	// GDT entry
	struct gdtEntryx86_64_t {
		word_t		limitLow;
		word_t		baseLow;
		byte_t		baseMid;
		byte_t		access;
		byte_t		limitFlags;
		byte_t		baseHigh;
	};

	// GDT pointer
	struct gdtPointerx86_64_t {
		word_t			size;			// GDT size
		const gdtEntryx86_64_t*	pointer;		// GDT pointer
	};

#pragma pack(pop)


}	// namespace igros::x86_64


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus

	// Reset segments to apply new GDT
	inline void					gdtResetSegments() noexcept;

	// Load GDT
	inline void					gdtLoad(const igros::x86_64::gdtPointerx86_64_t* gdtPtr) noexcept;
	// Store GDT
	inline const igros::x86_64::gdtPointerx86_64_t*	gdtStore() noexcept;

#ifdef	__cplusplus

}

#endif	// __cplusplus


// x86_64 namespace
namespace igros::x86_64 {


	// GDT structure
	class gdtx86_64 final {

		// GDT flags enum
		enum flags_t : word_t {
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
		constexpr static flags_t GDT_ENTRY_EMPTY	= flags_t::GDT_SEG_EMPTY;
		// Kernel code page (4 Mb page, ring 0, code)
		constexpr static flags_t GDT_ENTRY_CODE_RING0	= flags_t::GDT_SEG_RING0_CODE;
		// Kernel data page (4 Mb page, ring 0, data)
		constexpr static flags_t GDT_ENTRY_DATA_RING0	= flags_t::GDT_SEG_RING0_DATA;
		// User code page (4 Mb page, ring 3, code)
		constexpr static flags_t GDT_ENTRY_CODE_RING3	= flags_t::GDT_SEG_RING3_CODE;
		// User data page (4 Mb page, ring 3, data)
		constexpr static flags_t GDT_ENTRY_DATA_RING3	= flags_t::GDT_SEG_RING3_DATA;

		// Number of GDT entries
		constexpr static auto				GDT_SIZE		= 5ULL;

		// Global descriptors table (GDT)
		static std::array<gdtEntryx86_64_t, GDT_SIZE>	table;
		// Pointer to GDT
		static gdtPointerx86_64_t			pointer;


		// Copy c-tor
		gdtx86_64(const gdtx86_64 &other) = delete;
		// Copy assignment
		gdtx86_64& operator=(const gdtx86_64 &other) = delete;

		// Move c-tor
		gdtx86_64(gdtx86_64 &&other) = delete;
		// Move assignment
		gdtx86_64& operator=(gdtx86_64 &&other) = delete;


	public:

		// Default c-tor
		gdtx86_64() noexcept = default;

		// Set GDT entry
		constexpr static gdtEntryx86_64_t	setEntry(const dword_t base, const dword_t &limit, const flags_t flags) noexcept;
		// Calc GDT size
		[[nodiscard]]
		constexpr static word_t			calcSize() noexcept;

		// Init GDT table
		static void	init() noexcept;


	};


	// Set GDT entry
	constexpr gdtEntryx86_64_t gdtx86_64::setEntry(const dword_t base, const dword_t &limit, const flags_t flags) noexcept {
		return {
			.limitLow	= static_cast<word_t>(limit & 0xFFFF),
			.baseLow	= static_cast<word_t>(base & 0xFFFF),
			.baseMid	= static_cast<byte_t>((base & 0xFF0000) >> 16),
			.access		= static_cast<byte_t>(flags & 0x00FF),
			.limitFlags	= static_cast<byte_t>(((limit & 0xF0000) >> 16) | (static_cast<word_t>(flags & 0x0F00) >> 4)),
			.baseHigh	= static_cast<byte_t>((base & 0xFF000000) >> 24)
		};
	}

	// Calculate GDT size
	constexpr word_t gdtx86_64::calcSize() noexcept {
		// Size equals to (Num of entries * Entry size) - 1
		return (gdtx86_64::table.size() * sizeof(gdtEntryx86_64_t)) - 1U;
	}


	// Init GDT table
	inline void gdtx86_64::init() noexcept {
		// Load new GDT
		::gdtLoad(&pointer);
	}


}	// namespace igros::x86_64

