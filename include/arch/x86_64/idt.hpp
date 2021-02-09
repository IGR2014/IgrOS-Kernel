////////////////////////////////////////////////////////////////
//
//	Interrupt descriptor table low-level operations
//
//	File:	idt.hpp
//	Date:	07 Feb 2021
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <cstdint>
#include <array>
#include <type_traits>

#include <arch/x86_64/types.hpp>
#include <arch/x86_64/irq.hpp>
#include <arch/x86_64/exceptions.hpp>


// x86_64 namespace
namespace igros::x86_64 {


#pragma pack(push, 1)

	// IDT entry
	struct idtEntryx86_64_t {
		word_t		offsetLow;		// Offset lower part (0..15)
		word_t		selector;		// Selectod from GDT/LDT
		byte_t		ist;			// Interrupt service table
		byte_t		type;			// Type attributes
		word_t		offsetMiddle;		// Offset middle part (16..31)
		dword_t		offsetHigh;		// Offset higher part (32..63)
		dword_t		reserved2;		// Must be zero
	};

	// IDT pointer
	struct idtPointerx86_64_t {
		word_t			size;		// IDT size
		const idtEntryx86_64_t*	pointer;	// IDT pointer
	};

#pragma pack(pop)


}	// namespace igros::x86_64


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus

	// Load IDT
	inline void					idtLoad(const igros::x86_64::idtPointerx86_64_t* const idtPtr) noexcept;
	// Store IDT
	inline const igros::x86_64::idtPointerx86_64_t*	idtStore() noexcept;

#ifdef	__cplusplus

}

#endif	// __cplusplus


// x86_64 namespace
namespace igros::x86_64 {


	// IDT structure
	class idtx86_64 final {

		// IDT ISR pointer
		using isrPointerx86_64_t = std::add_pointer_t<void()>;

		// Number of IDT entries
		constexpr static auto				IDT_SIZE = 256ULL;

		// Exceptions and IRQ descriptors table (IDT)
		static std::array<idtEntryx86_64_t, IDT_SIZE>	table;
		// Pointer to IDT
		static idtPointerx86_64_t			pointer;


		// Copy c-tor
		idtx86_64(const idtx86_64 &other) = delete;
		// Copy assignment
		idtx86_64& operator=(const idtx86_64 &other) = delete;

		// Move c-tor
		idtx86_64(idtx86_64 &&other) = delete;
		// Move assignment
		idtx86_64& operator=(idtx86_64 &&other) = delete;


	public:

		// Default c-tor
		idtx86_64() noexcept = default;

		// Set IDT entry
		static idtEntryx86_64_t		setEntry(const isrPointerx86_64_t offset, const word_t selector, const byte_t type) noexcept;
		// Calc IDT size
		[[nodiscard]]
		constexpr static dword_t	calcSize() noexcept;

		// Init IDT table
		static void	init() noexcept;


	};


	// Set IDT entry
	inline idtEntryx86_64_t idtx86_64::setEntry(const isrPointerx86_64_t offset, const word_t selector, const byte_t type) noexcept {
		return {
			.offsetLow	= static_cast<word_t>(reinterpret_cast<std::uintptr_t>(offset) & 0xFFFF),
			.selector	= selector,
			.ist		= 0x00,
			.type		= type,
			.offsetMiddle	= static_cast<word_t>((reinterpret_cast<std::uintptr_t>(offset) & 0xFFFF0000) >> 16),
			.offsetHigh	= static_cast<dword_t>((reinterpret_cast<std::uintptr_t>(offset) & 0xFFFFFFFF00000000) >> 32),
			.reserved2	= 0x00000000
		};
	}

	// Calculate IDT size
	constexpr dword_t idtx86_64::calcSize() noexcept {
		return (IDT_SIZE * sizeof(idtEntryx86_64_t)) - 1u;
	}


