////////////////////////////////////////////////////////////////
//
//	Interrupt descriptor table low-level operations
//
//	File:	idt.cpp
//	Date:	13 Mar 2023
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


// IgrOS-Kernel arch i386
#include <arch/i386/idt.hpp>


// i386 namespace
namespace igros::i386 {


	// Exceptions and IRQ descriptors table (IDT)
	std::array<idtEntry_t, idt::IDT_SIZE> idt::table {
		// Exceptions setup
		idt::setEntry<::exHandler00, 0x0008, 0x8E>(),
		idt::setEntry<::exHandler01, 0x0008, 0x8E>(),
		idt::setEntry<::exHandler02, 0x0008, 0x8E>(),
		idt::setEntry<::exHandler03, 0x0008, 0x8E>(),
		idt::setEntry<::exHandler04, 0x0008, 0x8E>(),
		idt::setEntry<::exHandler05, 0x0008, 0x8E>(),
		idt::setEntry<::exHandler06, 0x0008, 0x8E>(),
		idt::setEntry<::exHandler07, 0x0008, 0x8E>(),
		idt::setEntry<::exHandler08, 0x0008, 0x8E>(),
		idt::setEntry<::exHandler09, 0x0008, 0x8E>(),
		idt::setEntry<::exHandler0A, 0x0008, 0x8E>(),
		idt::setEntry<::exHandler0B, 0x0008, 0x8E>(),
		idt::setEntry<::exHandler0C, 0x0008, 0x8E>(),
		idt::setEntry<::exHandler0D, 0x0008, 0x8E>(),
		idt::setEntry<::exHandler0E, 0x0008, 0x8E>(),
		idt::setEntry<::exHandler0F, 0x0008, 0x8E>(),
		idt::setEntry<::exHandler10, 0x0008, 0x8E>(),
		idt::setEntry<::exHandler11, 0x0008, 0x8E>(),
		idt::setEntry<::exHandler12, 0x0008, 0x8E>(),
		idt::setEntry<::exHandler13, 0x0008, 0x8E>(),
		idt::setEntry<::exHandler14, 0x0008, 0x8E>(),
		idt::setEntry<::exHandler15, 0x0008, 0x8E>(),
		idt::setEntry<::exHandler16, 0x0008, 0x8E>(),
		idt::setEntry<::exHandler17, 0x0008, 0x8E>(),
		idt::setEntry<::exHandler18, 0x0008, 0x8E>(),
		idt::setEntry<::exHandler19, 0x0008, 0x8E>(),
		idt::setEntry<::exHandler1A, 0x0008, 0x8E>(),
		idt::setEntry<::exHandler1B, 0x0008, 0x8E>(),
		idt::setEntry<::exHandler1C, 0x0008, 0x8E>(),
		idt::setEntry<::exHandler1D, 0x0008, 0x8E>(),
		idt::setEntry<::exHandler1E, 0x0008, 0x8E>(),
		idt::setEntry<::exHandler1F, 0x0008, 0x8E>(),
		// IRQs setup
		idt::setEntry<::irqHandler0, 0x0008, 0x8E>(),
		idt::setEntry<::irqHandler1, 0x0008, 0x8E>(),
		idt::setEntry<::irqHandler2, 0x0008, 0x8E>(),
		idt::setEntry<::irqHandler3, 0x0008, 0x8E>(),
		idt::setEntry<::irqHandler4, 0x0008, 0x8E>(),
		idt::setEntry<::irqHandler5, 0x0008, 0x8E>(),
		idt::setEntry<::irqHandler6, 0x0008, 0x8E>(),
		idt::setEntry<::irqHandler7, 0x0008, 0x8E>(),
		idt::setEntry<::irqHandler8, 0x0008, 0x8E>(),
		idt::setEntry<::irqHandler9, 0x0008, 0x8E>(),
		idt::setEntry<::irqHandlerA, 0x0008, 0x8E>(),
		idt::setEntry<::irqHandlerB, 0x0008, 0x8E>(),
		idt::setEntry<::irqHandlerC, 0x0008, 0x8E>(),
		idt::setEntry<::irqHandlerD, 0x0008, 0x8E>(),
		idt::setEntry<::irqHandlerE, 0x0008, 0x8E>(),
		idt::setEntry<::irqHandlerF, 0x0008, 0x8E>()
	};

	// Pointer to IDT
	constinit idtPointer_t idt::pointer {
		idt::calcSize(),
		idt::table.cbegin()
	};



}	// namespace igros::i386

