////////////////////////////////////////////////////////////////
//
//	Boot low-level main setup function
//
//	File:	boot.cpp
//	Date:	21 Jun 2019
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
//	All rights reserved.
//
//


#include <version.hpp>
#include <multiboot.hpp>
#include <platform.hpp>
#include <flags.hpp>

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
	void kmain(multiboot::info* multiboot, dword_t magic) {

		// Init VGA memory
		arch::vmemInit();
		arch::vmemWrite("IgrOS kernel\r\n\r\n");

		// Print buffer
		sbyte_t text[64];

		// Write Multiboot info message
		//arch::vmemWrite("Multiboot info\r\n");

		// Check multiboot magic
		if (!multiboot::check(magic)) {

			arch::vmemWrite("\tBAD MULTIBOOT MAGIC!!!\r\n");
			// Print multiboot magic
			arch::vmemWrite("\tMAGIC:\t\t0x");
			arch::vmemWrite(klib::kitoa(text, 20, magic, klib::base::HEX));
			// Print multiboot address
			arch::vmemWrite("\r\n\tADDRESS:\t0x");
			arch::vmemWrite(klib::kitoa(text, 20, quad_t(multiboot), klib::base::HEX));

			// Hang CPU
			while (true) {};

		}

		// Dump multiboot flags
		//multiboot->dumpFlags();

		// Write Multiboot info message
		arch::vmemWrite("Bootloader info\r\n");
		// Dump multiboot command line
		arch::vmemWrite("\tCommand line:\t");
		arch::vmemWrite(multiboot->commandLine());
		arch::vmemWrite("\r\n");
		// Dump multiboot bootloader name
		arch::vmemWrite("\tLoader name:\t");
		arch::vmemWrite(multiboot->loaderName());
		arch::vmemWrite("\r\n\r\n");

		// Write Kernel info message
		arch::vmemWrite("Kernel info\r\n");
		arch::vmemWrite("\tBuild:\t\t" __DATE__ ", " __TIME__ "\r\n");
		arch::vmemWrite("\tVersion:\tv");
		arch::vmemWrite(klib::kitoa(text, 20, IGROS_VERSION_MAJOR));
		arch::vmemWrite(".");
		arch::vmemWrite(klib::kitoa(text, 20, IGROS_VERSION_MINOR));
		arch::vmemWrite(".");
		arch::vmemWrite(klib::kitoa(text, 20, IGROS_VERSION_BUILD));
		arch::vmemWrite(" (");
		arch::vmemWrite(IGROS_VERSION_NAME);
		arch::vmemWrite(")\r\n");
		arch::vmemWrite("\tAuthor:\t\tIgor Baklykov (c) ");
		arch::vmemWrite(klib::kitoa(text, 20, dword_t(2017)));
		arch::vmemWrite(" - ");
		arch::vmemWrite(klib::kitoa(text, 20, dword_t(2019)));
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
		// Page mapping test (higher half test)
		volatile word_t* ptr = reinterpret_cast<word_t*>(0xC00B8006);
		arch::vmemWrite("0x");
		arch::vmemWrite(klib::kitoa(text, 20, dword_t(ptr), klib::base::HEX));
		arch::vmemWrite(" = 0x");
		arch::vmemWrite(klib::kitoa(text, 20, dword_t(arch::pagingVirtToPhys(pointer_t(ptr))), klib::base::HEX));
		arch::vmemWrite("\r\n\r\n");
		// Rewrite 'IgrOS' text 'O' and 'S' green symbols with white ones
		*ptr = 0x0700 | 'O';
		++ptr;
		*ptr = 0x0700 | 'S';
		*/

		/*
		// Numbers print test
		arch::vmemWrite("0x");
		arch::vmemWrite(klib::kitoa(text, 20, dword_t(0x7FFFFFFF), klib::base::HEX));
		arch::vmemWrite(" = ");
		arch::vmemWrite(klib::kitoa(text, 20, dword_t(0x7FFFFFFF)));
		arch::vmemWrite("\r\n");
		*/

		/*
		// Page Fault Exception test
		volatile word_t* ptr2 = reinterpret_cast<word_t*>(0xA0000000);
		*ptr2 = 0x4000;
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

