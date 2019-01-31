////////////////////////////////////////////////////////////////
//
//	Keyboard generic handling
//
//	File:	keyboard.cpp
//	Date:	23 Jun. 2018
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
//	All rights reserved.
//
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

		byte_t keyStatus = inPort8(KEYBOARD_CONTROL);

		// Check keyboard data port
		if (keyStatus & 0x01) {

			byte_t keyCode = inPort8(KEYBOARD_DATA);

			if (keyCode > 0x80) {

				vgaConsoleWriteLine("KEY_RELEASED");

			} else {

				vgaConsoleWriteLine("KEY_PRESSED");

			}

			vgaConsoleWrite("Key CODE: ");
			vgaConsoleWriteHex(keyCode);
			vgaConsoleWriteLine("\r\n");

		}

	}


}	// namespace arch

