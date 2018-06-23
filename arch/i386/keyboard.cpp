////////////////////////////////////////////////////////////////
//
//	Keyboard generic handling
//
//	File:	keyboard.cpp
//	Date:	23 Jun. 2018
//
//	Copyright (c) 2018, Igor Baklykov
//	All rights reserved.
//


#include <include/port.hpp>
#include <include/interrupts.hpp>
#include <include/vgaConsole.hpp>


// Arch-dependent code zone
namespace arch {


	// Keyboard interrupt (#1) handler
	void keyboardInterruptHandler(const taskRegs_t* regs) {

		vgaConsoleWriteLine("IRQ\t\t-> KEYBOARD");
		vgaConsoleWrite("KEY STATE:\t");

		// Read keyboard data port
		if (inPort8(KEYBOARD_CONTROL) > 0x7F) {

			vgaConsoleWriteLine("KEY_UP");
			vgaConsoleWriteLine("");

			return;

		}

		vgaConsoleWriteLine("KEY_DOWN");
		vgaConsoleWriteLine("");

	}


}	// namespace arch

