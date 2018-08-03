////////////////////////////////////////////////////////////////
//
//	Boot low-level main setup function
//
//	File:	boot.cpp
//	Date:	23 Jun. 2018
//
//	Copyright (c) 2018, Igor Baklykov
//	All rights reserved.
//


#include <include/version.hpp>
#include <include/types.hpp>
#include <include/gdt.hpp>
#include <include/idt.hpp>
#include <include/interrupts.hpp>
#include <include/vgaConsole.hpp>
#include <include/paging.hpp>
#include <include/keyboard.hpp>
#include <include/pit.hpp>


// Kernel main function
extern "C" void kernelFunc() {

	// Init VGA console
	arch::vgaConsoleInit();
	arch::vgaConsoleWriteLine("IgrOS kernel");

	// Setup Global Descriptors Table
	arch::gdtSetup();

	// Setup Interrupts Descriptor Table
	arch::idtSetup();

	// Init interrupts
	arch::irqInit();
	// Enable interrupts
	arch::irqEnable();

	// Setup PIT frequency to 100 HZ
	arch::pitSetupFrequency(100);

	// Install keyboard interrupt handler
	arch::irqHandlerInstall(arch::KEYBOARD, arch::keyboardInterruptHandler);
	// Mask keyboard interrupts
	arch::irqMask(arch::KEYBOARD);

	// Install PIT interrupt handler
	arch::irqHandlerInstall(arch::PIT, arch::pitInterruptHandler);
	// Mask PIT interrupts
	arch::irqMask(arch::PIT);

	// Setup paging (And identity map first 4MB where kernel is)
	arch::pagingSetup();

	// Write "Hello World" message
	arch::vgaConsoleWriteLine("");
	arch::vgaConsoleWriteLine("Build:\t\t" __DATE__ " " __TIME__);
	arch::vgaConsoleWrite("Version:\tv");
	arch::vgaConsoleWriteDec(IGROS_VERSION_MAJOR);
	arch::vgaConsoleWrite(".");
	arch::vgaConsoleWriteDec(IGROS_VERSION_MINOR);
	arch::vgaConsoleWrite(".");
	arch::vgaConsoleWriteDec(IGROS_VERSION_BUILD);
	arch::vgaConsoleWrite(" (");
	arch::vgaConsoleWrite(IGROS_VERSION_NAME);
	arch::vgaConsoleWriteLine(")");
	arch::vgaConsoleWrite("Author:\t\tIgor Baklykov (c) ");
	arch::vgaConsoleWriteDec(2017);
	arch::vgaConsoleWrite("-");
	arch::vgaConsoleWriteDec(2018);
	arch::vgaConsoleWriteLine("");
	arch::vgaConsoleWriteLine("");

	// Page mapping test
	/*
	arch::vgaConsoleWriteHex(reinterpret_cast<dword_t>(arch::pagingVirtToPhys(reinterpret_cast<pointer_t>(0xC00B8000))));
	arch::vgaConsoleWriteLine("");
	arch::vgaConsoleWriteLine("");
	volatile t_u16p ptr = reinterpret_cast<t_u16p>(0xC00B8006);
	*ptr = 0x0730;
	*/

	// Numbers print test
	/*
	arch::vgaConsoleWriteDec(0x7FFFFFFF);
	arch::vgaConsoleWrite(" = ");
	arch::vgaConsoleWriteHex(0x7FFFFFFF);
	arch::vgaConsoleWriteLine("");
	*/

	// Page Fault Exception test
	/*
	volatile t_u32p ptr = reinterpret_cast<t_u32p>(0xA0000000);
	*ptr = 0x4000;
	*/

	// Divide by Zero Exception Test
	/*
	volatile sdword_t x = 10;
	volatile sdword_t y = 0;
	volatile sdword_t z = x / y;
	*/

}

