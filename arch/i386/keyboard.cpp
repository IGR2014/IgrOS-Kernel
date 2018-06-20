////////////////////////////////////////////////////////////////
//
//	Keyboard generic handling
//
//	File:	keyboard.cpp
//	Date:	21 Jun. 2018
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
	void keyboardInterruptHandler(const taskRegs_t* regs) {

		videoMemWriteLine("IRQ\t\t-> KEYBOARD");
		videoMemWrite("KEY STATE:\t");

		// Read keyboard data port
		if (inPort8(KEYBOARD_CONTROL) > 0x7F) {

			videoMemWriteLine("KEY_UP");
			videoMemWriteLine("");

			return;

		}

		videoMemWriteLine("KEY_DOWN");
		videoMemWriteLine("");

	}


}	// namespace arch

