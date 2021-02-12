////////////////////////////////////////////////////////////////
//
//	Interrupt service routines low-level operations
//
//	File:	isr.cpp
//	Date:	10 Feb 2021
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#include <array>

#include <arch/x86_64/register.hpp>
#include <arch/x86_64/irq.hpp>
#include <arch/x86_64/isr.hpp>
#include <arch/x86_64/io.hpp>
#include <arch/x86_64/cpu.hpp>

#include <klib/kprint.hpp>


// x86_64 namespace
namespace igros::x86_64 {


	// Interrupt handlers
	static std::array<isrx86_64_t, ISR_SIZE> isrList {
		nullptr
	};


	// Install interrupt service routine handler
	void isrHandlerInstall(const dword_t isrNumber, const isrx86_64_t isrHandler) noexcept{
		// Put interrupt service routine handler in ISRs list
		isrList[isrNumber] = isrHandler;
	}

	// Uninstall interrupt service routine handler
	void isrHandlerUninstall(const dword_t isrNumber) noexcept {
		// Remove interrupt service routine handler from ISRs list
		isrList[isrNumber] = nullptr;
	}


}	// namespace igros::x86_64


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus

	// Interrupts handler function
	void isrHandler(const igros::x86_64::registerx86_64_t* regs) noexcept {
		// Check if irq/exception handler installed
		if (const auto isr = igros::x86_64::isrList[regs->number]; nullptr != isr) {
			// Handle ISR
			isr(regs);
		} else {
			// Disable interrupts
			igros::x86_64::irqx86_64::disable();
			// Debug
			igros::klib::kprintf(
				u8"%s -> [#%d]\r\n"
				u8"State:\t\tUNHANDLED! CPU halted!\r\n",
				((regs->number >= igros::x86_64::IRQ_OFFSET) ? u8"IRQ" : u8"EXCEPTION"),
				((regs->number >= igros::x86_64::IRQ_OFFSET) ? (regs->number - igros::x86_64::IRQ_OFFSET) : regs->number)
			);
			// Hang CPU
			igros::x86_64::cpux86_64::halt();
		}
	}

#ifdef	__cplusplus

}	// extern "C"

#endif	// __cplusplus

