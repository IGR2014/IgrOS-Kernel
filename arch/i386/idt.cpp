////////////////////////////////////////////////////////////////
//
//	Interrupt descriptor table low-level operations
//
//	File:	idt.cpp
//	Date:	11 Jul 2020
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#include <arch/i386/idt.hpp>


// i386 namespace
namespace igros::i386 {


	// Exceptions and IRQ descriptors table (IDT)
	idtEntry_t	idt::table[IDT_SIZE];
	// Pointer to IDT
	idtPointer_t	idt::pointer;



}	// namespace igros::i386

