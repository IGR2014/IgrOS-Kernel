////////////////////////////////////////////////////////////////
//
//	Exceptions low-level operations
//
//	File:	exceptions.cpp
//	Date:	20 Jun. 2018
//
//	Copyright (c) 2018, Igor Baklykov
//	All rights reserved.
//


#include <include/taskRegs.hpp>
#include <include/exceptions.hpp>
#include <include/videoMem.hpp>


// Arch-dependent code zone
namespace arch {


	// Exceptions names
	const t_i8p exName[32] = {"Divide by Zero",			// 0
				  "Debug",				// 1
				  "Non-Maskable Interrupt",		// 2
				  "Breakpoint",				// 3
				  "Into Detected Overflow",		// 4
				  "Bound Range Exceeded",		// 5
				  "Invalid Opcode",			// 6
				  "No Coprocessor",			// 7
				  "Double Fault",			// 8
				  "Coprocessor Segment Overrun",	// 9
				  "Invalid TSS",			// 10
				  "Segment Not Present",		// 11
				  "Stack Fault",			// 12
				  "General Protection Fault",		// 13
				  "PAGE FAULT",				// 14
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


	// Exception handlers
	static exHandler_t	exList[32] = {0};


	// Exception handler function
	void exHandler(const taskRegs* regs) {

		// Actually it`s an interrupt and normaly shouldn't be there
		if (regs->number > 32) {

			return;

		}

		// Acquire irq handler from list
		exHandler_t exception = exList[regs->number];

		// Check if exception handler installed
		if (exception) {

			// Manage exception
			videoMemWriteLine("");
			videoMemWrite("EXCEPTION:\t");
			videoMemWriteLine(exName[regs->number]);
			exception(regs);
			videoMemWriteLine("");

		} else {

			// Exception handler is not installed
			videoMemWriteLine("");
			videoMemWrite("EXCEPTION:\t");
			videoMemWrite(exName[regs->number]);
			videoMemWriteLine(" unhandled!");
			videoMemWriteLine("CPU halted.");

			// Hang CPU
			while (true) {};

		}

	}


	// Install handler
	void exHandlerInstall(EX_NUMBER exNumber, exHandler_t handler) {

		exList[exNumber] = handler;

	}

	// Uninstall handler
	void exHandlerUninstall(EX_NUMBER exNumber) {

		exList[exNumber] = nullptr;

	}


}	// namespace arch

