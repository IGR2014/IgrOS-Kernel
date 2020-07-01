////////////////////////////////////////////////////////////////
//
//	Interrupt descriptor table low-level operations
//
//	File:	idt.cpp
//	Date:	30 Jun 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#include <idt.hpp>


namespace igros::arch {


	// Exceptions and IRQ descriptors table (IDT)
	idtEntry_t	idt::table[IDT_SIZE];
	// Pointer to IDT
	idtPointer_t	idt::pointer;



}	// namespace igros::arch

