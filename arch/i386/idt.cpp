////////////////////////////////////////////////////////////////
//
//	Interrupt descriptor table low-level operations
//
//	File:	idt.cpp
//	Date:	07 Feb 2021
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#include <arch/i386/idt.hpp>


// i386 namespace
namespace igros::i386 {


	// Exceptions and IRQ descriptors table (IDT)
	std::array<idtEntryi386_t, idti386::IDT_SIZE>	idti386::table;
	// Pointer to IDT
	idtPointeri386_t				idti386::pointer;



}	// namespace igros::i386

