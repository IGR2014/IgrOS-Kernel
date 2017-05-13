#include <include/taskRegs.hpp>
#include <include/interrupts.hpp>
#include <include/port.hpp>


// Arch-dependent code zone
namespace arch {

	// interrupts handler function
	void irqHandler(const taskRegs* regs) {

		*(t_u16*)0xb8010 = (t_u16)0x2f4f;
		*(t_u16*)0xb8012 = (t_u16)0x2f4b;
		*(t_u16*)0xb8014 = (t_u16)0x2f41;
		*(t_u16*)0xb8016 = (t_u16)0x2f59;

		// Notify slave PIC if needed
		if (regs->number > 0x27) {

			outPortB(0xA0, 0x20);

		}
		// Notify master PIC
		outPortB(0x20, 0x20);

	}

}	// arch
