#include <include/types.hpp>
#include <include/idt.hpp>
#include <include/port.hpp>


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

	arch::idtEntry idtTable[256];
	idtTable[0x21]	= arch::idtSetEntry(0x00, 0x08, 0x8E);

	arch::idtPointer idt;
	idt.size	= 0;
	idt.pointer	= idtTable;

	arch::outPortB(0x20 , 0x11);
	arch::outPortB(0xA0 , 0x11);

	arch::outPortB(0x21 , 0x20);
	arch::outPortB(0xA1 , 0x28);

	arch::outPortB(0x21 , 0x00);  
	arch::outPortB(0xA1 , 0x00);  

	arch::outPortB(0x21 , 0x01);
	arch::outPortB(0xA1 , 0x01);

	arch::outPortB(0x21 , 0xff);
	arch::outPortB(0xA1 , 0xff);

	arch::idtLoad(idt);

	*(t_u16*)0xb8000 = (t_u16)0x2f4f;
	*(t_u16*)0xb8002 = (t_u16)0x2f4b;
	*(t_u16*)0xb8004 = (t_u16)0x2f41;
	*(t_u16*)0xb8006 = (t_u16)0x2f59;

}

