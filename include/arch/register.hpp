////////////////////////////////////////////////////////////////
//
//	Architecture register type deduction
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
#include <arch/i386/register.hpp>
#elif	defined (IGROS_ARCH_x86_64)
#include <arch/x86_64/register.hpp>
#endif


// Arch namespace
namespace igros::arch {


#if	defined (IGROS_ARCH_i386)

	// Register type
	using register_t = i386::register_t;

#elif	defined (IGROS_ARCH_x86_64)

	// Register type
	using register_t = x86_64::register_t;

#else

	// Register type
	using register_t = void;

#endif


}	// namespace igros::arch

