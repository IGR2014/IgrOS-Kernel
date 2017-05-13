#ifndef INTERRUPTS_H
#define INTERRUPTS_H


#include <include/types.hpp>


// Arch-dependent code zone
namespace arch {

	struct tsRegs;

	/*
	#pragma pack(push, 1)
	// x86 task switch registers save
	struct tsRegs {

		// Segment registers pushed by us
		t_u32	gs;		// Extra3 segment register
		t_u32	fs;		// Extra2 segment register
		t_u32	es;		// Extra segment register
		t_u32	ds;		// Data segment register
		// "All" registers pushed by pushal instruction
		t_u32	edi;		// Destination index register
		t_u32	esi;		// Source index register
		t_u32	ebp;		// Stack base pointer register
		t_u32	esp;		// Stack pointer register
		t_u32	ebx;		// Base register
		t_u32	edx;		// Data register
		t_u32	ecx;		// Counter register
		t_u32	eax;		// Accumulator register
		// Exception number pushed by us
		t_u32	exception;	// Exception number
		// Param pushed by processor or (0x00) by us
		t_u32	param;		// Exception param (or null if no param provided)
		// This pushed to stack by processor
		t_u32	eip;		// Instruction pointer register
		t_u32	cs;		// Code segment register
		t_u32	eflags;		// Flags register
		t_u32	userEsp;	// User stack pointer register
		t_u32	ss;		// Stack segment register

	};
	#pragma pack(pop)
	*/

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
		void irqHandler(tsRegs*);

	}	// extern "C"

}	// arch

#endif	// INTERRUPTS_H
