////////////////////////////////////////////////////////////////
//
//	Interrupt descriptor table low-level operations
//
//	File:	idt.cpp
//	Date:	08 Oct 2019
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
	idtEntry_t idtSetEntry(const idtISRPtr_t offset, const word_t &selector, const byte_t &type) {

		return	idtEntry_t {

				.offsetLow	= word_t(dword_t(offset) & 0xFFFF),
				.selector	= selector,
				.reserved	= 0x00,
				.type		= type,
				.offsetHigh	= word_t((dword_t(offset) & 0xFFFF0000) >> 16)

			};

	}

	// Calculate IDT size
	constexpr dword_t idtCalcTableSize(const dword_t &numOfEntries) {
		return (numOfEntries * sizeof(idtEntry_t)) - 1;
	}

	// Setup IDT
	void idtSetup() {

		// Exceptions setup
		idtTable[0x00]	= idtSetEntry(exHandler00, 0x08, 0x8E);
		idtTable[0x01]	= idtSetEntry(exHandler01, 0x08, 0x8E);
		idtTable[0x02]	= idtSetEntry(exHandler02, 0x08, 0x8E);
		idtTable[0x03]	= idtSetEntry(exHandler03, 0x08, 0x8E);
		idtTable[0x04]	= idtSetEntry(exHandler04, 0x08, 0x8E);
		idtTable[0x05]	= idtSetEntry(exHandler05, 0x08, 0x8E);
		idtTable[0x06]	= idtSetEntry(exHandler06, 0x08, 0x8E);
		idtTable[0x07]	= idtSetEntry(exHandler07, 0x08, 0x8E);
		idtTable[0x08]	= idtSetEntry(exHandler08, 0x08, 0x8E);
		idtTable[0x09]	= idtSetEntry(exHandler09, 0x08, 0x8E);
		idtTable[0x0A]	= idtSetEntry(exHandler0A, 0x08, 0x8E);
		idtTable[0x0B]	= idtSetEntry(exHandler0B, 0x08, 0x8E);
		idtTable[0x0C]	= idtSetEntry(exHandler0C, 0x08, 0x8E);
		idtTable[0x0D]	= idtSetEntry(exHandler0D, 0x08, 0x8E);
		idtTable[0x0E]	= idtSetEntry(exHandler0E, 0x08, 0x8E);
		idtTable[0x0F]	= idtSetEntry(exHandler0F, 0x08, 0x8E);
		idtTable[0x10]	= idtSetEntry(exHandler10, 0x08, 0x8E);
		idtTable[0x11]	= idtSetEntry(exHandler11, 0x08, 0x8E);
		idtTable[0x12]	= idtSetEntry(exHandler12, 0x08, 0x8E);
		idtTable[0x13]	= idtSetEntry(exHandler13, 0x08, 0x8E);
		idtTable[0x14]	= idtSetEntry(exHandler14, 0x08, 0x8E);
		idtTable[0x15]	= idtSetEntry(exHandler15, 0x08, 0x8E);
		idtTable[0x16]	= idtSetEntry(exHandler16, 0x08, 0x8E);
		idtTable[0x17]	= idtSetEntry(exHandler17, 0x08, 0x8E);
		idtTable[0x18]	= idtSetEntry(exHandler18, 0x08, 0x8E);
		idtTable[0x19]	= idtSetEntry(exHandler19, 0x08, 0x8E);
		idtTable[0x1A]	= idtSetEntry(exHandler1A, 0x08, 0x8E);
		idtTable[0x1B]	= idtSetEntry(exHandler1B, 0x08, 0x8E);
		idtTable[0x1C]	= idtSetEntry(exHandler1C, 0x08, 0x8E);
		idtTable[0x1D]	= idtSetEntry(exHandler1D, 0x08, 0x8E);
		idtTable[0x1E]	= idtSetEntry(exHandler1E, 0x08, 0x8E);
		idtTable[0x1F]	= idtSetEntry(exHandler1F, 0x08, 0x8E);

		// IRQs setup
		idtTable[0x20]	= idtSetEntry(irqHandler0, 0x08, 0x8E);
		idtTable[0x21]	= idtSetEntry(irqHandler1, 0x08, 0x8E);
		idtTable[0x22]	= idtSetEntry(irqHandler2, 0x08, 0x8E);
		idtTable[0x23]	= idtSetEntry(irqHandler3, 0x08, 0x8E);
		idtTable[0x24]	= idtSetEntry(irqHandler4, 0x08, 0x8E);
		idtTable[0x25]	= idtSetEntry(irqHandler5, 0x08, 0x8E);
		idtTable[0x26]	= idtSetEntry(irqHandler6, 0x08, 0x8E);
		idtTable[0x27]	= idtSetEntry(irqHandler7, 0x08, 0x8E);
		idtTable[0x28]	= idtSetEntry(irqHandler8, 0x08, 0x8E);
		idtTable[0x29]	= idtSetEntry(irqHandler9, 0x08, 0x8E);
		idtTable[0x2A]	= idtSetEntry(irqHandlerA, 0x08, 0x8E);
		idtTable[0x2B]	= idtSetEntry(irqHandlerB, 0x08, 0x8E);
		idtTable[0x2C]	= idtSetEntry(irqHandlerC, 0x08, 0x8E);
		idtTable[0x2D]	= idtSetEntry(irqHandlerD, 0x08, 0x8E);
		idtTable[0x2E]	= idtSetEntry(irqHandlerE, 0x08, 0x8E);
		idtTable[0x2F]	= idtSetEntry(irqHandlerF, 0x08, 0x8E);

		// Set IDT size and data pointer
		idt.size	= idtCalcTableSize(IDT_SIZE);
		idt.pointer	= idtTable;

		// Load new IDT
		idtLoad(&idt);

	}


}	// namespace arch

