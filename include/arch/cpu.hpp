////////////////////////////////////////////////////////////////
//
//	CPU operations
//
//	File:	cpu.hpp
//	Date:	15 Jul 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#if	defined (IGROS_ARCH_i386)
#include <arch/i386/cpu.hpp>
#elif	defined (IGROS_ARCH_x86_64)
#include <arch/x86_64/cpu.hpp>
#endif


// Arch namespace
namespace igros::arch {


#if	defined (IGROS_ARCH_i386)

	// CPU type
	using cpu = i386::cpu;

#elif	defined (IGROS_ARCH_x86_64)

	// CPU type
	using cpu = x86_64::cpu;

#else

	// CPU type
	using cpu = void;

#endif


}	// namespace igros::arch

