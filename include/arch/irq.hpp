////////////////////////////////////////////////////////////////
//
//	Architecture interrupts type deduction
//
//	File:	types.hpp
//	Date:	15 Jul 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#if	defined (IGROS_ARCH_i386)
#include <arch/i386/irq.hpp>
#elif	defined (IGROS_ARCH_x86_64)
#include <arch/x86_64/irq.hpp>
#endif


// Arch namespace
namespace igros::arch {


#if	defined (IGROS_ARCH_i386)

	// IRQ type
	using irq	= i386::irq;
	// IRQ index type
	using irq_t	= i386::irq_t;

#elif	defined (IGROS_ARCH_x86_64)

	// IRQ type
	using irq	= x86_64::irq;
	// IRQ index type
	using irq_t	= x86_64::irq_t;

#else

	// IRQ type
	using irq	= void;
	// IRQ index type
	using irq_t	= void;

#endif


}	// namespace igros::arch

