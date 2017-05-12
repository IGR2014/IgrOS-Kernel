#include <include/exception.hpp>
#include <include/port.hpp>


namespace arch {

	// IRQ handler function
	extern "C" void isrHandler(exceptionRegs* regs) {

		*(t_u16*)0xb8008 = (t_u16)0x2f4f;
		*(t_u16*)0xb800A = (t_u16)0x2f4b;
		*(t_u16*)0xb800C = (t_u16)0x2f41;
		*(t_u16*)0xb800E = (t_u16)0x2f59;

		// Infinite loop
		while (true) {};

	}

}	// arch

