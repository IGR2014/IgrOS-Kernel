////////////////////////////////////////////////////////////////
//
//	Interrupt service routines low-level operations
//
//	File:	isr.cpp
//	Date:	10 Oct 2019
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#include <arch/taskRegs.hpp>
#include <arch/isr.hpp>
#include <arch/port.hpp>

#include <drivers/vmem.hpp>

#include <klib/kprint.hpp>


// Arch-dependent code zone
namespace arch {


	// Interrupt handlers
	static isrHandler_t isrList[256] = {};

	 // Print buffer
	sbyte_t text[1024];

	// Interrupts handler function
	void isrHandler(const taskRegs_t* regs) {

		// Acquire irq handler from list
		auto isr = isrList[regs->number];

		// It`s an interrupt
		if (regs->number >= IRQ_OFFSET) {
			// Notify slave PIC if needed
			if (regs->number > 39) {
				outPort8(PIC_SLAVE_CONTROL, 0x20);
			} else {
				// Notify master PIC
				outPort8(PIC_MASTER_CONTROL, 0x20);
			}
		// Otherwise it's exception
		} else {
			// Check if exception handler installed
			if (nullptr == isr) {
				// Hang CPU
				while (true) {};
			}
		}

		// Check if exception handler installed
		if (nullptr != isr) {
			isr(regs);
		} else {
			// Debug
			klib::ksprintf(  text,
					"%s -> [#%d]\r\n"
					"State:\t\tUNHANDLED! CPU halted!\r\n\r\n",
					((regs->number >= IRQ_OFFSET) ? "IRQ" : "EXCEPTION"),
					((regs->number >= IRQ_OFFSET) ? (regs->number - IRQ_OFFSET) : regs->number));
			vmemWrite(text);
		}

	}


	// Install interrupt service routine handler
	void isrHandlerInstall(const dword_t number, const isrHandler_t handler) {
		// Put interrupt service routine handler in ISRs list
		isrList[number] = handler;
	}

	// Uninstall interrupt service routine handler
	void isrHandlerUninstall(const dword_t number) {
		// Remove interrupt service routine handler from ISRs list
		isrList[number] = nullptr;
	}


}	// namespace arch

