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
#include <arch/i386/cpu.hpp>
#include <arch/i386/io.hpp>
#include <arch/i386/irq.hpp>
#include <arch/i386/isr.hpp>
#include <arch/i386/register.hpp>
// IgrOS-Kernel library
#include <klib/kprint.hpp>


// i386 namespace
namespace igros::i386 {


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


}	// namespace igros::i386


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus


	// Interrupts handler function
	void isrHandler(const igros::i386::register_t* regs) noexcept {
		// Interrupt vector
		const auto vector {static_cast<igros::igros_usize_t>(regs->number)};
		// Is it hardware interrupt
		const auto isIrq {igros::i386::isIrqVector(vector)};
		// Spurious IRQ has no handler and must not be acknowledged on PIC which raised it
		if (isIrq && igros::i386::irq::isSpurious(igros::i386::irqFromVector(vector))) [[unlikely]] {
			// Master PIC still needs EOI for cascaded slave spurious IRQ
			if (igros::i386::irq_t::ATA_SECONDARY == igros::i386::irqFromVector(vector)) {
				igros::i386::irq::eoi(igros::i386::irq_t::CASCADE);
			}
			return;
		}
		// Check if irq/exception handler installed
		if (const auto isr {igros::i386::isrList[vector]}; nullptr != isr) {
			// Handle ISR
			isr(regs);
			// Acknowledge hardware interrupt
			if (isIrq) {
				igros::i386::irq::eoi(igros::i386::irqFromVector(vector));
			}
		} else {
			// Disable interrupts
			igros::i386::irq::disable();
			// Debug
			igros::klib::kprintf(
R"unhandled(
%s -> [#%d]
	UNHANDLED! CPU halted!
)unhandled",
				(isIrq ? "IRQ" : "EXCEPTION"),
				static_cast<igros::igros_dword_t>(isIrq ? (vector - igros::i386::IRQ_OFFSET) : vector)
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

