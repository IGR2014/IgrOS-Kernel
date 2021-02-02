////////////////////////////////////////////////////////////////
//
//	Keyboard generic handling
//
//	File:	keyboard.cpp
//	Date:	01 Feb 2021
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#include <arch/types.hpp>
#include <arch/io.hpp>
#include <arch/irq.hpp>
#include <arch/register.hpp>

#include <klib/kprint.hpp>


// Arch-dependent code zone
namespace igros::arch {


	// Keyboard ports
	constexpr auto KEYBOARD_CONTROL	= static_cast<io::port_t>(0x0064);
	constexpr auto KEYBOARD_DATA	= static_cast<io::port_t>(0x0060);


	// Keyboard interrupt (#1) handler
	void keyboardInterruptHandler(const register_t* regs) {
		// Check keyboard status
		const auto keyStatus = io::get().readPort8(KEYBOARD_CONTROL);
		// Check keyboard data port
		if (keyStatus & 0x01) {
			// Read keyboard data
			const auto keyCode = io::get().readPort8(KEYBOARD_DATA);
			klib::kprintf(
				u8"IRQ #%d\t[Keyboard]\r\n"
				u8"Key:\t%s\r\n"
				u8"Code:\t0x%x\r\n",
				irq::irq_t::KEYBOARD,
				(keyCode > 0x80) ? u8"RELEASED" : u8"PRESSED",
				keyCode
			);
		}
		// IRQ EOI
		irq::get().eoi(static_cast<irq::irq_t>(regs->number));
	}


	// Setip keyboard function
	void keyboardSetup() {

		// Install keyboard interrupt handler
		irq::get().install(irq::irq_t::KEYBOARD, keyboardInterruptHandler);
		// Mask Keyboard interrupts
		irq::get().mask(irq::irq_t::KEYBOARD);

		klib::kprintf(
			u8"IRQ #%d [Keyboard] installed",
			irq::irq_t::KEYBOARD
		);

	}


}	// namespace igros::arch

