////////////////////////////////////////////////////////////////
//
//	Interrupt descriptor table low-level operations
//
//	File:	idt.cpp
//	Date:	18 Jun. 2018
//
//	Copyright (c) 2018, Igor Baklykov
//	All rights reserved.
//


#include <include/idt.hpp>
#include <include/exceptions.hpp>
#include <include/interrupts.hpp>


namespace arch {


	// Exceptions and IRQ descriptors table (IDT)
	static idtEntry		idtTable[IDT_SIZE];
	// Pointer to IDT
	static idtPointer	idt;


	// Create IDT entry
	idtEntry idtSetEntry(const t_u32 &offset, const t_u16 &selector, const t_u8 &type) {

		idtEntry entry;	

		entry.offsetLow		= offset & 0xFFFF;
		entry.selector		= selector;
		entry.reserved		= 0x00;
		entry.type		= type;
		entry.offsetHigh	= (offset & 0xFFFF0000) >> 16;

		return entry;

	}

	// Calculate IDT size
	t_u32 idtCalcTableSize(const t_u32 &numOfEntries) {

		return (numOfEntries * sizeof(idtEntry)) - 1;

	}

	// Setup IDT
	void idtSetup() {

		// Exceptions setup
		idtTable[0x00]	= idtSetEntry((t_u32)exHandler00, 0x08, 0x8E);
		idtTable[0x01]	= idtSetEntry((t_u32)exHandler01, 0x08, 0x8E);
		idtTable[0x02]	= idtSetEntry((t_u32)exHandler02, 0x08, 0x8E);
		idtTable[0x03]	= idtSetEntry((t_u32)exHandler03, 0x08, 0x8E);
		idtTable[0x04]	= idtSetEntry((t_u32)exHandler04, 0x08, 0x8E);
		idtTable[0x05]	= idtSetEntry((t_u32)exHandler05, 0x08, 0x8E);
		idtTable[0x06]	= idtSetEntry((t_u32)exHandler06, 0x08, 0x8E);
		idtTable[0x07]	= idtSetEntry((t_u32)exHandler07, 0x08, 0x8E);
		idtTable[0x08]	= idtSetEntry((t_u32)exHandler08, 0x08, 0x8E);
		idtTable[0x09]	= idtSetEntry((t_u32)exHandler09, 0x08, 0x8E);
		idtTable[0x0A]	= idtSetEntry((t_u32)exHandler0A, 0x08, 0x8E);
		idtTable[0x0B]	= idtSetEntry((t_u32)exHandler0B, 0x08, 0x8E);
		idtTable[0x0C]	= idtSetEntry((t_u32)exHandler0C, 0x08, 0x8E);
		idtTable[0x0D]	= idtSetEntry((t_u32)exHandler0D, 0x08, 0x8E);
		idtTable[0x0E]	= idtSetEntry((t_u32)exHandler0E, 0x08, 0x8E);
		idtTable[0x0F]	= idtSetEntry((t_u32)exHandler0F, 0x08, 0x8E);
		idtTable[0x10]	= idtSetEntry((t_u32)exHandler10, 0x08, 0x8E);
		idtTable[0x11]	= idtSetEntry((t_u32)exHandler11, 0x08, 0x8E);
		idtTable[0x12]	= idtSetEntry((t_u32)exHandler12, 0x08, 0x8E);
		idtTable[0x13]	= idtSetEntry((t_u32)exHandler13, 0x08, 0x8E);
		idtTable[0x14]	= idtSetEntry((t_u32)exHandler14, 0x08, 0x8E);
		idtTable[0x15]	= idtSetEntry((t_u32)exHandler15, 0x08, 0x8E);
		idtTable[0x16]	= idtSetEntry((t_u32)exHandler16, 0x08, 0x8E);
		idtTable[0x17]	= idtSetEntry((t_u32)exHandler17, 0x08, 0x8E);
		idtTable[0x18]	= idtSetEntry((t_u32)exHandler18, 0x08, 0x8E);
		idtTable[0x19]	= idtSetEntry((t_u32)exHandler19, 0x08, 0x8E);
		idtTable[0x1A]	= idtSetEntry((t_u32)exHandler1A, 0x08, 0x8E);
		idtTable[0x1B]	= idtSetEntry((t_u32)exHandler1B, 0x08, 0x8E);
		idtTable[0x1C]	= idtSetEntry((t_u32)exHandler1C, 0x08, 0x8E);
		idtTable[0x1D]	= idtSetEntry((t_u32)exHandler1D, 0x08, 0x8E);
		idtTable[0x1E]	= idtSetEntry((t_u32)exHandler1E, 0x08, 0x8E);
		idtTable[0x1F]	= idtSetEntry((t_u32)exHandler1F, 0x08, 0x8E);

		// IRQs setup
		idtTable[0x20]	= idtSetEntry((t_u32)irqHandler0, 0x08, 0x8E);
		idtTable[0x21]	= idtSetEntry((t_u32)irqHandler1, 0x08, 0x8E);
		idtTable[0x22]	= idtSetEntry((t_u32)irqHandler2, 0x08, 0x8E);
		idtTable[0x23]	= idtSetEntry((t_u32)irqHandler3, 0x08, 0x8E);
		idtTable[0x24]	= idtSetEntry((t_u32)irqHandler4, 0x08, 0x8E);
		idtTable[0x25]	= idtSetEntry((t_u32)irqHandler5, 0x08, 0x8E);
		idtTable[0x26]	= idtSetEntry((t_u32)irqHandler6, 0x08, 0x8E);
		idtTable[0x27]	= idtSetEntry((t_u32)irqHandler7, 0x08, 0x8E);
		idtTable[0x28]	= idtSetEntry((t_u32)irqHandler8, 0x08, 0x8E);
		idtTable[0x29]	= idtSetEntry((t_u32)irqHandler9, 0x08, 0x8E);
		idtTable[0x2A]	= idtSetEntry((t_u32)irqHandlerA, 0x08, 0x8E);
		idtTable[0x2B]	= idtSetEntry((t_u32)irqHandlerB, 0x08, 0x8E);
		idtTable[0x2C]	= idtSetEntry((t_u32)irqHandlerC, 0x08, 0x8E);
		idtTable[0x2D]	= idtSetEntry((t_u32)irqHandlerD, 0x08, 0x8E);
		idtTable[0x2E]	= idtSetEntry((t_u32)irqHandlerE, 0x08, 0x8E);
		idtTable[0x2F]	= idtSetEntry((t_u32)irqHandlerF, 0x08, 0x8E);

		// Set IDT size and data pointer
		idt.size	= idtCalcTableSize(IDT_SIZE);
		idt.pointer	= idtTable;

		// Load new IDT
		idtLoad(&idt);

	}


}	// namespace arch

