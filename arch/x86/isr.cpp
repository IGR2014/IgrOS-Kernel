////////////////////////////////////////////////////////////////
//
//	Interrupt service routines low-level operations
//
//	File:	isr.cpp
//	Date:	19 Dec 2023
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


// IgrOS-Kernel library
#include <arch/x86/io.hpp>
#include <arch/x86/irq.hpp>
#include <arch/x86/isr.hpp>
#include <arch/x86/native.hpp>
// IgrOS-Kernel library
#include <klib/kprint.hpp>


// x86 namespace
namespace igros::x86 {


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


}	// namespace igros::x86


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus


	// Interrupts handler function
	void isrHandler(const igros::x86::register_t* regs) noexcept {
		// Interrupt vector
		const auto vector {static_cast<igros::igros_usize_t>(regs->number)};
		// Is it hardware interrupt
		const auto isIrq {igros::x86::isIrqVector(vector)};
		// Spurious IRQ has no handler and must not be acknowledged on PIC which raised it
		if (isIrq && igros::x86::irq::isSpurious(igros::x86::irqFromVector(vector))) [[unlikely]] {
			// Master PIC still needs EOI for cascaded slave spurious IRQ
			if (igros::x86::irq_t::ATA_SECONDARY == igros::x86::irqFromVector(vector)) {
				igros::x86::irq::eoi(igros::x86::irq_t::CASCADE);
			}
			return;
		}
		// Check if irq/exception handler installed
		if (const auto isr {igros::x86::isrList[vector]}; nullptr != isr) {
			// Handle ISR
			isr(regs);
			// Acknowledge hardware interrupt
			if (isIrq) {
				igros::x86::irq::eoi(igros::x86::irqFromVector(vector));
			}
		} else {
			// Disable interrupts
			igros::x86::irq::disable();
			// Debug
			igros::klib::kprintf(
R"unhandled(
%s -> [#%d]
	UNHANDLED! CPU halted!
)unhandled",
				(isIrq ? "IRQ" : "EXCEPTION"),
				static_cast<igros::igros_dword_t>(isIrq ? (vector - igros::x86::IRQ_OFFSET) : vector)
			);
			// Dump registres
			igros::x86::cpu::dumpRegisters(regs);
			// Hang CPU
			igros::x86::cpu::halt();
		}
	}


#ifdef	__cplusplus

}	// extern "C"

#endif	// __cplusplus

