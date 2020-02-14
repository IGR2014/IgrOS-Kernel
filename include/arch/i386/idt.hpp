////////////////////////////////////////////////////////////////
//
//	Interrupt descriptor table low-level operations
//
//	File:	idt.hpp
//	Date:	12 Feb 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#pragma once
#ifndef IGROS_ARCH_IDT_HPP
#define IGROS_ARCH_IDT_HPP


#include <type_traits>

#include <types.hpp>
#include <irq.hpp>
#include <exceptions.hpp>


// Arch-dependent code zone
namespace arch {


#pragma pack(push, 1)

	// IDT entry
	struct idtEntry_t {
		word_t		offsetLow;		// Offset lower part (0..15)
		word_t		selector;		// Selectod from GDT/LDT
		byte_t		reserved;		// Must be zero
		byte_t		type;			// Type attributes
		word_t		offsetHigh;		// Offset higher part (16..31)
	};

	// IDT pointer
	struct idtPointer_t {
		word_t		size;			// IDT size
		idtEntry_t*	pointer;		// IDT pointer
	};

#pragma pack(pop)

	// IDT structure
	class idt final {

		// IDT ISR pointer
		using isrPointer_t = std::add_pointer_t<void()>;

		// Number of IDT entries
		constexpr static dword_t IDT_SIZE = 256u;

		// Exceptions and IRQ descriptors table (IDT)
		static idtEntry_t	table[IDT_SIZE];
		// Pointer to IDT
		static idtPointer_t	pointer;


	public:

		// Default c-tor
		idt() noexcept = default;

		// Copy c-tor
		idt(const idt &other) = delete;
		// Copy assignment
		idt& operator=(const idt &other) = delete;

		// Move c-tor
		idt(idt &&other) = delete;
		// Move assignment
		idt& operator=(idt &&other) = delete;

		// Set IDT entry
		constexpr static void	setEntry(const byte_t ID, const isrPointer_t offset, const word_t selector, const byte_t type) noexcept;
		// Calc IDT size
		[[nodiscard]] constexpr static word_t	calcSize() noexcept;

		// Init IDT table
		constexpr static void	init() noexcept;


	};


	// Set IDT entry
	constexpr void idt::setEntry(const byte_t ID, const isrPointer_t offset, const word_t selector, const byte_t type) noexcept {
		table[ID] = {
			.offsetLow	= word_t(dword_t(offset) & 0xFFFF),
			.selector	= selector,
			.reserved	= 0x00,
			.type		= type,
			.offsetHigh	= word_t((dword_t(offset) & 0xFFFF0000) >> 16)
		};
	}

	// Calculate IDT size
	constexpr word_t idt::calcSize() noexcept {
		return (IDT_SIZE * sizeof(idtEntry_t)) - 1u;
	}


#ifdef	__cplusplus

	extern "C" {

#endif	// __cplusplus


		// Load IDT
		constexpr void idtLoad(const idtPointer_t* idtPtr) noexcept;


#ifdef	__cplusplus

	}

#endif	// __cplusplus


	// Init IDT table
	constexpr void idt::init() noexcept {
		// Exceptions setup
		idt::setEntry(0x00, exHandler00, 0x08, 0x8E);
		idt::setEntry(0x01, exHandler01, 0x08, 0x8E);
		idt::setEntry(0x02, exHandler02, 0x08, 0x8E);
		idt::setEntry(0x03, exHandler03, 0x08, 0x8E);
		idt::setEntry(0x04, exHandler04, 0x08, 0x8E);
		idt::setEntry(0x05, exHandler05, 0x08, 0x8E);
		idt::setEntry(0x06, exHandler06, 0x08, 0x8E);
		idt::setEntry(0x07, exHandler07, 0x08, 0x8E);
		idt::setEntry(0x08, exHandler08, 0x08, 0x8E);
		idt::setEntry(0x09, exHandler09, 0x08, 0x8E);
		idt::setEntry(0x0A, exHandler0A, 0x08, 0x8E);
		idt::setEntry(0x0B, exHandler0B, 0x08, 0x8E);
		idt::setEntry(0x0C, exHandler0C, 0x08, 0x8E);
		idt::setEntry(0x0D, exHandler0D, 0x08, 0x8E);
		idt::setEntry(0x0E, exHandler0E, 0x08, 0x8E);
		idt::setEntry(0x0F, exHandler0F, 0x08, 0x8E);
		idt::setEntry(0x10, exHandler10, 0x08, 0x8E);
		idt::setEntry(0x11, exHandler11, 0x08, 0x8E);
		idt::setEntry(0x12, exHandler12, 0x08, 0x8E);
		idt::setEntry(0x13, exHandler13, 0x08, 0x8E);
		idt::setEntry(0x14, exHandler14, 0x08, 0x8E);
		idt::setEntry(0x15, exHandler15, 0x08, 0x8E);
		idt::setEntry(0x16, exHandler16, 0x08, 0x8E);
		idt::setEntry(0x17, exHandler17, 0x08, 0x8E);
		idt::setEntry(0x18, exHandler18, 0x08, 0x8E);
		idt::setEntry(0x19, exHandler19, 0x08, 0x8E);
		idt::setEntry(0x1A, exHandler1A, 0x08, 0x8E);
		idt::setEntry(0x1B, exHandler1B, 0x08, 0x8E);
		idt::setEntry(0x1C, exHandler1C, 0x08, 0x8E);
		idt::setEntry(0x1D, exHandler1D, 0x08, 0x8E);
		idt::setEntry(0x1E, exHandler1E, 0x08, 0x8E);
		idt::setEntry(0x1F, exHandler1F, 0x08, 0x8E);
		// IRQs setup
		idt::setEntry(0x20, irqHandler0, 0x08, 0x8E);
		idt::setEntry(0x21, irqHandler1, 0x08, 0x8E);
		idt::setEntry(0x22, irqHandler2, 0x08, 0x8E);
		idt::setEntry(0x23, irqHandler3, 0x08, 0x8E);
		idt::setEntry(0x24, irqHandler4, 0x08, 0x8E);
		idt::setEntry(0x25, irqHandler5, 0x08, 0x8E);
		idt::setEntry(0x26, irqHandler6, 0x08, 0x8E);
		idt::setEntry(0x27, irqHandler7, 0x08, 0x8E);
		idt::setEntry(0x28, irqHandler8, 0x08, 0x8E);
		idt::setEntry(0x29, irqHandler9, 0x08, 0x8E);
		idt::setEntry(0x2A, irqHandlerA, 0x08, 0x8E);
		idt::setEntry(0x2B, irqHandlerB, 0x08, 0x8E);
		idt::setEntry(0x2C, irqHandlerC, 0x08, 0x8E);
		idt::setEntry(0x2D, irqHandlerD, 0x08, 0x8E);
		idt::setEntry(0x2E, irqHandlerE, 0x08, 0x8E);
		idt::setEntry(0x2F, irqHandlerF, 0x08, 0x8E);
		// Set IDT size and data pointer
		idt::pointer.size	= idt::calcSize();
		idt::pointer.pointer	= idt::table;
		// Load new IDT
		idtLoad(&pointer);
	}


}	// namespace arch


#endif	// IGROS_ARCH_IDT_HPP

