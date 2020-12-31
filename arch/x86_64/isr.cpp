////////////////////////////////////////////////////////////////
//
//	Interrupt service routines low-level operations
//
//	File:	isr.cpp
//	Date:	18 Jul 2020
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
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


#ifdef	__cplusplus

	extern "C" {

#endif	// __cplusplus


		// Interrupts handler function
		void isrHandler(const register_t* regs) noexcept {
			// Acquire irq handler from list
			const auto isr = isrList[regs->number];
			// Check if irq/exception handler installed
			if (nullptr != isr) {
				isr(regs);
			} else {
				// Disable interrupts
				irq::disable();
				// Debug
				klib::kprintf(	u8"%s -> [#%d]\r\n"
						u8"State:\t\tUNHANDLED! CPU halted!\r\n",
						((regs->number >= IRQ_OFFSET) ? u8"IRQ" : u8"EXCEPTION"),
						((regs->number >= IRQ_OFFSET) ? (regs->number - IRQ_OFFSET) : regs->number));
				// Hang CPU
				cpu::halt();
			}
		}


#ifdef	__cplusplus

	}	// extern "C"

#endif	// __cplusplus


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

