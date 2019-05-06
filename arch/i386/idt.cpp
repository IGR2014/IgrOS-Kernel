////////////////////////////////////////////////////////////////
//
//	Interrupt descriptor table low-level operations
//
//	File:	idt.cpp
//	Date:	06 May 2019
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
//	All rights reserved.
//
//


#include <arch/idt.hpp>
#include <arch/exceptions.hpp>
#include <arch/interrupts.hpp>


namespace arch {


	// Exceptions and IRQ descriptors table (IDT)
	static idtEntry_t	idtTable[IDT_SIZE];
	// Pointer to IDT
	static idtPointer_t	idt;


	// Create IDT entry
	idtEntry_t idtSetEntry(const dword_t &offset, const word_t &selector, const byte_t &type) {

		idtEntry_t entry;	

		entry.offsetLow		= offset & 0xFFFF;
		entry.selector		= selector;
		entry.reserved		= 0x00;
		entry.type		= type;
		entry.offsetHigh	= (offset & 0xFFFF0000) >> 16;

		return entry;

	}

	// Calculate IDT size
	dword_t idtCalcTableSize(const dword_t &numOfEntries) {

		return (numOfEntries * sizeof(idtEntry_t)) - 1;

	}

	// Setup IDT
	void idtSetup() {

		// Exceptions setup
		idtTable[0x00]	= idtSetEntry((dword_t)exHandler00, 0x08, 0x8E);
		idtTable[0x01]	= idtSetEntry((dword_t)exHandler01, 0x08, 0x8E);
		idtTable[0x02]	= idtSetEntry((dword_t)exHandler02, 0x08, 0x8E);
		idtTable[0x03]	= idtSetEntry((dword_t)exHandler03, 0x08, 0x8E);
		idtTable[0x04]	= idtSetEntry((dword_t)exHandler04, 0x08, 0x8E);
		idtTable[0x05]	= idtSetEntry((dword_t)exHandler05, 0x08, 0x8E);
		idtTable[0x06]	= idtSetEntry((dword_t)exHandler06, 0x08, 0x8E);
		idtTable[0x07]	= idtSetEntry((dword_t)exHandler07, 0x08, 0x8E);
		idtTable[0x08]	= idtSetEntry((dword_t)exHandler08, 0x08, 0x8E);
		idtTable[0x09]	= idtSetEntry((dword_t)exHandler09, 0x08, 0x8E);
		idtTable[0x0A]	= idtSetEntry((dword_t)exHandler0A, 0x08, 0x8E);
		idtTable[0x0B]	= idtSetEntry((dword_t)exHandler0B, 0x08, 0x8E);
		idtTable[0x0C]	= idtSetEntry((dword_t)exHandler0C, 0x08, 0x8E);
		idtTable[0x0D]	= idtSetEntry((dword_t)exHandler0D, 0x08, 0x8E);
		idtTable[0x0E]	= idtSetEntry((dword_t)exHandler0E, 0x08, 0x8E);
		idtTable[0x0F]	= idtSetEntry((dword_t)exHandler0F, 0x08, 0x8E);
		idtTable[0x10]	= idtSetEntry((dword_t)exHandler10, 0x08, 0x8E);
		idtTable[0x11]	= idtSetEntry((dword_t)exHandler11, 0x08, 0x8E);
		idtTable[0x12]	= idtSetEntry((dword_t)exHandler12, 0x08, 0x8E);
		idtTable[0x13]	= idtSetEntry((dword_t)exHandler13, 0x08, 0x8E);
		idtTable[0x14]	= idtSetEntry((dword_t)exHandler14, 0x08, 0x8E);
		idtTable[0x15]	= idtSetEntry((dword_t)exHandler15, 0x08, 0x8E);
		idtTable[0x16]	= idtSetEntry((dword_t)exHandler16, 0x08, 0x8E);
		idtTable[0x17]	= idtSetEntry((dword_t)exHandler17, 0x08, 0x8E);
		idtTable[0x18]	= idtSetEntry((dword_t)exHandler18, 0x08, 0x8E);
		idtTable[0x19]	= idtSetEntry((dword_t)exHandler19, 0x08, 0x8E);
		idtTable[0x1A]	= idtSetEntry((dword_t)exHandler1A, 0x08, 0x8E);
		idtTable[0x1B]	= idtSetEntry((dword_t)exHandler1B, 0x08, 0x8E);
		idtTable[0x1C]	= idtSetEntry((dword_t)exHandler1C, 0x08, 0x8E);
		idtTable[0x1D]	= idtSetEntry((dword_t)exHandler1D, 0x08, 0x8E);
		idtTable[0x1E]	= idtSetEntry((dword_t)exHandler1E, 0x08, 0x8E);
		idtTable[0x1F]	= idtSetEntry((dword_t)exHandler1F, 0x08, 0x8E);

		// IRQs setup
		idtTable[0x20]	= idtSetEntry((dword_t)irqHandler0, 0x08, 0x8E);
		idtTable[0x21]	= idtSetEntry((dword_t)irqHandler1, 0x08, 0x8E);
		idtTable[0x22]	= idtSetEntry((dword_t)irqHandler2, 0x08, 0x8E);
		idtTable[0x23]	= idtSetEntry((dword_t)irqHandler3, 0x08, 0x8E);
		idtTable[0x24]	= idtSetEntry((dword_t)irqHandler4, 0x08, 0x8E);
		idtTable[0x25]	= idtSetEntry((dword_t)irqHandler5, 0x08, 0x8E);
		idtTable[0x26]	= idtSetEntry((dword_t)irqHandler6, 0x08, 0x8E);
		idtTable[0x27]	= idtSetEntry((dword_t)irqHandler7, 0x08, 0x8E);
		idtTable[0x28]	= idtSetEntry((dword_t)irqHandler8, 0x08, 0x8E);
		idtTable[0x29]	= idtSetEntry((dword_t)irqHandler9, 0x08, 0x8E);
		idtTable[0x2A]	= idtSetEntry((dword_t)irqHandlerA, 0x08, 0x8E);
		idtTable[0x2B]	= idtSetEntry((dword_t)irqHandlerB, 0x08, 0x8E);
		idtTable[0x2C]	= idtSetEntry((dword_t)irqHandlerC, 0x08, 0x8E);
		idtTable[0x2D]	= idtSetEntry((dword_t)irqHandlerD, 0x08, 0x8E);
		idtTable[0x2E]	= idtSetEntry((dword_t)irqHandlerE, 0x08, 0x8E);
		idtTable[0x2F]	= idtSetEntry((dword_t)irqHandlerF, 0x08, 0x8E);

		// Set IDT size and data pointer
		idt.size	= idtCalcTableSize(IDT_SIZE);
		idt.pointer	= idtTable;

		// Load new IDT
		idtLoad(&idt);

	}


}	// namespace arch

