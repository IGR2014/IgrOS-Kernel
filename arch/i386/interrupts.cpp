////////////////////////////////////////////////////////////////
//
//	Interrupts low-level operations
//
//	File:	interupts.cpp
//	Date:	18 Jun. 2018
//
//	Copyright (c) 2018, Igor Baklykov
//	All rights reserved.
//



#include <include/taskRegs.hpp>
#include <include/interrupts.hpp>
#include <include/port.hpp>
#include <include/videoMem.hpp>


// Arch-dependent code zone
namespace arch {


	// interrupts handler function
	void irqHandler(const taskRegs* regs) {

		// Notify slave PIC if needed
		if (regs->number > 39) {

			outPortB(0xA0, 0x20);

		}
		// Notify master PIC
		outPortB(0x20, 0x20);

		// Read keyboard data port
		if (inPortB(0x60) > 0x7F) {

			return;

		}

		// Write text to screen
		videoMemWrite("IRQ #");
		videoMemWriteDec(regs->number - 32);
		videoMemWriteLine("");

	}


}	// namespace arch

