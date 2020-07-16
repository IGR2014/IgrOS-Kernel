////////////////////////////////////////////////////////////////
//
//	Interrupt service routines low-level operations
//
//	File:	isr.cpp
//	Date:	16 Jul 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#include <arch/i386/register.hpp>
#include <arch/i386/irq.hpp>
#include <arch/i386/isr.hpp>
#include <arch/i386/port.hpp>
#include <arch/i386/cpu.hpp>

#include <klib/kprint.hpp>


// i386 namespace
namespace igros::i386 {


	// Interrupt handlers
	static isr_t isrList[ISR_SIZE] {nullptr};


#ifdef	__cplusplus

	extern "C" {

#endif	// __cplusplus


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
				// Disable interrupts
				irq::disable();
				// Debug
				klib::kprintf(	u8"%s -> [#%d]\r\n"
						u8"State:\t\tUNHANDLED! CPU halted!\r\n",
						((regs->number >= IRQ_OFFSET) ? u8"IRQ" : u8"EXCEPTION"),
						((regs->number >= IRQ_OFFSET) ? (regs->number - IRQ_OFFSET) : regs->number));
				// Hang CPU
				cpuHalt();
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


}	// namespace igros::i386

