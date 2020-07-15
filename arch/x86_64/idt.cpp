////////////////////////////////////////////////////////////////
//
//	Interrupt descriptor table low-level operations
//
//	File:	idt.cpp
//	Date:	11 Jul 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#include <arch/x86_64/idt.hpp>


// x86_64 namespace
namespace igros::x86_64 {


	// Exceptions and IRQ descriptors table (IDT)
	idtEntry_t	idt::table[IDT_SIZE];
	// Pointer to IDT
	idtPointer_t	idt::pointer;


}	// namespace igros::x86_64

