////////////////////////////////////////////////////////////////
//
//	Interrupt service routines low-level operations
//
//	File:	isr.cpp
//	Date:	12 Mar 2023
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


// IgrOS-Kernel arch x86_64
#include <arch/x86_64/cpu.hpp>
#include <arch/x86_64/io.hpp>
#include <arch/x86_64/irq.hpp>
#include <arch/x86_64/isr.hpp>
#include <arch/x86_64/register.hpp>
// IgrOS-Kernel library
#include <klib/kprint.hpp>


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus

	// Interrupts handler function
	void isrHandler(const igros::x86_64::register_t* regs) noexcept {
		// Check if irq/exception handler installed
		if (const auto isr = igros::x86_64::isrList[regs->number]; nullptr != isr) {
			// Handle ISR
			isr(regs);
		} else {
			// Disable interrupts
			igros::x86_64::irq::disable();
			// Debug
			igros::klib::kprintf(
				"%s -> [#%d]\n"
				"State:\t\tUNHANDLED! CPU halted!\n",
				((regs->number >= igros::x86_64::IRQ_OFFSET) ? "IRQ" : "EXCEPTION"),
				((regs->number >= igros::x86_64::IRQ_OFFSET) ? (regs->number - igros::x86_64::IRQ_OFFSET) : regs->number)
			);
			// Hang CPU
			igros::x86_64::cpu::halt();
		}
	}

#ifdef	__cplusplus

}	// extern "C"

#endif	// __cplusplus

