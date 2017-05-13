#ifndef INTERRUPTS_H
#define INTERRUPTS_H


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
		extern void irqHandler10();
		// Interrupt 11 handler
		extern void irqHandler11();
		// Interrupt 12 handler
		extern void irqHandler12();
		// Interrupt 13 handler
		extern void irqHandler13();
		// Interrupt 14 handler
		extern void irqHandler14();
		// Interrupt 15 handler
		extern void irqHandler15();

		// Interrupts handler function
		void irqHandler(const taskRegs*);

	}	// extern "C"

}	// arch

#endif	// INTERRUPTS_H
