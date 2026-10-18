////////////////////////////////////////////////////////////////
//
//	Host stubs for kernel assembly functions (fake hardware, see hw.hpp)
//
//	File:	cpu.cpp
//	Date:	19 Oct 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


// C++
#include <cstdlib>
// IgrOS-Kernel arch
#include <arch/cpu.hpp>
#include <arch/x86/cr.hpp>
#include <arch/x86/io.hpp>
#include <arch/x86/irq.hpp>
// Tests
#include <tests/hw.hpp>


namespace hw = igros::test::hw;


// Read port (device model or register value)
[[nodiscard]]
static auto portRead(const igros::x86::port_t addr) -> std::uint32_t {
	const auto value {hw::portReadHook ? hw::portReadHook(addr) : hw::portValue[addr]};
	hw::portLog.push_back({addr, value, false});
	return value;
}

// Write port
static void portWrite(const igros::x86::port_t addr, const std::uint32_t value) {
	hw::portValue[addr] = value;
	hw::portLog.push_back({addr, value, true});
}


extern "C" {


	// Port I/O
	auto portRead8(const igros::x86::port_t addr) noexcept -> igros::igros_byte_t {
		return static_cast<igros::igros_byte_t>(portRead(addr));
	}

	auto portRead16(const igros::x86::port_t addr) noexcept -> igros::igros_word_t {
		return static_cast<igros::igros_word_t>(portRead(addr));
	}

	auto portRead32(const igros::x86::port_t addr) noexcept -> igros::igros_dword_t {
		return static_cast<igros::igros_dword_t>(portRead(addr));
	}

	void portWrite8(const igros::x86::port_t addr, const igros::igros_byte_t value) noexcept {
		portWrite(addr, value);
	}

	void portWrite16(const igros::x86::port_t addr, const igros::igros_word_t value) noexcept {
		portWrite(addr, value);
	}

	void portWrite32(const igros::x86::port_t addr, const igros::igros_dword_t value) noexcept {
		portWrite(addr, value);
	}


	// Interrupts flag
	void irqEnable() noexcept {
		hw::irqEnabled = true;
	}

	void irqDisable() noexcept {
		hw::irqEnabled = false;
	}

	auto irqSave() noexcept -> igros::igros_usize_t {
		const auto flags {static_cast<igros::igros_usize_t>(hw::irqEnabled)};
		hw::irqEnabled = false;
		return flags;
	}

	void irqRestore(const igros::igros_usize_t flags) noexcept {
		hw::irqEnabled = (0 != flags);
	}


	// Control registers
	auto readCR2() noexcept -> igros::igros_usize_t {
		return static_cast<igros::igros_usize_t>(hw::cr2);
	}

	auto readCR3() noexcept -> igros::igros_usize_t {
		return static_cast<igros::igros_usize_t>(hw::cr3);
	}

	void writeCR3(const igros::igros_usize_t value) noexcept {
		hw::cr3 = value;
	}

	void pageInvalidate(const void* const page) noexcept {
		hw::invalidated.push_back(page);
	}


	// Halt returns to IGROS_CHECK_HALTS (unexpected halt fails the test)
	[[noreturn]]
	void cpuHalt() noexcept {
		if (hw::haltArmed) {
			std::longjmp(hw::haltJump, 1);
		}
		std::printf("unexpected CPU halt\n");
		std::abort();
	}


}	// extern "C"

