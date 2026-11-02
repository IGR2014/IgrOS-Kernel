////////////////////////////////////////////////////////////////
///
///	@file
///	@brief		Global descriptor table low-level operations
///	@date		19 Dec 2023
///
///	@copyright	Copyright (c) 2017 - 2022, Igor Baklykov
///			All rights reserved.
///	@author		Igor Baklykov
///


#pragma once


// C++
#include <array>
// IgrOS-Kernel arch
#include <arch/types.hpp>


// i386 namespace
namespace igros::i386 {


	/// @brief GDT structure
	class gdt final {

		/// @brief GDT flags enum
		enum flags_t : igros_word_t {	// NOLINT(cppcoreguidelines-use-enum-class): bit flags, combined and masked as integers
			/// @brief GDT empty segment
			GDT_SEG_EMPTY		= 0x0000_u16,
			/// @brief GDT access page
			GDT_SEG_TYPE_ACCESS	= 0x0001_u16,
			/// @brief GDT page types
			GDT_SEG_TYPE_RODATA	= GDT_SEG_EMPTY,
			GDT_SEG_TYPE_DATA	= 0x0002_u16,
			GDT_SEG_TYPE_ROSTACK	= 0x0004_u16,
			GDT_SEG_TYPE_STACK	= 0x0006_u16,
			GDT_SEG_TYPE_EXCODE	= 0x0008_u16,
			GDT_SEG_TYPE_CODE	= 0x000A_u16,
			GDT_SEG_TYPE_EXCONF	= 0x000C_u16,
			GDT_SEG_TYPE_CONF	= 0x000E_u16,
			/// @brief GDT rings
			GDT_SEG_RING0		= 0x0010_u16,
			GDT_SEG_RING1		= 0x0030_u16,
			GDT_SEG_RING2		= 0x0050_u16,
			GDT_SEG_RING3		= 0x0070_u16,
			/// @brief GDT use ring number
			GDT_SEG_RING_USE	= 0x0080_u16,
			/// @brief GDT types
			GDT_SEG_L		= 0x0200_u16,
			GDT_SEG_SZ		= 0x0400_u16,
			GDT_SEG_G		= 0x0800_u16,
			/// @brief GDT entries
			GDT_SEG_RING0_CODE	= GDT_SEG_G		|
						  GDT_SEG_SZ		|
						  GDT_SEG_RING_USE	|
						  GDT_SEG_RING0		|
						  GDT_SEG_TYPE_CODE,
			GDT_SEG_RING0_DATA	= GDT_SEG_G		|
						  GDT_SEG_SZ		|
						  GDT_SEG_RING_USE	|
						  GDT_SEG_RING0		|
						  GDT_SEG_TYPE_DATA,
			GDT_SEG_RING3_CODE	= GDT_SEG_G		|
						  GDT_SEG_SZ		|
						  GDT_SEG_RING_USE	|
						  GDT_SEG_RING3		|
						  GDT_SEG_TYPE_CODE,
			GDT_SEG_RING3_DATA	= GDT_SEG_G		|
						  GDT_SEG_SZ		|
						  GDT_SEG_RING_USE	|
						  GDT_SEG_RING3		|
						  GDT_SEG_TYPE_DATA,
		};


		/// @brief Empty GDT entry (the first one)
		constexpr static auto	GDT_ENTRY_EMPTY			{flags_t::GDT_SEG_EMPTY};
		/// @brief Kernel code page (4 Mb page, ring 0, code)
		constexpr static auto	GDT_ENTRY_CODE_RING0		{flags_t::GDT_SEG_RING0_CODE};
		/// @brief Kernel data page (4 Mb page, ring 0, data)
		constexpr static auto	GDT_ENTRY_DATA_RING0		{flags_t::GDT_SEG_RING0_DATA};
		/// @brief User code page (4 Mb page, ring 3, code)
		constexpr static auto	GDT_ENTRY_CODE_RING3		{flags_t::GDT_SEG_RING3_CODE};
		/// @brief User data page (4 Mb page, ring 3, data)
		constexpr static auto	GDT_ENTRY_DATA_RING3		{flags_t::GDT_SEG_RING3_DATA};

		/// @brief Number of GDT entries
		constexpr static auto	GDT_SIZE			{7_usize};

		/// @brief Copy c-tor
		gdt(const gdt &other) = delete;
		/// @brief Copy assignment
		auto	operator=(const gdt &other) -> gdt& = delete;

		/// @brief Move c-tor
		gdt(gdt &&other) = delete;
		/// @brief Move assignment
		auto	operator=(gdt &&other) -> gdt& = delete;


	public:

#pragma pack(push, 1)

