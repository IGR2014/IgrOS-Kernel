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
// IgrOS-Kernel arch i386
#include <arch/i386/exceptions.hpp>
#include <arch/i386/irq.hpp>


// i386 namespace
namespace igros::i386 {


#pragma pack(push, 1)

	// IDT entry
	struct idtEntry_t {
		igros_word_t		offsetLow;		// Offset lower part (0..15)
		igros_word_t		selector;		// Selectod from GDT/LDT
		igros_byte_t		reserved;		// Must be zero
		igros_byte_t		type;			// Type attributes
		igros_word_t		offsetHigh;		// Offset higher part (16..31)
	};

	// IDT pointer
	struct idtPointer_t {
		igros_word_t		size;			// IDT size
		const idtEntry_t*	pointer;		// IDT pointer
	};

#pragma pack(pop)


}	// namespace igros::i386


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus


	// Load IDT
	void	idtLoad(const igros::i386::idtPointer_t* const idtPtr) noexcept;
	// Store IDT
	auto	idtStore() noexcept -> const igros::i386::idtPointer_t*;


#ifdef	__cplusplus

}

#endif	// __cplusplus


// i386 namespace
namespace igros::i386 {


	// IDT structure
	class idt final {

		// Number of IDT entries
		constexpr static auto IDT_SIZE			{56_usize};

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

		// IDT offset type
		using offset_t = std::add_pointer_t<void ()>;

		// Default c-tor
		idt() noexcept = default;

		// Set IDT entry
		template<offset_t HANDLE, igros_word_t SELECTOR, igros_byte_t TYPE>
		[[nodiscard]]
		constexpr static auto	setEntry() noexcept -> idtEntry_t;
		// Calc IDT size
		[[nodiscard]]
		constexpr static auto	calcSize() noexcept -> igros_word_t;

		// Init IDT table
		static void		init() noexcept;


	};


	// Set IDT entry
	template<idt::offset_t HANDLE, igros_word_t SELECTOR, igros_byte_t TYPE>
	[[nodiscard]]
	constexpr auto idt::setEntry() noexcept -> idtEntry_t {
		return idtEntry_t {
			.offsetLow	= static_cast<igros_word_t>(std::bit_cast<igros_usize_t>(HANDLE) & 0xFFFF_usize),
			.selector	= SELECTOR,
			.reserved	= 0_u8,
			.type		= TYPE,
			.offsetHigh	= static_cast<igros_word_t>((std::bit_cast<igros_usize_t>(HANDLE) >> 16) & 0xFFFF_usize)
		};
	}

	// Calculate IDT size
	[[nodiscard]]
	constexpr auto idt::calcSize() noexcept -> igros_word_t {
		return static_cast<igros_word_t>(IDT_SIZE * sizeof(idtEntry_t)) - 1_u16;
	}


	// Init IDT table
	inline void idt::init() noexcept {
		// Load new IDT
		::idtLoad(&pointer);
	}


}	// namespace igros::i386

