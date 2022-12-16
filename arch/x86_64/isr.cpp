////////////////////////////////////////////////////////////////
//
//	Interrupt service routines low-level operations
//
//	File:	isr.cpp
//	Date:	16 Dec 2022
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


// C++
#include <array>
// IgrOS-Kernel arch x86_64
#include <arch/x86_64/cpu.hpp>
#include <arch/x86_64/io.hpp>
#include <arch/x86_64/irq.hpp>
#include <arch/x86_64/isr.hpp>
#include <arch/x86_64/register.hpp>
// IgrOS-Kernel library
#include <klib/kprint.hpp>


// x86_64 namespace
namespace igros::x86_64 {


	// Interrupt handlers
	static auto isrList {std::array<isrx86_64_t, ISR_SIZE> {}};


	// Install interrupt service routine handler
	void isrHandlerInstall(const igros_dword_t isrNumber, const isrx86_64_t isrHandler) noexcept{
		// Put interrupt service routine handler in ISRs list
		isrList[isrNumber] = isrHandler;
	}

	// Uninstall interrupt service routine handler
	void isrHandlerUninstall(const igros_dword_t isrNumber) noexcept {
		// Remove interrupt service routine handler from ISRs list
		isrList[isrNumber] = nullptr;
	}


}	// namespace igros::x86_64


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

