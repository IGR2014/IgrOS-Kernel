////////////////////////////////////////////////////////////////
//
//	Interrupt service routines low-level operations
//
//	File:	isr.cpp
//	Date:	11 Jul 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#include <arch/x86_64/register.hpp>
#include <arch/x86_64/irq.hpp>
#include <arch/x86_64/isr.hpp>
#include <arch/x86_64/port.hpp>
#include <arch/x86_64/cpu.hpp>

#include <klib/kprint.hpp>


// x86_64 namespace
namespace igros::x86_64 {


	// Interrupt handlers
	static isr_t isrList[ISR_SIZE] {nullptr};


	// Interrupts handler function
	void isrHandler(const register_t* regs) noexcept {

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
				cpuHalt();
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


	// Install interrupt service routine handler
	void isrHandlerInstall(const dword_t isrNumber, const isr_t isrHandler) noexcept{
		// Put interrupt service routine handler in ISRs list
		isrList[isrNumber] = isrHandler;
	}

	// Uninstall interrupt service routine handler
	void isrHandlerUninstall(const dword_t isrNumber) noexcept {
		// Remove interrupt service routine handler from ISRs list
		isrList[isrNumber] = nullptr;
	}


}	// namespace igros::x86_64

