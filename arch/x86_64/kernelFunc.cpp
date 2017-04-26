//#include <videoMem.hpp>
#include <include/types.hpp>
#include <include/gdt.hpp>

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

	t_u64 gdt = arch::gdtCreateEntry(0xFFFFFFFF, 0x00000000, 0xAABB);
	//arch::gdtLoad(0x00);

	*(t_u16*)0xb8000 = (t_u16)0x2f4f;
	*(t_u16*)0xb8002 = (t_u16)0x2f4b;
	*(t_u16*)0xb8004 = (t_u16)0x2f41;
	*(t_u16*)0xb8006 = (t_u16)0x2f59;

}

