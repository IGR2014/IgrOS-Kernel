////////////////////////////////////////////////////////////////
//
//	Interrupts low-level operations
//
//	File:	interrupts.hpp
//	Date:	20 Nov. 2017
//
//	Copyright (c) 2017, Igor Baklykov
//	All rights reserved.
//


#ifndef IGROS_ARCH_INTERRUPTS_HPP
#define IGROS_ARCH_INTERRUPTS_HPP


#include <include/types.hpp>
#include <include/taskRegs.hpp>


// Arch-dependent code zone
namespace arch {


	extern "C" {

		// Interrupt 0 handler
		extern void irqHandler0();
		// Interrupt 1 handler
		extern void irqHandler1();
		// Interrupt 2 handler
		extern void irqHandler2();
		// Interrupt 3 handler
		extern void irqHandler3();
		// Interrupt 4 handler
		extern void irqHandler4();
		// Interrupt 5 handler
		extern void irqHandler5();
		// Interrupt 6 handler
		extern void irqHandler6();
		// Interrupt 7 handler
		extern void irqHandler7();
		// Interrupt 8 handler
		extern void irqHandler8();
		// Interrupt 9 handler
		extern void irqHandler9();
		// Interrupt 10 handler
		extern void irqHandlerA();
		// Interrupt 11 handler
		extern void irqHandlerB();
		// Interrupt 12 handler
		extern void irqHandlerC();
		// Interrupt 13 handler
		extern void irqHandlerD();
		// Interrupt 14 handler
		extern void irqHandlerE();
		// Interrupt 15 handler
		extern void irqHandlerF();

		// Interrupts handler function
		void irqHandler(const taskRegs*);

	}	// extern "C"


}	// namespace arch


#endif	// IGROS_ARCH_INTERRUPTS_HPP

