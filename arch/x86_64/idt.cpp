////////////////////////////////////////////////////////////////
//
//	Interrupt descriptor table low-level operations
//
//	File:	idt.cpp
//	Date:	20 Jan 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#include <arch/idt.hpp>


namespace arch {


	// Exceptions and IRQ descriptors table (IDT)
	idtEntry_t	idt::table[IDT_SIZE];
	// Pointer to IDT
	idtPointer_t	idt::pointer;


}	// namespace arch

