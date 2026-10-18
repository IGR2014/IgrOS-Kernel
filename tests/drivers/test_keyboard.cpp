////////////////////////////////////////////////////////////////
//
//	PS/2 keyboard driver tests
//
//	File:	test_keyboard.cpp
//	Date:	19 Oct 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


// IgrOS-Kernel arch
#include <arch/x86/irq.hpp>
// IgrOS-Kernel drivers
#include <drivers/input/keyboard.hpp>
// Tests
#include <tests/hw.hpp>


// Common ISR entry (called by assembly stubs)
extern "C" void isrHandler(const igros::x86::register_t* regs) noexcept;


namespace hw = igros::test::hw;


// Keyboard controller ports
constexpr auto KEYBOARD_DATA	{std::uint16_t {0x60}};
constexpr auto KEYBOARD_STATUS	{std::uint16_t {0x64}};


// Keyboard interrupt with given controller state
static void keyboardIRQ(const std::uint8_t status, const std::uint8_t scanCode) {
	hw::console.clear();
	hw::portValue[KEYBOARD_STATUS]	= status;
	hw::portValue[KEYBOARD_DATA]	= scanCode;
	auto regs {igros::x86::register_t {}};
	regs.number = static_cast<decltype(regs.number)>(igros::x86::irqVector(igros::x86::irq_t::KEYBOARD));
	isrHandler(&regs);
}


int main() {

	// Setup unmasks IRQ 1
	hw::reset();
	hw::portValue[igros::x86::PIC_MASTER_DATA]	= 0xFF;
	hw::portValue[igros::x86::PIC_SLAVE_DATA]	= 0xFF;
	IGROS_CHECK(igros::arch::keyboardSetup());
	IGROS_CHECK(0xFD == hw::portValue[igros::x86::PIC_MASTER_DATA]);

	// Key press and release (bit 7) scan codes
	keyboardIRQ(0x01, 0x1E);
	IGROS_CHECK(hw::printed("[Keyboard]"));
	IGROS_CHECK(hw::printed("Key:\tPRESSED"));
	IGROS_CHECK(hw::printed("Code:\t0x1e"));
	keyboardIRQ(0x01, 0x9E);
	IGROS_CHECK(hw::printed("Key:\tRELEASED"));
	IGROS_CHECK(hw::printed("Code:\t0x9e"));

	// Output buffer empty: nothing read
	keyboardIRQ(0x00, 0x1E);
	IGROS_CHECK(hw::console.empty());

	return igros::test::result();

}

