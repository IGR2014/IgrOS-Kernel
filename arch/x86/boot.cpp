#include <include/types.hpp>
#include <include/idt.hpp>
#include <include/exception.hpp>
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

	idtTable[0x00]	= arch::idtSetEntry((t_u32)arch::isrException0, 0x08, 0x8E);
	idtTable[0x01]	= arch::idtSetEntry((t_u32)arch::isrException1, 0x08, 0x8E);
	idtTable[0x02]	= arch::idtSetEntry((t_u32)arch::isrException2, 0x08, 0x8E);
	idtTable[0x03]	= arch::idtSetEntry((t_u32)arch::isrException3, 0x08, 0x8E);
	idtTable[0x04]	= arch::idtSetEntry((t_u32)arch::isrException4, 0x08, 0x8E);
	idtTable[0x05]	= arch::idtSetEntry((t_u32)arch::isrException5, 0x08, 0x8E);
	idtTable[0x06]	= arch::idtSetEntry((t_u32)arch::isrException6, 0x08, 0x8E);
	idtTable[0x07]	= arch::idtSetEntry((t_u32)arch::isrException7, 0x08, 0x8E);
	idtTable[0x08]	= arch::idtSetEntry((t_u32)arch::isrException8, 0x08, 0x8E);
	idtTable[0x09]	= arch::idtSetEntry((t_u32)arch::isrException9, 0x08, 0x8E);
	idtTable[0x0A]	= arch::idtSetEntry((t_u32)arch::isrException10, 0x08, 0x8E);
	idtTable[0x0B]	= arch::idtSetEntry((t_u32)arch::isrException11, 0x08, 0x8E);
	idtTable[0x0C]	= arch::idtSetEntry((t_u32)arch::isrException12, 0x08, 0x8E);
	idtTable[0x0D]	= arch::idtSetEntry((t_u32)arch::isrException13, 0x08, 0x8E);
	idtTable[0x0E]	= arch::idtSetEntry((t_u32)arch::isrException14, 0x08, 0x8E);
	idtTable[0x0F]	= arch::idtSetEntry((t_u32)arch::isrException15, 0x08, 0x8E);
	idtTable[0x10]	= arch::idtSetEntry((t_u32)arch::isrException16, 0x08, 0x8E);
	idtTable[0x11]	= arch::idtSetEntry((t_u32)arch::isrException17, 0x08, 0x8E);
	idtTable[0x12]	= arch::idtSetEntry((t_u32)arch::isrException18, 0x08, 0x8E);
	idtTable[0x13]	= arch::idtSetEntry((t_u32)arch::isrException19, 0x08, 0x8E);
	idtTable[0x14]	= arch::idtSetEntry((t_u32)arch::isrException20, 0x08, 0x8E);
	idtTable[0x15]	= arch::idtSetEntry((t_u32)arch::isrException21, 0x08, 0x8E);
	idtTable[0x16]	= arch::idtSetEntry((t_u32)arch::isrException22, 0x08, 0x8E);
	idtTable[0x17]	= arch::idtSetEntry((t_u32)arch::isrException23, 0x08, 0x8E);
	idtTable[0x18]	= arch::idtSetEntry((t_u32)arch::isrException24, 0x08, 0x8E);
	idtTable[0x19]	= arch::idtSetEntry((t_u32)arch::isrException25, 0x08, 0x8E);
	idtTable[0x1A]	= arch::idtSetEntry((t_u32)arch::isrException26, 0x08, 0x8E);
	idtTable[0x1B]	= arch::idtSetEntry((t_u32)arch::isrException27, 0x08, 0x8E);
	idtTable[0x1C]	= arch::idtSetEntry((t_u32)arch::isrException28, 0x08, 0x8E);
	idtTable[0x1D]	= arch::idtSetEntry((t_u32)arch::isrException29, 0x08, 0x8E);
	idtTable[0x1E]	= arch::idtSetEntry((t_u32)arch::isrException30, 0x08, 0x8E);
	idtTable[0x1F]	= arch::idtSetEntry((t_u32)arch::isrException31, 0x08, 0x8E);

	//idtTable[0x21]	= arch::idtSetEntry(0x00, 0x08, 0x8E);

	arch::idtPointer idt;
	idt.size	= sizeof(arch::idtEntry) * 256 - 1;
	idt.pointer	= idtTable;

	arch::outPortB(0x20, 0x11);
	arch::outPortB(0xA0, 0x11);

	arch::outPortB(0x21, 0x20);
	arch::outPortB(0xA1, 0x28);

	arch::outPortB(0x21, 0x00);  
	arch::outPortB(0xA1, 0x00);  

	arch::outPortB(0x21, 0x01);
	arch::outPortB(0xA1, 0x01);

	arch::outPortB(0x21, 0xff);
	arch::outPortB(0xA1, 0xff);

	arch::idtLoad(&idt);

	*(t_u16*)0xb8000 = (t_u16)0x2f4f;
	*(t_u16*)0xb8002 = (t_u16)0x2f4b;
	*(t_u16*)0xb8004 = (t_u16)0x2f41;
	*(t_u16*)0xb8006 = (t_u16)0x2f59;

	// Divide by Zero Exception Test
	int x = 10;
	int y = 0;
	int z = x / y;

}
