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


	// Interrupts handler function
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


	// Init interrupts
	void irqInit() {

		// ICW 1 for PIC
		arch::outPortB(0x20, 0x11);
		arch::outPortB(0xA0, 0x11);

		// ICW 2 for PIC
		arch::outPortB(0x21, 0x20);
		arch::outPortB(0xA1, 0x28);

		// ICW 3 for PIC
		arch::outPortB(0x21, 0x04);
		arch::outPortB(0xA1, 0x02);

		// ICW 4 for PIC
		arch::outPortB(0x21, 0x01);
		arch::outPortB(0xA1, 0x01);
	
	}

	// Mask interrupts
	void irqMask(const t_u16 mask) {

		// Set Master controller mask
		arch::outPortB(0x21, static_cast<t_u8>(mask & 0xFF));
		// Set Slave controller mask
		arch::outPortB(0xA1, static_cast<t_u8>((mask & 0xFF00) >> 8));
	
	}


}	// namespace arch

