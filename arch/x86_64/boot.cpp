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


//#include <videoMem.hpp>
#include <include/types.hpp>
#include <include/gdt.hpp>
#include <include/longMode.hpp>


arch::gdtEntry gdtTable[5];
arch::gdtPointer gdt;


extern "C" void kernelFunc() {

	//const char* message = "Hello world, Kernel! :)";

	//unsigned char color = 0x0A;

	// Set color
	//videoMemSetColor(color);

	// Clear screen
	//videoMemClear();

	// Print message on screen
	//videoMemWriteLine(message);

	/*
	unsigned char i = 0;
	do {

		videoMemSetColor(i);
		videoMemWriteSymbol('0');

		if ((i++ % 16) == 15) {

			videoMemWriteSymbol('\r');
			videoMemWriteSymbol('\n');

		}

	} while (i != 0);
	*/

	*(t_u16*)0xb8000 = (t_u16)0x2f4f;

	if (arch::checkMultiboot() == 0x00) {

		return;
	
	}
	if (arch::checkCPUID() == 0x00) {

		return;

	}
	if (arch::checkLongMode() == 0x00) {

		return;
	
	}

	*(t_u16*)0xb8002 = (t_u16)0x2f4b;

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

	*(t_u16*)0xb8004 = (t_u16)0x2f41;
	*(t_u16*)0xb8006 = (t_u16)0x2f59;

}

