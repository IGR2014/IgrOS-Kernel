#include <include/interrupts.hpp>
#include <include/exceptions.hpp>
#include <include/port.hpp>


// Arch-dependent code zone
namespace arch {

	// interrupts handler function
	void irqHandler(tsRegs* regs) {

		*(t_u16*)0xb8010 = (t_u16)0x2f4f;
		*(t_u16*)0xb8012 = (t_u16)0x2f4b;
		*(t_u16*)0xb8014 = (t_u16)0x2f41;
		*(t_u16*)0xb8016 = (t_u16)0x2f59;

		if (regs->exception >= 40) {

			outPortB(0xA0, 0x20);

		}
		outPortB(0x20, 0x20);

	}

}	// arch
