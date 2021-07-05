////////////////////////////////////////////////////////////////
//
//	Interrupt descriptor table low-level operations
//
//	File:	idt.cpp
//	Date:	11 Feb 2021
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#include <arch/x86_64/idt.hpp>


// x86_64 namespace
namespace igros::x86_64 {


	// Exceptions and IRQ descriptors table (IDT)
	std::array<idtEntryx86_64_t, idt::IDT_SIZE> idt::table;

	// Pointer to IDT
	idtPointerx86_64_t idt::pointer {
		idt::calcSize(),
		idt::table.cbegin()
	};


}	// namespace igros::x86_64

