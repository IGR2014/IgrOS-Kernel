////////////////////////////////////////////////////////////////
//
//	Keyboard generic handling
//
//	File:	keyboard.cpp
//	Date:	10 Oct 2019
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#include <arch/port.hpp>
#include <arch/irq.hpp>

#include <drivers/vmem.hpp>

#include <klib/kprint.hpp>


// Arch-dependent code zone
namespace arch {


	// Keyboard interrupt (#1) handler
	void keyboardInterruptHandler(const taskRegs_t* regs) {

		// Check keyboard status
		byte_t keyStatus = inPort8(KEYBOARD_CONTROL);
		// Check keyboard data port
		if (keyStatus & 0x01) {

			// Read keyboard data
			byte_t keyCode = inPort8(KEYBOARD_DATA);
			// Print buffer
			sbyte_t text[1024];
			klib::ksprintf(	text,
					"IRQ #%d\t[Keyboard]\r\n"
					"Key:\t%s\r\n"
					"Code:\t0x%x\r\n"
					"\r\n",
					arch::irqNumber_t::KEYBOARD,
					(keyCode > 0x80) ? "RELEASED" : "PRESSED",
					keyCode);
			arch::vmemWrite(text);

		}

	}


	// Setip keyboard function
	void keyboardSetup() {

		// Install keyboard interrupt handler
		irqHandlerInstall(arch::irqNumber_t::KEYBOARD, arch::keyboardInterruptHandler);
		// Mask Keyboard interrupts
		irqMask(arch::irqNumber_t::KEYBOARD);

		// Print buffer
		sbyte_t text[1024];
		klib::ksprintf(	text,
				"IRQ #%d [Keyboard] installed\r\n",
				arch::irqNumber_t::KEYBOARD);
		arch::vmemWrite(text);

	}


}	// namespace arch

