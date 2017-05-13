#include <include/exceptions.hpp>


namespace arch {

	// Exception handler function
	void exHandler(exceptionRegs* regs) {

		*(t_u16*)0xb8008 = (t_u16)0x2f4f;
		*(t_u16*)0xb800A = (t_u16)0x2f4b;
		*(t_u16*)0xb800C = (t_u16)0x2f41;
		*(t_u16*)0xb800E = (t_u16)0x2f59;

	}

}	// arch
