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
	constexpr auto KEYBOARD_CONTROL	{static_cast<io::port_t>(0x0064_u16)};
	constexpr auto KEYBOARD_DATA	{static_cast<io::port_t>(0x0060_u16)};


	// Keyboard interrupt (#1) handler
	void keyboardInterruptHandler(const register_t* regs) {
		// Check keyboard data port
		if (const auto status = io::get().readPort8(KEYBOARD_CONTROL); status & 0x01_u8) [[likely]] {
			// Read keyboard data
			const auto keyCode = io::get().readPort8(KEYBOARD_DATA);
			klib::kprintf(
				"IRQ #%d\t[Keyboard]\n"
				"Key:\t%s\n"
				"Code:\t0x%x\n",
				irq::irq_t::KEYBOARD,
				(keyCode > 0x80_u8) ? "RELEASED" : "PRESSED",
				keyCode
			);
		}
		// IRQ EOI
		irq::get().eoi(static_cast<irq::irq_t>(regs->number));
	}


	// Setip keyboard function
	void keyboardSetup() {

		// Install keyboard interrupt handler
		irq::get().install<irq::irq_t::KEYBOARD, keyboardInterruptHandler>();
		// Mask Keyboard interrupts
		irq::get().mask(irq::irq_t::KEYBOARD);

	}


}	// namespace igros::arch

