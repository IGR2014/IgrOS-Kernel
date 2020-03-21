////////////////////////////////////////////////////////////////
//
//	Keyboard generic handling
//
//	File:	keyboard.cpp
//	Date:	14 Feb 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#include <port.hpp>
#include <irq.hpp>

#include <drivers/vmem.hpp>

#include <klib/kprint.hpp>


// Arch-dependent code zone
namespace arch {


	// Keyboard interrupt (#1) handler
	void keyboardInterruptHandler(const taskRegs_t* regs) {

		// Check keyboard status
		auto keyStatus = outPort8(KEYBOARD_CONTROL);
		// Check keyboard data port
		if (keyStatus & 0x01) {
			// Read keyboard data
			auto keyCode = outPort8(KEYBOARD_DATA);
			klib::kprintf(	u8"IRQ #%d\t[Keyboard]\r\n"
					u8"Key:\t%s\r\n"
					u8"Code:\t0x%x\r\n",
					arch::irqNumber_t::KEYBOARD,
					(keyCode > 0x80) ? u8"RELEASED" : u8"PRESSED",
					keyCode);
		}

	}


	// Setip keyboard function
	void keyboardSetup() {

		// Install keyboard interrupt handler
		irq::install(arch::irqNumber_t::KEYBOARD, arch::keyboardInterruptHandler);
		// Mask Keyboard interrupts
		irq::mask(arch::irqNumber_t::KEYBOARD);

		klib::kprintf(	u8"IRQ #%d [Keyboard] installed",
				arch::irqNumber_t::KEYBOARD);

	}


}	// namespace arch

