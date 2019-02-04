////////////////////////////////////////////////////////////////
//
//	Boot low-level main setup function
//
//	File:	boot.cpp
//	Date:	13 Aug. 2018
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
//	All rights reserved.
//
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


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus


	// Kernel main function
	void kernelFunc(dword_t mBootMagic, pointer_t* mBootData) {

		// Init VGA console
		arch::vgaConsoleInit();
		arch::vgaConsoleWriteLine("IgrOS kernel");

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
		arch::irqHandlerInstall(arch::irqNumber_t::KEYBOARD, arch::keyboardInterruptHandler);
		// Mask Keyboard interrupts
		arch::irqMask(arch::irqNumber_t::KEYBOARD);
		// Install PIT interrupt handler
		arch::irqHandlerInstall(arch::irqNumber_t::PIT, arch::pitInterruptHandler);
		// Mask PIT interrupts
		arch::irqMask(arch::irqNumber_t::PIT);

		// Setup paging (And identity map first 4MB where kernel is)
		arch::pagingSetup();

		// Write "Hello World" message
		arch::vgaConsoleWriteLine("");
		arch::vgaConsoleWriteLine("Booted successfully");
		arch::vgaConsoleWriteLine("");

		/*
		// Page mapping test
		arch::vgaConsoleWriteHex(reinterpret_cast<dword_t>(arch::pagingVirtToPhys(reinterpret_cast<pointer_t>(0xC00B8000))));
		arch::vgaConsoleWriteLine("");
		arch::vgaConsoleWriteLine("");
		volatile word_t* ptr = reinterpret_cast<word_t*>(0xC00B8006);
		*ptr = 0x0700 | 'O';
		++ptr;
		*ptr = 0x0700 | 'S';
		*/

		// Numbers print test
		/*
		arch::vgaConsoleWriteDec(0x7FFFFFFF);
		arch::vgaConsoleWrite(" = ");
		arch::vgaConsoleWriteHex(0x7FFFFFFF);
		arch::vgaConsoleWriteLine("");
		*/

		/*
		// Page Fault Exception test
		volatile word_t* ptr = reinterpret_cast<word_t*>(0xA0000000);
		*ptr = 0x4000;
		*/

		/*
		// Divide by Zero Exception Test
		volatile dword_t x = 10;
		volatile dword_t y = 0;
		volatile dword_t z = x / y;
		*/

	}


#ifdef	__cplusplus

}	// extern "C"

#endif	// __cplusplus

