#include <include/taskRegs.hpp>
#include <include/interrupts.hpp>
#include <include/port.hpp>
#include <include/videoMem.hpp>


// Arch-dependent code zone
namespace arch {

	// interrupts handler function
	void irqHandler(const taskRegs* regs) {

		// Notify slave PIC if needed
		if (regs->number > 39) {

			outPortB(0xA0, 0x20);

		}
		// Notify master PIC
		outPortB(0x20, 0x20);

		// Write text to screen
		videoMemWriteMessage("IRQ");
		videoMemWriteSymbol('0' + regs->number - 32);
		videoMemWriteMessage("\r\n");

	}

}	// arch
