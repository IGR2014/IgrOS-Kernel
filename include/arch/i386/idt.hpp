////////////////////////////////////////////////////////////////
//
//	Interrupt descriptor table low-level operations
//
//	File:	idt.hpp
//	Date:	30 Jun 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <type_traits>

#include <types.hpp>
#include <irq.hpp>
#include <exceptions.hpp>


// Arch-dependent code zone
namespace igros::arch {


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
		inline static idtEntry_t	setEntry(const isrPointer_t offset, const word_t selector, const byte_t type) noexcept;
		// Calc IDT size
		[[nodiscard]] constexpr static word_t	calcSize() noexcept;

		// Init IDT table
		inline static void	init() noexcept;


	};


	// Set IDT entry
	inline idtEntry_t idt::setEntry(const isrPointer_t offset, const word_t selector, const byte_t type) noexcept {
		return {
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
	inline void idt::init() noexcept {
		// Exceptions setup
		table[0]  = idt::setEntry(exHandler00, 0x08, 0x8E);
		table[1]  = idt::setEntry(exHandler01, 0x08, 0x8E);
		table[2]  = idt::setEntry(exHandler02, 0x08, 0x8E);
		table[3]  = idt::setEntry(exHandler03, 0x08, 0x8E);
		table[4]  = idt::setEntry(exHandler04, 0x08, 0x8E);
		table[5]  = idt::setEntry(exHandler05, 0x08, 0x8E);
		table[6]  = idt::setEntry(exHandler06, 0x08, 0x8E);
		table[7]  = idt::setEntry(exHandler07, 0x08, 0x8E);
		table[8]  = idt::setEntry(exHandler08, 0x08, 0x8E);
		table[9]  = idt::setEntry(exHandler09, 0x08, 0x8E);
		table[10] = idt::setEntry(exHandler0A, 0x08, 0x8E);
		table[11] = idt::setEntry(exHandler0B, 0x08, 0x8E);
		table[12] = idt::setEntry(exHandler0C, 0x08, 0x8E);
		table[13] = idt::setEntry(exHandler0D, 0x08, 0x8E);
		table[14] = idt::setEntry(exHandler0E, 0x08, 0x8E);
		table[15] = idt::setEntry(exHandler0F, 0x08, 0x8E);
		table[16] = idt::setEntry(exHandler10, 0x08, 0x8E);
		table[17] = idt::setEntry(exHandler11, 0x08, 0x8E);
		table[18] = idt::setEntry(exHandler12, 0x08, 0x8E);
		table[19] = idt::setEntry(exHandler13, 0x08, 0x8E);
		table[20] = idt::setEntry(exHandler14, 0x08, 0x8E);
		table[21] = idt::setEntry(exHandler15, 0x08, 0x8E);
		table[22] = idt::setEntry(exHandler16, 0x08, 0x8E);
		table[23] = idt::setEntry(exHandler17, 0x08, 0x8E);
		table[24] = idt::setEntry(exHandler18, 0x08, 0x8E);
		table[25] = idt::setEntry(exHandler19, 0x08, 0x8E);
		table[26] = idt::setEntry(exHandler1A, 0x08, 0x8E);
		table[27] = idt::setEntry(exHandler1B, 0x08, 0x8E);
		table[28] = idt::setEntry(exHandler1C, 0x08, 0x8E);
		table[29] = idt::setEntry(exHandler1D, 0x08, 0x8E);
		table[30] = idt::setEntry(exHandler1E, 0x08, 0x8E);
		table[31] = idt::setEntry(exHandler1F, 0x08, 0x8E);
		// IRQs setup
		table[32] = idt::setEntry(irqHandler0, 0x08, 0x8E);
		table[33] = idt::setEntry(irqHandler1, 0x08, 0x8E);
		table[34] = idt::setEntry(irqHandler2, 0x08, 0x8E);
		table[35] = idt::setEntry(irqHandler3, 0x08, 0x8E);
		table[36] = idt::setEntry(irqHandler4, 0x08, 0x8E);
		table[37] = idt::setEntry(irqHandler5, 0x08, 0x8E);
		table[38] = idt::setEntry(irqHandler6, 0x08, 0x8E);
		table[39] = idt::setEntry(irqHandler7, 0x08, 0x8E);
		table[40] = idt::setEntry(irqHandler8, 0x08, 0x8E);
		table[41] = idt::setEntry(irqHandler9, 0x08, 0x8E);
		table[42] = idt::setEntry(irqHandlerA, 0x08, 0x8E);
		table[43] = idt::setEntry(irqHandlerB, 0x08, 0x8E);
		table[44] = idt::setEntry(irqHandlerC, 0x08, 0x8E);
		table[45] = idt::setEntry(irqHandlerD, 0x08, 0x8E);
		table[46] = idt::setEntry(irqHandlerE, 0x08, 0x8E);
		table[47] = idt::setEntry(irqHandlerF, 0x08, 0x8E);
		// Set IDT size and data pointer
		idt::pointer.size	= idt::calcSize();
		idt::pointer.pointer	= idt::table;
		// Load new IDT
		idtLoad(&pointer);
	}


}	// namespace igros::arch

