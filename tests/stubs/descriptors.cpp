////////////////////////////////////////////////////////////////
//
//	Host stubs for descriptor table assembly (lgdt, lidt, ltr) and
//	interrupt entry points tables (see hw.hpp)
//
//	File:	descriptors.cpp
//	Date:	29 Oct 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


// C++
#include <array>
#include <cstddef>
#include <utility>
// IgrOS-Kernel arch
#include <arch/types.hpp>
// IgrOS-Kernel arch x86
#include <arch/x86/exceptions.hpp>
#include <arch/x86/irq.hpp>
// Tests
#include <tests/hw.hpp>


namespace hw = igros::test::hw;


namespace {


	// Distinct entry point per vector (only addresses matter)
	template<std::size_t VECTOR>
	void entry() noexcept {}

	// Table of N entry points starting at vector FIRST
	template<std::size_t FIRST, std::size_t... INDEX>
	consteval auto entries(std::index_sequence<INDEX...> /*unused*/) noexcept {
		return std::array<std::add_pointer_t<void () noexcept>, sizeof...(INDEX)> {&entry<FIRST + INDEX>...};
	}

	constexpr auto EXCEPTIONS	{entries<0>(std::make_index_sequence<32> {})};
	constexpr auto IRQS		{entries<32>(std::make_index_sequence<16> {})};


}	// namespace


extern "C" {


	// Interrupt entry points (exceptions.s, irq.s)
	const std::add_pointer_t<void () noexcept> exHandlerTable[32] {
		EXCEPTIONS[0],  EXCEPTIONS[1],  EXCEPTIONS[2],  EXCEPTIONS[3],  EXCEPTIONS[4],  EXCEPTIONS[5],  EXCEPTIONS[6],  EXCEPTIONS[7],
		EXCEPTIONS[8],  EXCEPTIONS[9],  EXCEPTIONS[10], EXCEPTIONS[11], EXCEPTIONS[12], EXCEPTIONS[13], EXCEPTIONS[14], EXCEPTIONS[15],
		EXCEPTIONS[16], EXCEPTIONS[17], EXCEPTIONS[18], EXCEPTIONS[19], EXCEPTIONS[20], EXCEPTIONS[21], EXCEPTIONS[22], EXCEPTIONS[23],
		EXCEPTIONS[24], EXCEPTIONS[25], EXCEPTIONS[26], EXCEPTIONS[27], EXCEPTIONS[28], EXCEPTIONS[29], EXCEPTIONS[30], EXCEPTIONS[31]
	};
	const std::add_pointer_t<void () noexcept> irqHandlerTable[16] {
		IRQS[0],  IRQS[1],  IRQS[2],  IRQS[3],  IRQS[4],  IRQS[5],  IRQS[6],  IRQS[7],
		IRQS[8],  IRQS[9],  IRQS[10], IRQS[11], IRQS[12], IRQS[13], IRQS[14], IRQS[15]
	};


	// Descriptor table registers (pointer structures are static in kernel code)
	void gdtLoad(const void* const gdtPtr) noexcept {
		hw::gdtPointer = gdtPtr;
	}

	void idtLoad(const void* const idtPtr) noexcept {
		hw::idtPointer = idtPtr;
	}

	void tssLoad(const igros::igros_word_t selector) noexcept {
		hw::taskRegister = selector;
	}


}	// extern "C"
