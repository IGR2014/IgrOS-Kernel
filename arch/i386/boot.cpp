////////////////////////////////////////////////////////////////
//
//	Boot low-level main setup function
//
//	File:	boot.cpp
//	Date:	22 May 2019
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
//	All rights reserved.
//
//


#include <version.hpp>
#include <multiboot.hpp>

#include <arch/types.hpp>
#include <arch/gdt.hpp>
#include <arch/idt.hpp>
#include <arch/interrupts.hpp>
#include <arch/paging.hpp>

#include <drivers/vmem.hpp>
#include <drivers/keyboard.hpp>
#include <drivers/pit.hpp>

#include <klib/kprint.hpp>


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus


	// Kernel main function
	void kernelFunc(multiboot::info* multiboot, dword_t magic) {

		// Init VGA memory
		arch::vmemInit();
		arch::vmemWrite("IgrOS kernel\r\n\r\n");

		// Check multiboot magic
		if (!multiboot::check(magic)) {

			//arch::vgaConsoleWriteHex(magic);
			arch::vmemWrite("Bad multiboot 1 bootloader magic!");

			// Hang CPU
			while (true) {};

		}

		// Print buffer
		sbyte_t text[64];

		// Write "Hello World" message
		arch::vmemWrite("Build:\t\t" __DATE__ ", " __TIME__ "\r\n");
		arch::vmemWrite("Version:\tv");
		klib::kitoa(text, 20, IGROS_VERSION_MAJOR);
		arch::vmemWrite(text);
		arch::vmemWrite(".");
		klib::kitoa(text, 20, IGROS_VERSION_MINOR);
		arch::vmemWrite(text);
		arch::vmemWrite(".");
		klib::kitoa(text, 20, IGROS_VERSION_BUILD);
		arch::vmemWrite(text);
		arch::vmemWrite(" (");
		arch::vmemWrite(IGROS_VERSION_NAME);
		arch::vmemWrite(")\r\n");
		arch::vmemWrite("Author:\t\tIgor Baklykov (c) ");
		klib::kitoa(text, 20, static_cast<dword_t>(2017));
		arch::vmemWrite(text);
		arch::vmemWrite(" - ");
		klib::kitoa(text, 20, static_cast<dword_t>(2019));
		arch::vmemWrite(text);
		arch::vmemWrite("\r\n\r\n");

		// Setup Interrupts Descriptor Table
		arch::idtSetup();

		// Init interrupts
		arch::irqInit();
		// Enable interrupts
		arch::irqEnable();

		// Setup Global Descriptors Table
		arch::gdtSetup();

		// Setup paging (And identity map first 4MB where kernel is)
		arch::pagingSetup();

		// Setup keyboard
		arch::keyboardSetup();
		// Setup PIT
		arch::pitSetup();

		// Write "Booted successfully" message
		arch::vmemWrite("\r\nBooted successfully\r\n\r\n");

		/*
		// Page mapping test
		arch::vgaConsoleWriteHex(reinterpret_cast<dword_t>(arch::pagingVirtToPhys(reinterpret_cast<pointer_t>(0xC00B8000))));
		arch::vmemWrite("\r\n\r\n");
		volatile word_t* ptr = reinterpret_cast<word_t*>(0xC00B8006);
		*ptr = 0x0700 | 'O';
		++ptr;
		*ptr = 0x0700 | 'S';
		*/

		// Numbers print test
		/*
		//arch::vgaConsoleWriteDec(0x7FFFFFFF);
		arch::vmemWrite(" = ");
		//arch::vgaConsoleWriteHex(0x7FFFFFFF);
		arch::vmemWrite("\r\n");
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

