////////////////////////////////////////////////////////////////
//
//	Exceptions low-level operations
//
//	File:	exceptions.cpp
//	Date:	08 Oct 2019
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
//	All rights reserved.
//
//


#include <arch/taskRegs.hpp>
#include <arch/exceptions.hpp>

#include <drivers/vmem.hpp>

#include <klib/kprint.hpp>


// Arch-dependent code zone
namespace arch {


	// Exceptions names
	const sbyte_t* exName[32] = {"DIVIDE BY ZERO",			// 0
				     "DEBUG",				// 1
				     "NON-MASKABLE INTERRUPT",		// 2
				     "BREAKPOINT",			// 3
				     "INTO DETECTED OVERFLOW",		// 4
				     "BOUND RANGE EXCEEDED",		// 5
				     "INVALID OPCODE",			// 6
				     "NO COPROCESSOR",			// 7
				     "DOUBLE FAULT",			// 8
				     "COPROCESSOR SEGMENT OVERRUN",	// 9
				     "INVALID TSS",			// 10
				     "SEGMENT NOT PPRESENT",		// 11
				     "STACK FAULT",			// 12
				     "GENERAL PROTECTION FAULT",	// 13
				     "PAGE FAULT",			// 14
				     "UNKNOWN INTERRUPT",		// 15
				     "COPROCESSOR FAULT",		// 16
				     "ALIGNMENT CHECK",			// 17
				     "MACHINE CHECK",			// 18
				     "RESERVED",			// 19
				     "RESERVED",			// 20
				     "RESERVED",			// 21
				     "RESERVED",			// 22
				     "RESERVED",			// 23
				     "RESERVED",			// 24
				     "RESERVED",			// 25
				     "RESERVED",			// 26
				     "RESERVED",			// 27
				     "RESERVED",			// 28
				     "RESERVED",			// 29
				     "RESERVED",			// 30
				     "RESERVED"};			// 31


	// Exception handlers
	static exHandler_t exList[32] = {};


	// Exception handler function
	void exHandler(const taskRegs_t* regs) {

		// Actually it`s an interrupt and normaly shouldn't be there
		if (regs->number > 32) {
			return;
		}

		// Acquire irq handler from list
		auto exception = exList[regs->number];

		// Print buffer
		sbyte_t text[1024];
		klib::ksprint(text,	"EXCEPTION ->\t[#%d] %s\r\n"
					"STATE:\t\t%s\r\n",
					regs->number,
					exName[regs->number],
					(nullptr == exception) ? "UNHANDLED! CPU halted!\r\n\r\n" : "IN PROGRESS");
		vmemWrite(text);

		// Check if exception handler installed
		if (nullptr != exception) {
			exception(regs);
		} else {
			// Hang CPU
			while (true) {};
		}

	}


	// Install handler
	void exHandlerInstall(exNumber_t exNumber, exHandler_t handler) {
		// Set exception handler to handlers list
		exList[exNumber] = handler;
	}

	// Uninstall handler
	void exHandlerUninstall(exNumber_t exNumber) {
		// Set exception handler as nullptr to exceprions list
		exList[exNumber] = nullptr;
	}


}	// namespace arch

