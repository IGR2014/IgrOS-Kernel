////////////////////////////////////////////////////////////////
//
//	Boot low-level main setup function
//
//	File:	boot.cpp
//	Date:	20 Nov. 2017
//
//	Copyright (c) 2017, Igor Baklykov
//	All rights reserved.
//


//#include <vgaConsole.hpp>
#include <include/types.hpp>
#include <include/gdt.hpp>
#include <include/longMode.hpp>


arch::gdtEntry gdtTable[5];
arch::gdtPointer gdt;


extern "C" void kernelFunc() {

	//const char* message = "Hello world, Kernel! :)";

	//unsigned char color = 0x0A;

	// Set color
	//vgaConsoleSetColor(color);

	// Clear screen
	//vgaConsoleClear();

	// Print message on screen
	//vgaConsoleWriteLine(message);

	/*
	unsigned char i = 0;
	do {

		vgaConsoleSetColor(i);
		vgaConsoleWriteSymbol('0');

		if ((i++ % 16) == 15) {

			vgaConsoleWriteSymbol('\r');
			vgaConsoleWriteSymbol('\n');

		}

	} while (i != 0);
	*/

	if (arch::checkMultiboot() == 0x00) {

		return;
	
	}
	if (arch::checkCPUID() == 0x00) {

		return;

	}
	if (arch::checkLongMode() == 0x00) {

		return;
	
	}

	*(word_t*)0xb8000 = (word_t)0x2f4f;
	*(word_t*)0xb8002 = (word_t)0x2f4b;

	arch::setupPageTables();
	arch::enablePaging();

	gdtTable[0] = arch::gdtSetEntry(0x00000000, 0x00000000, 0x0000);
	gdtTable[1] = arch::gdtSetEntry(0x00000000, 0xFFFFFFFF, GDT_CODE_RING0);
	gdtTable[2] = arch::gdtSetEntry(0x00000000, 0xFFFFFFFF, GDT_DATA_RING0);
	gdtTable[3] = arch::gdtSetEntry(0x00000000, 0xFFFFFFFF, GDT_CODE_RING3);
	gdtTable[4] = arch::gdtSetEntry(0x00000000, 0xFFFFFFFF, GDT_DATA_RING3);

	gdt.size	= arch::gdtCalcTableSize(5);
	gdt.pointer	= gdtTable;

	arch::gdtLoad(&gdt);

	*(word_t*)0xb8004 = (word_t)0x2f41;
	*(word_t*)0xb8006 = (word_t)0x2f59;

}

