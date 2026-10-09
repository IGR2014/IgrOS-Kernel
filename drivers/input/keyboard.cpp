////////////////////////////////////////////////////////////////
//
//	Keyboard generic handling
//
//	File:	keyboard.cpp
//	Date:	12 Mar 2023
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


// IgrOS-Kernel arch
#include <arch/io.hpp>
#include <arch/irq.hpp>
#include <arch/register.hpp>
#include <arch/types.hpp>
// IgrOS-Kernel library
#include <klib/kprint.hpp>


// Arch-dependent code zone
namespace igros::arch {


	// Keyboard ports
	constexpr auto KEYBOARD_CONTROL	{static_cast<port_t>(0x0064_u16)};
	constexpr auto KEYBOARD_DATA	{static_cast<port_t>(0x0060_u16)};


	// Keyboard interrupt (#1) handler
	static void keyboardInterruptHandler([[maybe_unused]] const register_t* regs) noexcept {
		// Check keyboard data port
		if (const auto status = io::readPort8(KEYBOARD_CONTROL); status & 0x01_u8) [[likely]] {
			// Read keyboard data
			const auto keyCode = io::readPort8(KEYBOARD_DATA);
			klib::kprintf(
				"IRQ #%d\t[Keyboard]\n"
				"Key:\t%s\n"
				"Code:\t0x%x\n",
				irq_t::KEYBOARD,
				(0x00_u8 != (keyCode & 0x80_u8)) ? "RELEASED" : "PRESSED",
				keyCode
			);
		}
	}


	// Setip keyboard function
	void keyboardSetup() {

		// Install keyboard interrupt handler
		irq::install<irq_t::KEYBOARD, keyboardInterruptHandler>();
		// Unmask Keyboard interrupts
		irq::unmask(irq_t::KEYBOARD);

	}


}	// namespace igros::arch

