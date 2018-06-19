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


	// Interrupt handlers
	static irqHandler_t	isrList[224] = {0};


	// Interrupts handler function
	void irqHandler(const taskRegs* regs) {

		// Notify slave PIC if needed
		if (regs->number > 39) {

			outPortB(0xA0, 0x20);

		}
		// Notify master PIC
		outPortB(0x20, 0x20);

		// Actually it`s an exception and normaly shouldn't be there
		if (regs->number < 32) {

			return;

		}

		// Acquire irq handler from list
		irqHandler_t isr = isrList[regs->number - 32];

		// If handler exists
		if (isr) {

			// Finally handle IRQ
			isr(regs);

		} else {

			// Print message about unhandled interrupt
			videoMemWrite("IRQ #");
			videoMemWriteDec(regs->number - 32);
			videoMemWriteLine(" unhandled!");
		
		}

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

		// Unmask all interrupts
		irqMaskSet(0xFFFF);

	}

	// Mask interrupts
	void irqMask(const IRQ_NUMBER irq) {

		// Chech if it's hardware interrupt
		if (irq < 16) {

			// Set interrupts mask
			irqMaskSet(~(1 << irq));

		}

	}

	// Set interrupts mask
	void irqMaskSet(const t_u16 mask) {

		// Set Master controller mask
		arch::outPortB(0x21, static_cast<t_u8>(mask & 0xFF));
		// Set Slave controller mask
		arch::outPortB(0xA1, static_cast<t_u8>((mask & 0xFF00) >> 8));

	}

	// Install handler
	void irqHandlerInstall(IRQ_NUMBER irqNumber, irqHandler_t handler) {

		isrList[irqNumber] = handler;

	}

	// Uninstall handler
	void irqHandlerUninstall(IRQ_NUMBER irqNumber) {

		isrList[irqNumber] = nullptr;

	}


}	// namespace arch