		/// @brief GDT table entry
		struct entry_t {
			igros_word_t	limitLow;	///< Limit bits 0..15
			igros_word_t	baseLow;	///< Base bits 0..15
			igros_byte_t	baseMid;	///< Base bits 16..23
			igros_byte_t	access;	///< Access byte (present, privilege, type)
			igros_byte_t	limitFlags;	///< Limit bits 16..19 and flags (granularity, size, long mode)
			igros_byte_t	baseHigh;	///< Base bits 24..31
		};

		/// @brief GDT table pointer
		struct pointer_t {
			igros_word_t	size;		///< GDT size
			const entry_t*	pointer;	///< GDT pointer
		};

#pragma pack(pop)

		/// @brief GDT table
		using table_t = std::array<gdt::entry_t, gdt::GDT_SIZE>;

		// Segment selectors (index * 8, see init)
		constexpr static auto	KERNEL_CODE		{igros_word_t {0x08}};	///< Kernel code segment
		constexpr static auto	KERNEL_DATA		{igros_word_t {0x10}};	///< Kernel data segment
		constexpr static auto	TSS			{igros_word_t {0x28}};	///< Kernel task
		constexpr static auto	TSS_DOUBLE_FAULT	{igros_word_t {0x30}};	///< Double fault task

		/// @brief Default c-tor
		gdt() noexcept = default;
		/// @brief D-tor
		~gdt() noexcept = default;

		/// @brief Available 32-bit TSS descriptor (base is known at run time only)
		/// @param base TSS address
		/// @param limit TSS size - 1
		/// @return GDT entry
		[[nodiscard]]
		constexpr static auto	tssEntry(const igros_dword_t base, const igros_dword_t limit) noexcept -> entry_t;

		/// @brief Set GDT entry
		/// @tparam BASE Segment base
		/// @tparam LIMIT Segment limit (20 bits used)
		/// @tparam FLAGS Access byte and flags
		/// @return GDT entry
		template<igros_dword_t BASE, igros_dword_t LIMIT, gdt::flags_t FLAGS>
		[[nodiscard]]
		constexpr static auto	setEntry() noexcept -> entry_t;
		/// @brief Calc GDT size
		/// @param table GDT
		/// @return GDT size in bytes - 1 (GDT pointer limit)
		[[nodiscard]]
		constexpr static auto	calcSize(const table_t &table) noexcept -> igros_word_t;

		/// @brief Set GDT pointer
		/// @param pointer GDT pointer to load
		static void		setPointer(const pointer_t &pointer) noexcept;

		/// @brief Init GDT table
		static void		init() noexcept;


	};


	/// @brief Set GDT entry
	template<igros_dword_t BASE, igros_dword_t LIMIT, gdt::flags_t FLAGS>
	[[nodiscard]]
	constexpr auto gdt::setEntry() noexcept -> entry_t {
		return entry_t {
			.limitLow	= static_cast<igros_word_t>(LIMIT & 0xFFFF_u32),
			.baseLow	= static_cast<igros_word_t>(BASE & 0xFFFF_u32),
			.baseMid	= static_cast<igros_byte_t>((BASE & 0xFF0000_u32) >> 16),
			.access		= static_cast<igros_byte_t>(FLAGS & 0x00FF_u16),
			.limitFlags	= static_cast<igros_byte_t>(((LIMIT & 0xF0000_u32) >> 16) | (static_cast<igros_word_t>(FLAGS & 0x0F00_u16) >> 4)),
			.baseHigh	= static_cast<igros_byte_t>((BASE & 0xFF000000_u32) >> 24),
		};
	}

	/// @brief Available 32-bit TSS descriptor
	[[nodiscard]]
	constexpr auto gdt::tssEntry(const igros_dword_t base, const igros_dword_t limit) noexcept -> entry_t {
		// Present, ring 0, type 9 (available 32-bit TSS), byte granular limit
		constexpr auto TSS_AVAILABLE {0x89_u8};
		return entry_t {
			.limitLow	= static_cast<igros_word_t>(limit & 0xFFFF_u32),
			.baseLow	= static_cast<igros_word_t>(base & 0xFFFF_u32),
			.baseMid	= static_cast<igros_byte_t>((base >> 16) & 0xFF_u32),
			.access		= TSS_AVAILABLE,
			.limitFlags	= static_cast<igros_byte_t>((limit >> 16) & 0x0F_u32),
			.baseHigh	= static_cast<igros_byte_t>((base >> 24) & 0xFF_u32),
		};
	}

	/// @brief Calculate GDT size
	[[nodiscard]]
	constexpr auto gdt::calcSize(const table_t &table) noexcept -> igros_word_t {
		// Size equals to (Num of entries * Entry size) - 1
		return static_cast<igros_word_t>(table.size() * sizeof(entry_t)) - 1_u16;
	}


}	// namespace igros::i386

