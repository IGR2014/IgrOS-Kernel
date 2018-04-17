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
		void irqHandler0();
		// Interrupt 1 handler
		void irqHandler1();
		// Interrupt 2 handler
		void irqHandler2();
		// Interrupt 3 handler
		void irqHandler3();
		// Interrupt 4 handler
		void irqHandler4();
		// Interrupt 5 handler
		void irqHandler5();
		// Interrupt 6 handler
		void irqHandler6();
		// Interrupt 7 handler
		void irqHandler7();
		// Interrupt 8 handler
		void irqHandler8();
		// Interrupt 9 handler
		void irqHandler9();
		// Interrupt 10 handler
		void irqHandlerA();
		// Interrupt 11 handler
		void irqHandlerB();
		// Interrupt 12 handler
		void irqHandlerC();
		// Interrupt 13 handler
		void irqHandlerD();
		// Interrupt 14 handler
		void irqHandlerE();
		// Interrupt 15 handler
		void irqHandlerF();

		// Interrupts handler function
		void irqHandler(const taskRegs*);

	}	// extern "C"


}	// namespace arch


#endif	// IGROS_ARCH_INTERRUPTS_HPP

