//#include <videoMem.hpp>
#include <include/types.hpp>
#include <include/gdt.hpp>
#include <include/longMode.hpp>

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

	if (arch::checkMultiboot() == 0x00) {

		return;
	
	}
	if (arch::checkCPUID() == 0x00) {

		return;

	}
	if (arch::checkLongMode() == 0x00) {

		return;
	
	}

	*(t_u16*)0xb8000 = (t_u16)0x2f4f;
	*(t_u16*)0xb8002 = (t_u16)0x2f4b;

	arch::setupPageTables();
	arch::enablePaging();

	t_u64 gdtTable[5];
	gdtTable[0] = arch::gdtCreateEntry(0x00000000, 0x00000000, 0x0000);
	gdtTable[1] = arch::gdtCreateEntry(0x00000000, 0x000FFFFF, GDT_CODE_RING0);
	gdtTable[2] = arch::gdtCreateEntry(0x00000000, 0x000FFFFF, GDT_DATA_RING0);
	gdtTable[3] = arch::gdtCreateEntry(0x00000000, 0x000FFFFF, GDT_CODE_RING3);
	gdtTable[4] = arch::gdtCreateEntry(0x00000000, 0x000FFFFF, GDT_DATA_RING3);

	arch::gdtPtr gdt_p;
	gdt_p.size	= (5 << 6) - 1;
	gdt_p.pointer	= (t_u64)gdtTable;

	arch::gdtLoad((t_u64)&gdt_p);

	*(t_u16*)0xb8004 = (t_u16)0x2f41;
	*(t_u16*)0xb8006 = (t_u16)0x2f59;

}

