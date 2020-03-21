////////////////////////////////////////////////////////////////
//
//	Interrupt service routines low-level operations
//
//	File:	isr.cpp
//	Date:	13 Feb 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#include <taskRegs.hpp>
#include <isr.hpp>
#include <port.hpp>

#include <klib/kprint.hpp>


// Arch-dependent code zone
namespace arch {


	// Interrupt handlers
	isrHandler_t isrList[ISR_SIZE] {};

	// Interrupts handler function
	void isrHandler(const taskRegs_t* regs) noexcept {

		// Acquire irq handler from list
		auto isr = isrList[regs->number];

		// It`s an interrupt
		if (regs->number >= IRQ_OFFSET) {
			// Notify slave PIC if needed
			if (regs->number > 39) {
				inPort8(PIC_SLAVE_CONTROL, 0x20);
			} else {
				// Notify master PIC
				inPort8(PIC_MASTER_CONTROL, 0x20);
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
			klib::kprintf(	u8"%s -> [#%d]\r\n"
					u8"State:\t\tUNHANDLED! CPU halted!\r\n",
					((regs->number >= IRQ_OFFSET) ? u8"IRQ" : u8"EXCEPTION"),
					((regs->number >= IRQ_OFFSET) ? (regs->number - IRQ_OFFSET) : regs->number));
		}

	}


}	// namespace arch

