#include <include/exceptions.hpp>


// Arch-dependent code zone
namespace arch {

	// Exceptions names
	const char* exceptionName[32] = {"Divide by Zero",			// 0
					 "Debug",				// 1
					 "Non-Maskable Interrupt",		// 2
					 "Breakpoint",				// 3
					 "Into Detected Overflow",		// 4
					 "Out of Bounds",			// 5
					 "Invalid Opcode",			// 6
					 "No Coprocessor",			// 7
					 "Double Fault",			// 8
					 "Coprocessor Segment Overrun",		// 9
					 "Bad Task Switch Segment",		// 10
					 "Segment Not Present",			// 11
					 "Stack Fault",				// 12
					 "General Protection Fault",		// 13
					 "Page Fault",				// 14
					 "Unknown Interrupt",			// 15
					 "Coprocessor Fault",			// 16
					 "Alignment Check",			// 17
					 "Machine Check",			// 18
					 "Reserved",				// 19
					 "Reserved",				// 20
					 "Reserved",				// 21
					 "Reserved",				// 22
					 "Reserved",				// 23
					 "Reserved",				// 24
					 "Reserved",				// 25
					 "Reserved",				// 26
					 "Reserved",				// 27
					 "Reserved",				// 28
					 "Reserved",				// 29
					 "Reserved",				// 30
					 "Reserved"};				// 31

	// Exception handler function
	void exHandler(tsRegs* regs) {

		*(t_u16*)0xb8008 = (t_u16)0x2f4f;
		*(t_u16*)0xb800A = (t_u16)0x2f4b;
		*(t_u16*)0xb800C = (t_u16)0x2f41;
		*(t_u16*)0xb800E = (t_u16)0x2f59;

	}

}	// arch
