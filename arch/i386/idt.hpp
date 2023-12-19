////////////////////////////////////////////////////////////////
//
//	Interrupt descriptor table low-level operations
//
//	File:	idt.hpp
//	Date:	19 Dec 2023
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
#include <limits>
#include <type_traits>
// IgrOS-Kernel arch
#include <arch/types.hpp>
// IgrOS-Kernel arch i386
#include <arch/i386/exceptions.hpp>
#include <arch/i386/irq.hpp>


// i386 namespace
namespace igros::i386 {


	// IDT structure
	class idt final {

		// Number of IDT entries
		constexpr static auto IDT_SIZE {256_usize};

		// Copy c-tor
		idt(const idt &other) = delete;
		// Copy assignment
		idt& operator=(const idt &other) = delete;

		// Move c-tor
		idt(idt &&other) = delete;
		// Move assignment
		idt& operator=(idt &&other) = delete;


	public:

#pragma pack(push, 1)

		// IDT table entry
		struct entry_t {
			igros_word_t	offsetLow;	// Offset lower part (0..15)
			igros_word_t	selector;	// Selectod from GDT/LDT
			igros_byte_t	reserved;	// Must be zero
			igros_byte_t	type;		// Type attributes
			igros_word_t	offsetHigh;	// Offset higher part (16..31)
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

		// Set IDT entry
		template<offset_t HANDLE, igros_word_t SELECTOR, igros_byte_t TYPE>
		[[nodiscard]]
		constexpr static auto	setEntry() noexcept -> entry_t;
		// Calc IDT size
		[[nodiscard]]
		constexpr static auto	calcSize(const table_t &table) noexcept -> igros_word_t;

		// Set IDT pointer
		static void		setPointer(const pointer_t &pointer) noexcept;

		// Init IDT table
		static void		init() noexcept;


	};


	// Set IDT entry
	template<idt::offset_t HANDLE, igros_word_t SELECTOR, igros_byte_t TYPE>
	[[nodiscard]]
	constexpr auto idt::setEntry() noexcept -> entry_t {
		return entry_t {
			.offsetLow	= static_cast<igros_word_t>(std::bit_cast<igros_usize_t>(HANDLE) & 0xFFFF_usize),
			.selector	= SELECTOR,
			.reserved	= 0_u8,
			.type		= TYPE,
			.offsetHigh	= static_cast<igros_word_t>((std::bit_cast<igros_usize_t>(HANDLE) >> std::numeric_limits<igros_word_t>::digits) & 0xFFFF_usize)
		};
	}

	// Calculate IDT size
	[[nodiscard]]
	constexpr auto idt::calcSize(const table_t &table) noexcept -> igros_word_t {
		return static_cast<igros_word_t>(table.size() * sizeof(entry_t)) - 1_u16;
	}


}	// namespace igros::i386

