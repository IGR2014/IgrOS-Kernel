////////////////////////////////////////////////////////////////
//
//	Boot low-level main setup function
//
//	File:	boot.cpp
//	Date:	21 Jun. 2018
//
//	Copyright (c) 2018, Igor Baklykov
//	All rights reserved.
//


#include <include/version.hpp>
#include <include/types.hpp>
#include <include/gdt.hpp>
#include <include/idt.hpp>
#include <include/interrupts.hpp>
#include <include/videoMem.hpp>
#include <include/paging.hpp>
#include <include/keyboard.hpp>


// Kernel main function
extern "C" void kernelFunc() {

	// Init video memory
	arch::videoMemInit();
	arch::videoMemWriteLine("IgrOS kernel");

	// Setup Global Descriptors Table
	arch::gdtSetup();

	// Setup Interrupts Descriptor Table
	arch::idtSetup();

	// Init interrupts
	arch::irqInit();
	// Enable interrupts
	arch::irqEnable();

	// Install keyboard interrupt handler
	arch::irqHandlerInstall(arch::KEYBOARD, arch::keyboardInterruptHandler);
	// Mask interrupts
	arch::irqMask(arch::KEYBOARD);

	// Setup paging (And identity map first 4MB where kernel is)
	arch::pagingSetup();

	// Write "Hello World" message
	arch::videoMemWriteLine("");
	arch::videoMemWriteLine("Build:\t\t" __DATE__ " " __TIME__);
	arch::videoMemWrite("Version:\tv");
	arch::videoMemWriteDec(IGROS_VERSION_MAJOR);
	arch::videoMemWrite(".");
	arch::videoMemWriteDec(IGROS_VERSION_MINOR);
	arch::videoMemWrite(".");
	arch::videoMemWriteDec(IGROS_VERSION_BUILD);
	arch::videoMemWrite(" (");
	arch::videoMemWrite(IGROS_VERSION_NAME);
	arch::videoMemWriteLine(")");
	arch::videoMemWrite("Author:\t\tIgor Baklykov (c) ");
	arch::videoMemWriteDec(2017);
	arch::videoMemWrite("-");
	arch::videoMemWriteDec(2018);
	arch::videoMemWriteLine("");
	arch::videoMemWriteLine("");

	// Page mapping test
	/*
	arch::videoMemWriteHex(reinterpret_cast<t_u32>(arch::pagingVirtToPhys(reinterpret_cast<t_ptr>(0xC00B8000))));
	arch::videoMemWriteLine("");
	arch::videoMemWriteLine("");
	volatile t_u16p ptr = reinterpret_cast<t_u16p>(0xC00B8006);
	*ptr = 0x0730;
	*/

	// Numbers print test
	/*
	arch::videoMemWriteDec(0x7FFFFFFF);
	arch::videoMemWrite(" = ");
	arch::videoMemWriteHex(0x7FFFFFFF);
	arch::videoMemWriteLine("");
	*/

	// Page Fault Exception test
	/*
	volatile t_u32p ptr = reinterpret_cast<t_u32p>(0xA0000000);
	*ptr = 0x4000;
	*/

	// Divide by Zero Exception Test
	/*
	volatile t_i32 x = 10;
	volatile t_i32 y = 0;
	volatile t_i32 z = x / y;
	*/

}

