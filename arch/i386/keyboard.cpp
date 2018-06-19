////////////////////////////////////////////////////////////////
//
//	Keyboard generic handling
//
//	File:	keyboard.cpp
//	Date:	19 Jun. 2018
//
//	Copyright (c) 2018, Igor Baklykov
//	All rights reserved.
//


#include <include/port.hpp>
#include <include/interrupts.hpp>
#include <include/videoMem.hpp>


// Arch-dependent code zone
namespace arch {


	// Keyboard interrupt (#1) handler
	void keyboardInterruptHandler(const taskRegs* regs) {

		videoMemWrite("Keyboard -> IRQ\t");

		// Read keyboard data port
		if (inPortB(0x60) > 0x7F) {

			videoMemWriteLine("KEY_UP");

			return;

		}

		videoMemWriteLine("KEY_DOWN");

	}


}	// namespace arch

