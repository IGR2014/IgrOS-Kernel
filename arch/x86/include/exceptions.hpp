#ifndef IRQ_H
#define IRQ_H


#include <include/types.hpp>


// Arch-dependent code zone
namespace arch {

#pragma pack(push, 1)
	// x86 exception registers save
	struct exceptionRegs {

		t_u32	gs;		// GS segment register
		t_u32	fs;		// FS segment register
		t_u32	es;		// Extra segment register
		t_u32	ds;		// Data segment register
		t_u32	edi;		// Destination index register
		t_u32	esi;		// Source index register
		t_u32	ebp;		// Stack base pointer register
		t_u32	esp;		// Stack pointer register
		t_u32	ebx;		// Base register
		t_u32	edx;		// Data register
		t_u32	ecx;		// Counter register
		t_u32	eax;		// Accumulator register
		t_u32	exception;	// Exception number
		t_u32	param;		// Exception param (or null if no param provided)
		t_u32	eip;		// Instruction pointer register
		t_u32	cs;		// Code segment register
		t_u32	eflags;		// Flags register
		t_u32	userEsp;	// User stack pointer register
		t_u32	ss;		// Stack segment register

	};
#pragma pack(pop)

	extern "C" {

		// Exception 0 handler
		extern void exHandler0();
		// Exception 1 handler
		extern void exHandler1();
		// Exception 2 handler
		extern void exHandler2();
		// Exception 3 handler
		extern void exHandler3();
		// Exception 4 handler
		extern void exHandler4();
		// Exception 5 handler
		extern void exHandler5();
		// Exception 6 handler
		extern void exHandler6();
		// Exception 7 handler
		extern void exHandler7();
		// Exception 8 handler
		extern void exHandler8();
		// Exception 9 handler
		extern void exHandler9();
		// Exception 10 handler
		extern void exHandler10();
		// Exception 11 handler
		extern void exHandler11();
		// Exception 12 handler
		extern void exHandler12();
		// Exception 13 handler
		extern void exHandler13();
		// Exception 14 handler
		extern void exHandler14();
		// Exception 15 handler
		extern void exHandler15();
		// Exception 16 handler
		extern void exHandler16();
		// Exception 17 handler
		extern void exHandler17();
		// Exception 18 handler
		extern void exHandler18();
		// Exception 19 handler
		extern void exHandler19();
		// Exception 20 handler
		extern void exHandler20();
		// Exception 21 handler
		extern void exHandler21();
		// Exception 22 handler
		extern void exHandler22();
		// Exception 23 handler
		extern void exHandler23();
		// Exception 24 handler
		extern void exHandler24();
		// Exception 25 handler
		extern void exHandler25();
		// Exception 26 handler
		extern void exHandler26();
		// Exception 27 handler
		extern void exHandler27();
		// Exception 28 handler
		extern void exHandler28();
		// Exception 29 handler
		extern void exHandler29();
		// Exception 30 handler
		extern void exHandler30();
		// Exception 31 handler
		extern void exHandler31();

		// Exception handler function
		void exHandler(exceptionRegs*);

	}

}	// arch

#endif	// IRQ_H
