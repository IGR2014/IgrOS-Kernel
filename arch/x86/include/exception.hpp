#ifndef IRQ_H
#define IRQ_H


#include <include/types.hpp>


// Arch-dependent code zone
namespace arch {

#pragma pack(push, 1)
	struct exceptionRegs {

		t_u32	gs;
		t_u32	fs;
		t_u32	es;
		t_u32	ds;
		t_u32	edi;
		t_u32	esi;
		t_u32	ebp;
		t_u32	esp;
		t_u32	ebx;
		t_u32	edx;
		t_u32	ecx;
		t_u32	eax;
		t_u32	exception;
		t_u32	param;
		t_u32	eip;
		t_u32	cs;
		t_u32	eflags;
		t_u32	userEsp;
		t_u32	ss;

	};
#pragma pack(pop)

	extern "C" {

		// Exception 0 handler
		extern void isrException0();
		// Exception 1 handler
		extern void isrException1();
		// Exception 2 handler
		extern void isrException2();
		// Exception 3 handler
		extern void isrException3();
		// Exception 4 handler
		extern void isrException4();
		// Exception 5 handler
		extern void isrException5();
		// Exception 6 handler
		extern void isrException6();
		// Exception 7 handler
		extern void isrException7();
		// Exception 8 handler
		extern void isrException8();
		// Exception 9 handler
		extern void isrException9();
		// Exception 10 handler
		extern void isrException10();
		// Exception 11 handler
		extern void isrException11();
		// Exception 12 handler
		extern void isrException12();
		// Exception 13 handler
		extern void isrException13();
		// Exception 14 handler
		extern void isrException14();
		// Exception 15 handler
		extern void isrException15();
		// Exception 16 handler
		extern void isrException16();
		// Exception 17 handler
		extern void isrException17();
		// Exception 18 handler
		extern void isrException18();
		// Exception 19 handler
		extern void isrException19();
		// Exception 20 handler
		extern void isrException20();
		// Exception 21 handler
		extern void isrException21();
		// Exception 22 handler
		extern void isrException22();
		// Exception 23 handler
		extern void isrException23();
		// Exception 24 handler
		extern void isrException24();
		// Exception 25 handler
		extern void isrException25();
		// Exception 26 handler
		extern void isrException26();
		// Exception 27 handler
		extern void isrException27();
		// Exception 28 handler
		extern void isrException28();
		// Exception 29 handler
		extern void isrException29();
		// Exception 30 handler
		extern void isrException30();
		// Exception 31 handler
		extern void isrException31();

		// IRQ handler function
		void isrHandler(exceptionRegs*);

	}

}	// arch

#endif	// IRQ_H

