////////////////////////////////////////////////////////////////
///
///	@file
///	@brief		Global descriptor table low-level operations
///	@date		03 Nov 2026
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
// IgrOS-Kernel library
#include <klib/kFlags.hpp>


// x86_64 namespace
namespace igros::x86_64 {


	/// @brief GDT structure
	class gdt final {

		/// @brief GDT entry flags: access byte (bits 0..7) and flags nibble (bits 8..11)
		enum class FLAGS : igros_word_t {
			EMPTY	     = 0x0000_u16, ///< No flags (null descriptor)
			TYPE_ACCESS  = 0x0001_u16, ///< Segment accessed
			TYPE_RODATA  = 0x0000_u16, ///< Read-only data
			TYPE_DATA    = 0x0002_u16, ///< Read/write data
			TYPE_ROSTACK = 0x0004_u16, ///< Read-only expand-down data
			TYPE_STACK   = 0x0006_u16, ///< Read/write expand-down data
			TYPE_EXCODE  = 0x0008_u16, ///< Execute-only code
			TYPE_CODE    = 0x000A_u16, ///< Execute/read code
			TYPE_EXCONF  = 0x000C_u16, ///< Execute-only conforming code
			TYPE_CONF    = 0x000E_u16, ///< Execute/read conforming code
			RING0	     = 0x0010_u16, ///< Code/data segment, privilege level 0
			RING1	     = 0x0030_u16, ///< Code/data segment, privilege level 1
			RING2	     = 0x0050_u16, ///< Code/data segment, privilege level 2
			RING3	     = 0x0070_u16, ///< Code/data segment, privilege level 3
			RING_USE     = 0x0080_u16, ///< Segment present
			L	     = 0x0200_u16, ///< 64-bit code segment
			SZ	     = 0x0400_u16, ///< 32-bit segment
			G	     = 0x0800_u16, ///< Limit in 4 KB units
		};


		/// @brief Empty GDT entry (the first one)
		constexpr static auto GDT_ENTRY_EMPTY {klib::kFlags<FLAGS> {}};
		/// @brief Kernel code segment (ring 0, code)
		constexpr static auto GDT_ENTRY_CODE_RING0 {klib::make_kflags<FLAGS>(FLAGS::G, FLAGS::L, FLAGS::RING_USE, FLAGS::RING0, FLAGS::TYPE_CODE)};
		/// @brief Kernel data segment (ring 0, data)
		constexpr static auto GDT_ENTRY_DATA_RING0 {klib::make_kflags<FLAGS>(FLAGS::G, FLAGS::L, FLAGS::RING_USE, FLAGS::RING0, FLAGS::TYPE_DATA)};
		/// @brief User code segment (ring 3, code)
		constexpr static auto GDT_ENTRY_CODE_RING3 {klib::make_kflags<FLAGS>(FLAGS::G, FLAGS::L, FLAGS::RING_USE, FLAGS::RING3, FLAGS::TYPE_CODE)};
		/// @brief User data segment (ring 3, data)
		constexpr static auto GDT_ENTRY_DATA_RING3 {klib::make_kflags<FLAGS>(FLAGS::G, FLAGS::L, FLAGS::RING_USE, FLAGS::RING3, FLAGS::TYPE_DATA)};

		/// @brief Number of GDT entries
		constexpr static auto GDT_SIZE {7_usize};


	public:

		/// @brief Copy c-tor
		gdt(const gdt &other) = delete;
		/// @brief Copy assignment
		auto operator=(const gdt &other) -> gdt & = delete;
		/// @brief Move c-tor
		gdt(gdt &&other) = delete;
		/// @brief Move assignment
		auto operator=(gdt &&other) -> gdt & = delete;


#pragma pack(push, 1)

		/// @brief GDT table entry
		struct entry_t {
			igros_word_t limitLow;	 ///< Limit bits 0..15
			igros_word_t baseLow;	 ///< Base bits 0..15
			igros_byte_t baseMid;	 ///< Base bits 16..23
			igros_byte_t access;	 ///< Access byte (present, privilege, type)
			igros_byte_t limitFlags; ///< Limit bits 16..19 and flags (granularity, size, long mode)
			igros_byte_t baseHigh;	 ///< Base bits 24..31
		};

		/// @brief GDT table pointer
		// cppcheck-suppress-begin unusedStructMember ; fixed layout read by the CPU, not by C++ code
		struct pointer_t {
			igros_word_t   size;	///< GDT size
			const entry_t* pointer; ///< GDT pointer
		};
		// cppcheck-suppress-end unusedStructMember

#pragma pack(pop)

