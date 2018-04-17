////////////////////////////////////////////////////////////////
//
//	Boot low-level main setup function
//
//	File:	boot.cpp
//	Date:	17 Apr. 2018
//
//	Copyright (c) 2018, Igor Baklykov
//	All rights reserved.
//


#include <include/types.hpp>
#include <include/gdt.hpp>
#include <include/idt.hpp>
#include <include/exceptions.hpp>
#include <include/interrupts.hpp>
#include <include/port.hpp>
#include <include/videoMem.hpp>


// Exceptions and IRQ descriptors table (IDT)
arch::idtEntry idtTable[256];
// Pointer to IDT
arch::idtPointer idt;

// Global descriptors table (GDT)
arch::gdtEntry gdtTable[3];
// Pointer to GDT
arch::gdtPointer gdt;

// Kernel main function
extern "C" void kernelFunc() {

	//arch::setupPageTables();
	//arch::enablePaging();

	// GDT setup
	gdtTable[0x00]	= arch::gdtSetEntry(0x00000000, 0x00000000, 0x0000);
	gdtTable[0x01]	= arch::gdtSetEntry(0x00000000, 0xFFFFFFFF, GDT_CODE_RING0);
	gdtTable[0x02]	= arch::gdtSetEntry(0x00000000, 0xFFFFFFFF, GDT_DATA_RING0);

	// Set GDT size and data pointer
	gdt.size	= arch::gdtCalcTableSize(3);
	gdt.pointer	= gdtTable;

	// Load new GDT
	arch::gdtLoad(&gdt);
	//arch::reloadCS();

	// Exceptions setup
	idtTable[0x00]	= arch::idtSetEntry((t_u32)arch::exHandler00, 0x08, 0x8E);
	idtTable[0x01]	= arch::idtSetEntry((t_u32)arch::exHandler01, 0x08, 0x8E);
	idtTable[0x02]	= arch::idtSetEntry((t_u32)arch::exHandler02, 0x08, 0x8E);
	idtTable[0x03]	= arch::idtSetEntry((t_u32)arch::exHandler03, 0x08, 0x8E);
	idtTable[0x04]	= arch::idtSetEntry((t_u32)arch::exHandler04, 0x08, 0x8E);
	idtTable[0x05]	= arch::idtSetEntry((t_u32)arch::exHandler05, 0x08, 0x8E);
	idtTable[0x06]	= arch::idtSetEntry((t_u32)arch::exHandler06, 0x08, 0x8E);
	idtTable[0x07]	= arch::idtSetEntry((t_u32)arch::exHandler07, 0x08, 0x8E);
	idtTable[0x08]	= arch::idtSetEntry((t_u32)arch::exHandler08, 0x08, 0x8E);
	idtTable[0x09]	= arch::idtSetEntry((t_u32)arch::exHandler09, 0x08, 0x8E);
	idtTable[0x0A]	= arch::idtSetEntry((t_u32)arch::exHandler0A, 0x08, 0x8E);
	idtTable[0x0B]	= arch::idtSetEntry((t_u32)arch::exHandler0B, 0x08, 0x8E);
	idtTable[0x0C]	= arch::idtSetEntry((t_u32)arch::exHandler0C, 0x08, 0x8E);
	idtTable[0x0D]	= arch::idtSetEntry((t_u32)arch::exHandler0D, 0x08, 0x8E);
	idtTable[0x0E]	= arch::idtSetEntry((t_u32)arch::exHandler0E, 0x08, 0x8E);
	idtTable[0x0F]	= arch::idtSetEntry((t_u32)arch::exHandler0F, 0x08, 0x8E);
	idtTable[0x10]	= arch::idtSetEntry((t_u32)arch::exHandler10, 0x08, 0x8E);
	idtTable[0x11]	= arch::idtSetEntry((t_u32)arch::exHandler11, 0x08, 0x8E);
	idtTable[0x12]	= arch::idtSetEntry((t_u32)arch::exHandler12, 0x08, 0x8E);
	idtTable[0x13]	= arch::idtSetEntry((t_u32)arch::exHandler13, 0x08, 0x8E);
	idtTable[0x14]	= arch::idtSetEntry((t_u32)arch::exHandler14, 0x08, 0x8E);
	idtTable[0x15]	= arch::idtSetEntry((t_u32)arch::exHandler15, 0x08, 0x8E);
	idtTable[0x16]	= arch::idtSetEntry((t_u32)arch::exHandler16, 0x08, 0x8E);
	idtTable[0x17]	= arch::idtSetEntry((t_u32)arch::exHandler17, 0x08, 0x8E);
	idtTable[0x18]	= arch::idtSetEntry((t_u32)arch::exHandler18, 0x08, 0x8E);
	idtTable[0x19]	= arch::idtSetEntry((t_u32)arch::exHandler19, 0x08, 0x8E);
	idtTable[0x1A]	= arch::idtSetEntry((t_u32)arch::exHandler1A, 0x08, 0x8E);
	idtTable[0x1B]	= arch::idtSetEntry((t_u32)arch::exHandler1B, 0x08, 0x8E);
	idtTable[0x1C]	= arch::idtSetEntry((t_u32)arch::exHandler1C, 0x08, 0x8E);
	idtTable[0x1D]	= arch::idtSetEntry((t_u32)arch::exHandler1D, 0x08, 0x8E);
	idtTable[0x1E]	= arch::idtSetEntry((t_u32)arch::exHandler1E, 0x08, 0x8E);
	idtTable[0x1F]	= arch::idtSetEntry((t_u32)arch::exHandler1F, 0x08, 0x8E);

	// IRQs setup
	idtTable[0x20]	= arch::idtSetEntry((t_u32)arch::irqHandler0, 0x08, 0x8E);
	idtTable[0x21]	= arch::idtSetEntry((t_u32)arch::irqHandler1, 0x08, 0x8E);
	idtTable[0x22]	= arch::idtSetEntry((t_u32)arch::irqHandler2, 0x08, 0x8E);
	idtTable[0x23]	= arch::idtSetEntry((t_u32)arch::irqHandler3, 0x08, 0x8E);
	idtTable[0x24]	= arch::idtSetEntry((t_u32)arch::irqHandler4, 0x08, 0x8E);
	idtTable[0x25]	= arch::idtSetEntry((t_u32)arch::irqHandler5, 0x08, 0x8E);
	idtTable[0x26]	= arch::idtSetEntry((t_u32)arch::irqHandler6, 0x08, 0x8E);
	idtTable[0x27]	= arch::idtSetEntry((t_u32)arch::irqHandler7, 0x08, 0x8E);
	idtTable[0x28]	= arch::idtSetEntry((t_u32)arch::irqHandler8, 0x08, 0x8E);
	idtTable[0x29]	= arch::idtSetEntry((t_u32)arch::irqHandler9, 0x08, 0x8E);
	idtTable[0x2A]	= arch::idtSetEntry((t_u32)arch::irqHandlerA, 0x08, 0x8E);
	idtTable[0x2B]	= arch::idtSetEntry((t_u32)arch::irqHandlerB, 0x08, 0x8E);
	idtTable[0x2C]	= arch::idtSetEntry((t_u32)arch::irqHandlerC, 0x08, 0x8E);
	idtTable[0x2D]	= arch::idtSetEntry((t_u32)arch::irqHandlerD, 0x08, 0x8E);
	idtTable[0x2E]	= arch::idtSetEntry((t_u32)arch::irqHandlerE, 0x08, 0x8E);
	idtTable[0x2F]	= arch::idtSetEntry((t_u32)arch::irqHandlerF, 0x08, 0x8E);

	// Set IDT size and data pointer
	idt.size	= arch::idtCalcTableSize(256);
	idt.pointer	= idtTable;

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

	// Load new IDT
	arch::idtLoad(&idt);

	// Init video memory
	arch::videoMemInit();

	// Write "Hello World" message
	arch::videoMemWriteLine("Hello World from kernel!");

	// Divide by Zero Exception Test
	/*	
	int x = 10;
	int y = 0;
	int z = x / y;
	*/

	while(true) {};

}

