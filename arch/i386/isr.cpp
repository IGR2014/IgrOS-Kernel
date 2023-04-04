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


// IgrOS-Kernel library
#include <arch/i386/cpu.hpp>
#include <arch/i386/io.hpp>
#include <arch/i386/irq.hpp>
#include <arch/i386/isr.hpp>
#include <arch/i386/register.hpp>
// IgrOS-Kernel library
#include <klib/kprint.hpp>


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus


	// Interrupts handler function
	void isrHandler(const igros::i386::register_t* regs) noexcept {
		// Check if irq/exception handler installed
		if (const auto isr = igros::i386::isrList[regs->number]; nullptr != isr) {
			// Handle ISR
			isr(regs);
		} else {
			// Disable interrupts
			igros::i386::irq::disable();
			// Debug
			igros::klib::kprintf(
				"%s -> [#%d]\n"
				"State:\t\tUNHANDLED! CPU halted!\n",
				((regs->number >= igros::i386::IRQ_OFFSET) ? "IRQ" : "EXCEPTION"),
				((regs->number >= igros::i386::IRQ_OFFSET) ? (regs->number - igros::i386::IRQ_OFFSET) : regs->number)
			);
			// Dump registres
			igros::i386::cpu::dumpRegisters(regs);
			// Hang CPU
			igros::i386::cpu::halt();
		}
	}


#ifdef	__cplusplus

}	// extern "C"

#endif	// __cplusplus