		/// @brief GDT table
		using table_t = std::array<gdt::entry_t, gdt::GDT_SIZE>;

		// Segment selectors (index * 8, see init)
		constexpr static auto KERNEL_CODE {igros_word_t {0x08}}; ///< Kernel code segment
		constexpr static auto KERNEL_DATA {igros_word_t {0x10}}; ///< Kernel data segment
		constexpr static auto TSS {igros_word_t {0x28}};	 ///< 16-byte descriptor: entries 5 and 6

		/// @brief Default c-tor
		gdt() noexcept = default;
		/// @brief D-tor
		~gdt() noexcept = default;

		/// @brief 64-bit available TSS descriptor (two GDT entries: base is 64-bit)
		/// @param base TSS address
		/// @param limit TSS size - 1
		/// @return Two GDT entries (64-bit descriptor)
		[[nodiscard]]
		constexpr static auto tssEntry(const igros_quad_t base, const igros_dword_t limit) noexcept -> std::array<entry_t, 2>;

		/// @brief Set GDT entry
		/// @param base Segment base
		/// @param limit Segment limit (20 bits used)
		/// @param flags Access byte and flags
		/// @return GDT entry
		[[nodiscard]]
		constexpr static auto setEntry(const igros_dword_t base, const igros_dword_t limit, const klib::kFlags<FLAGS> flags) noexcept -> entry_t;
		/// @brief Calc GDT size
		/// @param table GDT
		/// @return GDT size in bytes - 1 (GDT pointer limit)
		[[nodiscard]]
		constexpr static auto calcSize(const table_t &table) noexcept -> igros_word_t;

		/// @brief Set GDT pointer
		/// @param pointer GDT pointer to load
		static void setPointer(const pointer_t &pointer) noexcept;

		/// @brief Init GDT table
		static void init() noexcept;
	};


	/// @brief Set GDT entry
	[[nodiscard]]
	constexpr auto gdt::setEntry(const igros_dword_t base, const igros_dword_t limit, const klib::kFlags<FLAGS> flags) noexcept -> entry_t {
		// Flags in 32-bit arithmetic (16-bit operands would be promoted to int)
		const auto bits {static_cast<igros_dword_t>(flags.value())};
		return entry_t {
			.limitLow   = static_cast<igros_word_t>(limit & 0xFFFF_u32),
			.baseLow    = static_cast<igros_word_t>(base & 0xFFFF_u32),
			.baseMid    = static_cast<igros_byte_t>((base & 0xFF0000_u32) >> 16),
			.access	    = static_cast<igros_byte_t>(bits & 0x00FF_u32),
			.limitFlags = static_cast<igros_byte_t>(((limit & 0xF0000_u32) >> 16) | ((bits & 0x0F00_u32) >> 4)),
			.baseHigh   = static_cast<igros_byte_t>((base & 0xFF000000_u32) >> 24),
		};
	}

	/// @brief 64-bit available TSS descriptor
	[[nodiscard]]
	constexpr auto gdt::tssEntry(const igros_quad_t base, const igros_dword_t limit) noexcept -> std::array<entry_t, 2> {
		// Present, ring 0, type 9 (available 64-bit TSS), byte granular limit
		constexpr auto TSS_AVAILABLE {0x89_u8};
		return {
			entry_t {
				.limitLow   = static_cast<igros_word_t>(limit & 0xFFFF_u32),
				.baseLow    = static_cast<igros_word_t>(base & 0xFFFF_u64),
				.baseMid    = static_cast<igros_byte_t>((base >> 16) & 0xFF_u64),
				.access	    = TSS_AVAILABLE,
				.limitFlags = static_cast<igros_byte_t>((limit >> 16) & 0x0F_u32),
				.baseHigh   = static_cast<igros_byte_t>((base >> 24) & 0xFF_u64),
			},
			// Upper half: base bits 32..63, rest reserved
			entry_t {
				.limitLow   = static_cast<igros_word_t>((base >> 32) & 0xFFFF_u64),
				.baseLow    = static_cast<igros_word_t>((base >> 48) & 0xFFFF_u64),
				.baseMid    = 0_u8,
				.access	    = 0_u8,
				.limitFlags = 0_u8,
				.baseHigh   = 0_u8,
			},
		};
	}

	/// @brief Calculate GDT size
	constexpr auto gdt::calcSize(const table_t &table) noexcept -> igros_word_t {
		// Size equals to (Num of entries * Entry size) - 1
		return static_cast<igros_word_t>(table.size() * sizeof(entry_t)) - 1_u16;
	}


} // namespace igros::x86_64

