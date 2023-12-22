////////////////////////////////////////////////////////////////
//
//	Interrupt service routines low-level operations
//
//	File:	isr.cpp
//	Date:	22 Dec 2023
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


// x86_64 namespace
namespace igros::x86_64 {


	// Interrupt handlers
	static auto isrList {std::array<isr_t, ISR_SIZE> {}};


	// Install interrupt service routine handler
	void isrHandlerInstall(const igros_usize_t number, const isr_t handle) noexcept {
		// Put interrupt service routine handler in ISRs list
		isrList[number] = handle;
	}

	// Uninstall interrupt service routine handler
	void isrHandlerUninstall(const igros_usize_t number) noexcept {
		// Remove interrupt service routine handler from ISRs list
		isrList[number] = nullptr;
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
R"unhandled(
%s -> [#%d]
	UNHANDLED! CPU halted!
)unhandled",
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

