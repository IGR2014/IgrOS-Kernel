////////////////////////////////////////////////////////////////
//
//	Interrupt descriptor table tests
//
//	File:	test_idt.cpp
//	Date:	29 Oct 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


// C++
#include <bit>
#include <cstdint>
// IgrOS-Kernel arch x86
#include <arch/x86/exceptions.hpp>
#include <arch/x86/irq.hpp>
#include <arch/x86/native.hpp>
#if	defined (IGROS_ARCH_i386)
#include <arch/i386/gdt.hpp>
#include <arch/i386/idt.hpp>
#elif	defined (IGROS_ARCH_x86_64)
#include <arch/x86_64/gdt.hpp>
#include <arch/x86_64/idt.hpp>
#include <arch/x86_64/tss.hpp>
#endif
// Tests
#include <tests/hw.hpp>


using namespace igros;
using gdt = x86::native::gdt;
using idt = x86::native::idt;
namespace hw = igros::test::hw;


// Present, ring 0, 32/64-bit interrupt gate
constexpr auto INTERRUPT_GATE	{std::uint8_t {0x8E}};
// Exception vectors
[[maybe_unused]] constexpr auto NMI		{2U};
constexpr auto DOUBLE_FAULT	{8U};
[[maybe_unused]] constexpr auto MACHINE_CHECK	{18U};


// IDT handed to CPU by lidt
[[nodiscard]]
static auto loadedIdt() -> const idt::pointer_t& {
	return *static_cast<const idt::pointer_t*>(hw::idtPointer);
}

// Handler address of gate
[[nodiscard]]
static auto offset(const idt::entry_t &entry) -> std::uintptr_t {
#if	defined (IGROS_ARCH_i386)
	return static_cast<std::uintptr_t>(entry.offsetLow) | (static_cast<std::uintptr_t>(entry.offsetHigh) << 16);
#elif	defined (IGROS_ARCH_x86_64)
	return static_cast<std::uintptr_t>(entry.offsetLow) | (static_cast<std::uintptr_t>(entry.offsetMiddle) << 16) | (static_cast<std::uintptr_t>(entry.offsetHigh) << 32);
#endif
}

// Kernel interrupt gate to handler
static void checkGate(const idt::entry_t &entry, const std::add_pointer_t<void () noexcept> handler) {
	IGROS_CHECK(std::bit_cast<std::uintptr_t>(handler) == offset(entry));
	IGROS_CHECK(gdt::KERNEL_CODE == entry.selector);
	IGROS_CHECK(INTERRUPT_GATE == entry.type);
}


// Exceptions at vectors 0..31, IRQs at 32..47, the rest not present
static void testInit() {
	hw::reset();
	idt::init();

	IGROS_CHECK(nullptr != hw::idtPointer);
	const auto &pointer {loadedIdt()};
	IGROS_CHECK(256 * sizeof(idt::entry_t) - 1 == pointer.size);
	const auto* const table {pointer.pointer};

	// Exceptions
	for (auto vector {0U}; vector < 32U; ++vector) {
#if	defined (IGROS_ARCH_i386)
		if (DOUBLE_FAULT == vector) {
			continue;
		}
#endif
		checkGate(table[vector], ::exHandlerTable[vector]);
	}
	// IRQs
	for (auto line {0U}; line < 16U; ++line) {
		checkGate(table[x86::IRQ_OFFSET + line], ::irqHandlerTable[line]);
	}
	// Unused vectors: not present
	auto empty {true};
	for (auto vector {x86::IRQ_OFFSET + 16U}; vector < 256U; ++vector) {
		empty = empty && (0 == table[vector].type) && (0 == offset(table[vector]));
	}
	IGROS_CHECK(empty);

#if	defined (IGROS_ARCH_i386)
	// Double fault: task gate to double fault task, no handler address
	IGROS_CHECK(gdt::TSS_DOUBLE_FAULT == table[DOUBLE_FAULT].selector);
	IGROS_CHECK(0x85 == table[DOUBLE_FAULT].type);
	IGROS_CHECK(0 == offset(table[DOUBLE_FAULT]));
	IGROS_CHECK(0 == table[DOUBLE_FAULT].reserved);
#elif	defined (IGROS_ARCH_x86_64)
	// Interrupt stacks for exceptions that can't trust current stack
	using IST = x86_64::tss::IST;
	IGROS_CHECK(static_cast<std::uint8_t>(IST::DOUBLE_FAULT) == table[DOUBLE_FAULT].ist);
	IGROS_CHECK(static_cast<std::uint8_t>(IST::NMI) == table[NMI].ist);
	IGROS_CHECK(static_cast<std::uint8_t>(IST::MACHINE_CHECK) == table[MACHINE_CHECK].ist);
	auto others {true};
	for (auto vector {0U}; vector < x86::IRQ_OFFSET + 16U; ++vector) {
		if ((DOUBLE_FAULT != vector) && (NMI != vector) && (MACHINE_CHECK != vector)) {
			others = others && (0 == table[vector].ist) && (0 == table[vector].reserved2);
		}
	}
	IGROS_CHECK(others);
#endif
}


int main() {

	testInit();

	return igros::test::result();

}
