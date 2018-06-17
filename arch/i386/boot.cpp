////////////////////////////////////////////////////////////////
//
//	Boot low-level main setup function
//
//	File:	boot.cpp
//	Date:	18 Jun. 2018
//
//	Copyright (c) 2018, Igor Baklykov
//	All rights reserved.
//


#include <include/types.hpp>
#include <include/gdt.hpp>
#include <include/idt.hpp>
#include <include/interrupts.hpp>
#include <include/port.hpp>
#include <include/videoMem.hpp>
#include <include/paging.hpp>
#include <include/memset.hpp>


extern t_u32* pageDirectory;
extern t_u32* pageTable;


// Kernel main function
extern "C" void kernelFunc() {

	// Init video memory
	arch::videoMemInit();
	arch::videoMemWriteLine("IgrOS kernel");

	// Setup Global Descriptors Table
	arch::gdtSetup();

	// Setup Interrupts Descriptor Table
	arch::idtSetup();

	// ICW 1 for PIC
	arch::outPortB(0x20, 0x11);
	arch::outPortB(0xA0, 0x11);

	// ICW 2 for PIC
	arch::outPortB(0x21, 0x20);
	arch::outPortB(0xA1, 0x28);

	// ICW 3 for PIC
	arch::outPortB(0x21, 0x04);
	arch::outPortB(0xA1, 0x02);

	// ICW 4 for PIC
	arch::outPortB(0x21, 0x01);
	arch::outPortB(0xA1, 0x01);

	// Mask interrupts
	arch::outPortB(0x21, 0xfd);
	arch::outPortB(0xA1, 0xff);

	// Enable interrupts
	arch::irqEnable();

	pageDirectory[0] = ((t_u32)pageTable) | 3;

	for (unsigned int i = 1; i < 1024; ++i) {

		pageDirectory[i] = 0x00000002;

	}

	//pageDirectory[1023] = ((t_u32)pageDirectory);

	for (unsigned int j = 0; j < 1024; ++j) {

		pageTable[j] = (j * 0x1000) | 3;

	}

	// Setup page directory
	arch::pagingSetupPD(pageDirectory);
	// Enable paging
	arch::pagingEnable();

	// Write "Hello World" message
	arch::videoMemWriteLine("");
	arch::videoMemWrite("Build:\t\t");
	arch::videoMemWrite(__DATE__);
	arch::videoMemWrite(" ");
	arch::videoMemWriteLine(__TIME__);
	arch::videoMemWrite("Version:\t");
	arch::videoMemWriteLine("v0.00.220 (pre-alpha)");
	arch::videoMemWrite("Author:\t\t");
	arch::videoMemWrite("Igor Baklykov (c) ");
	arch::videoMemWriteDec(2017);
	arch::videoMemWrite(" - ");
	arch::videoMemWriteDec(2018);
	arch::videoMemWriteLine("");
	arch::videoMemWriteLine("");

	// Page fault exception
	/*
	volatile t_u32* ptr = reinterpret_cast<t_u32*>(0xA0000000);
	volatile t_u32 a = *ptr;
	*/

	// Divide by Zero Exception Test
	/*
	volatile int x = 10;
	volatile int y = 0;
	volatile int z = x / y;
	*/


}