	// Init IDT table
	inline void idtx86_64::init() noexcept {
		// Exceptions setup
		table[0]  = idtx86_64::setEntry(::exHandler00, 0x08, 0x8E);
		table[1]  = idtx86_64::setEntry(::exHandler01, 0x08, 0x8E);
		table[2]  = idtx86_64::setEntry(::exHandler02, 0x08, 0x8E);
		table[3]  = idtx86_64::setEntry(::exHandler03, 0x08, 0x8E);
		table[4]  = idtx86_64::setEntry(::exHandler04, 0x08, 0x8E);
		table[5]  = idtx86_64::setEntry(::exHandler05, 0x08, 0x8E);
		table[6]  = idtx86_64::setEntry(::exHandler06, 0x08, 0x8E);
		table[7]  = idtx86_64::setEntry(::exHandler07, 0x08, 0x8E);
		table[8]  = idtx86_64::setEntry(::exHandler08, 0x08, 0x8E);
		table[9]  = idtx86_64::setEntry(::exHandler09, 0x08, 0x8E);
		table[10] = idtx86_64::setEntry(::exHandler0A, 0x08, 0x8E);
		table[11] = idtx86_64::setEntry(::exHandler0B, 0x08, 0x8E);
		table[12] = idtx86_64::setEntry(::exHandler0C, 0x08, 0x8E);
		table[13] = idtx86_64::setEntry(::exHandler0D, 0x08, 0x8E);
		table[14] = idtx86_64::setEntry(::exHandler0E, 0x08, 0x8E);
		table[15] = idtx86_64::setEntry(::exHandler0F, 0x08, 0x8E);
		table[16] = idtx86_64::setEntry(::exHandler10, 0x08, 0x8E);
		table[17] = idtx86_64::setEntry(::exHandler11, 0x08, 0x8E);
		table[18] = idtx86_64::setEntry(::exHandler12, 0x08, 0x8E);
		table[19] = idtx86_64::setEntry(::exHandler13, 0x08, 0x8E);
		table[20] = idtx86_64::setEntry(::exHandler14, 0x08, 0x8E);
		table[21] = idtx86_64::setEntry(::exHandler15, 0x08, 0x8E);
		table[22] = idtx86_64::setEntry(::exHandler16, 0x08, 0x8E);
		table[23] = idtx86_64::setEntry(::exHandler17, 0x08, 0x8E);
		table[24] = idtx86_64::setEntry(::exHandler18, 0x08, 0x8E);
		table[25] = idtx86_64::setEntry(::exHandler19, 0x08, 0x8E);
		table[26] = idtx86_64::setEntry(::exHandler1A, 0x08, 0x8E);
		table[27] = idtx86_64::setEntry(::exHandler1B, 0x08, 0x8E);
		table[28] = idtx86_64::setEntry(::exHandler1C, 0x08, 0x8E);
		table[29] = idtx86_64::setEntry(::exHandler1D, 0x08, 0x8E);
		table[30] = idtx86_64::setEntry(::exHandler1E, 0x08, 0x8E);
		table[31] = idtx86_64::setEntry(::exHandler1F, 0x08, 0x8E);
		// IRQs setup
		table[32] = idtx86_64::setEntry(::irqHandler0, 0x08, 0x8E);
		table[33] = idtx86_64::setEntry(::irqHandler1, 0x08, 0x8E);
		table[34] = idtx86_64::setEntry(::irqHandler2, 0x08, 0x8E);
		table[35] = idtx86_64::setEntry(::irqHandler3, 0x08, 0x8E);
		table[36] = idtx86_64::setEntry(::irqHandler4, 0x08, 0x8E);
		table[37] = idtx86_64::setEntry(::irqHandler5, 0x08, 0x8E);
		table[38] = idtx86_64::setEntry(::irqHandler6, 0x08, 0x8E);
		table[39] = idtx86_64::setEntry(::irqHandler7, 0x08, 0x8E);
		table[40] = idtx86_64::setEntry(::irqHandler8, 0x08, 0x8E);
		table[41] = idtx86_64::setEntry(::irqHandler9, 0x08, 0x8E);
		table[42] = idtx86_64::setEntry(::irqHandlerA, 0x08, 0x8E);
		table[43] = idtx86_64::setEntry(::irqHandlerB, 0x08, 0x8E);
		table[44] = idtx86_64::setEntry(::irqHandlerC, 0x08, 0x8E);
		table[45] = idtx86_64::setEntry(::irqHandlerD, 0x08, 0x8E);
		table[46] = idtx86_64::setEntry(::irqHandlerE, 0x08, 0x8E);
		table[47] = idtx86_64::setEntry(::irqHandlerF, 0x08, 0x8E);
		// Set IDT size and data pointer
		idtx86_64::pointer = {
			idtx86_64::calcSize(),
			idtx86_64::table.cbegin()
		};
		// Load new IDT
		::idtLoad(&pointer);
	}


}	// namespace igros::x86_64

