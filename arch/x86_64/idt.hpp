////////////////////////////////////////////////////////////////
//
//	Interrupt descriptor table low-level operations
//
//	File:	idt.hpp
//	Date:	22 Dec 2023
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// C++
#include <array>
#include <bit>
#include <cstdint>
#include <type_traits>
// IgrOS-Kernel arch
#include <arch/types.hpp>
// IgrOS-Kernel arch x86
#include <arch/x86/exceptions.hpp>
#include <arch/x86/irq.hpp>


// x86_64 namespace
namespace igros::x86_64 {


	// IDT structure
	class idt final {

		// Number of IDT entries
		constexpr static auto IDT_SIZE {256_usize};

		// Copy c-tor
		idt(const idt &other) = delete;
		// Copy assignment
		auto	operator=(const idt &other) -> idt& = delete;

		// Move c-tor
		idt(idt &&other) = delete;
		// Move assignment
		auto	operator=(idt &&other) -> idt& = delete;


	public:

#pragma pack(push, 1)

		// IDT table entry
		struct entry_t {
			igros_word_t	offsetLow;	// Offset lower part (0..15)
			igros_word_t	selector;	// Selectod from GDT/LDT
			igros_byte_t	ist;		// Interrupt service table
			igros_byte_t	type;		// Type attributes
			igros_word_t	offsetMiddle;	// Offset middle part (16..31)
			igros_dword_t	offsetHigh;	// Offset higher part (32..63)
			igros_dword_t	reserved2;	// Must be zero
		};

		// IDT table pointer
		struct pointer_t {
			igros_word_t	size;		// IDT size
			const entry_t*	pointer;	// IDT pointer
		};

#pragma pack(pop)

		// IDT table type
		using table_t	= std::array<entry_t, IDT_SIZE>;

		// IDT offset type
		using offset_t	= std::add_pointer_t<void ()>;

		// Default c-tor
		idt() noexcept = default;

		// Make IDT entry
		[[nodiscard]]
		static auto	setEntry(const offset_t handle, const igros_word_t selector, const igros_byte_t type) noexcept -> entry_t;
		// Calc IDT size
		[[nodiscard]]
		constexpr static auto	calcSize(const table_t &table) noexcept -> igros_word_t;

		// Set IDT pointer
		static void		setPointer(const pointer_t &pointer) noexcept;

		// Init IDT table
		static void		init() noexcept;


	};


	// Make IDT entry
	[[nodiscard]]
	inline auto idt::setEntry(const offset_t handle, const igros_word_t selector, const igros_byte_t type) noexcept -> entry_t {
		// Handler address
		const auto address {std::bit_cast<igros_usize_t>(handle)};
		return entry_t {
			.offsetLow	= static_cast<igros_word_t>(address & 0xFFFF_u64),
			.selector	= selector,
			.ist		= 0_u8,
			.type		= type,
			.offsetMiddle	= static_cast<igros_word_t>((address & 0xFFFF0000_u64) >> 16),
			.offsetHigh	= static_cast<igros_dword_t>((address & 0xFFFFFFFF00000000_u64) >> 32),
			.reserved2	= 0_u32
		};
	}

	// Calculate IDT size
	[[nodiscard]]
	constexpr auto idt::calcSize(const table_t &table) noexcept -> igros_word_t {
		return static_cast<igros_word_t>(table.size() * sizeof(entry_t)) - 1_u16;
	}


}	// namespace igros::x86_64

