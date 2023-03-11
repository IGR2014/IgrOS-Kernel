////////////////////////////////////////////////////////////////
//
//	Interrupt descriptor table low-level operations
//
//	File:	idt.hpp
//	Date:	11 Mar 2023
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
// IgrOS-Kernel arch x86_64
#include <arch/x86_64/irq.hpp>
#include <arch/x86_64/exceptions.hpp>


// x86_64 namespace
namespace igros::x86_64 {


#pragma pack(push, 1)

	// IDT entry
	struct idtEntry_t {
		igros_word_t		offsetLow;		// Offset lower part (0..15)
		igros_word_t		selector;		// Selectod from GDT/LDT
		igros_byte_t		ist;			// Interrupt service table
		igros_byte_t		type;			// Type attributes
		igros_word_t		offsetMiddle;		// Offset middle part (16..31)
		igros_dword_t		offsetHigh;		// Offset higher part (32..63)
		igros_dword_t		reserved2;		// Must be zero
	};

	// IDT pointer
	struct idtPointer_t {
		igros_word_t		size;			// IDT size
		const idtEntry_t*	pointer;		// IDT pointer
	};

#pragma pack(pop)


}	// namespace igros::x86_64


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus


	// Load IDT
	void	idtLoad(const igros::x86_64::idtPointer_t* const idtPtr) noexcept;
	// Store IDT
	auto	idtStore() noexcept -> const igros::x86_64::idtPointer_t*;


#ifdef	__cplusplus

}

#endif	// __cplusplus


// x86_64 namespace
namespace igros::x86_64 {


	// IDT structure
	class idt final {

		// IDT ISR pointer
		using isrPointer_t			= std::add_pointer_t<void()>;

		// Number of IDT entries
		constexpr static auto IDT_SIZE			{256_usize};

		// Exceptions and IRQ descriptors table (IDT)
		static std::array<idtEntry_t, IDT_SIZE>		table;
		// Pointer to IDT
		static constinit idtPointer_t			pointer;


		// Copy c-tor
		idt(const idt &other) = delete;
		// Copy assignment
		idt& operator=(const idt &other) = delete;

		// Move c-tor
		idt(idt &&other) = delete;
		// Move assignment
		idt& operator=(idt &&other) = delete;


	public:

		// Default c-tor
		idt() noexcept = default;

		// Set IDT entry
		constexpr static auto	setEntry(const isrPointer_t offset, const igros_word_t selector, const igros_byte_t type) noexcept -> idtEntry_t;
		// Calc IDT size
		[[nodiscard]]
		constexpr static auto	calcSize() noexcept -> igros_dword_t;

		// Init IDT table
		static void		init() noexcept;


	};


	// Set IDT entry
	[[nodiscard]]
	constexpr auto idt::setEntry(const isrPointer_t offset, const igros_word_t selector, const igros_byte_t type) noexcept -> idtEntry_t {
		return idtEntry_t {
			.offsetLow	= static_cast<igros_word_t>(std::bit_cast<igros_usize_t>(offset) & 0xFFFF_u64),
			.selector	= selector,
			.ist		= 0_u8,
			.type		= type,
			.offsetMiddle	= static_cast<igros_word_t>((std::bit_cast<igros_usize_t>(offset) & 0xFFFF0000_u64) >> 16),
			.offsetHigh	= static_cast<igros_dword_t>((std::bit_cast<igros_usize_t>(offset) & 0xFFFFFFFF00000000_u64) >> 32),
			.reserved2	= 0_u32
		};
	}

	// Calculate IDT size
	[[nodiscard]]
	constexpr auto idt::calcSize() noexcept -> igros_dword_t {
		return static_cast<igros_dword_t>(IDT_SIZE * sizeof(idtEntry_t)) - 1_u16;
	}


	// Init IDT table
	inline void idt::init() noexcept {
		// Load new IDT
		::idtLoad(&pointer);
	}


}	// namespace igros::x86_64

