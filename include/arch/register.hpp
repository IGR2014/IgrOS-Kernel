////////////////////////////////////////////////////////////////
//
//	Architecture register type deduction
//
//	File:	register.hpp
//	Date:	05 Dec 2022
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// i386
#include <arch/i386/register.hpp>
// x86_64
#include <arch/x86_64/register.hpp>


// Arch namespace
namespace igros::arch {


#if	defined (IGROS_ARCH_i386)

	// Register type
	using	register_t	= i386::register_t;

#elif	defined (IGROS_ARCH_x86_64)

	// Register type
	using	register_t	= x86_64::register_t;

#else

	static_assert(
		false,
		"Unknown architecture!"
	);

	// Register type
	using	register_t	= void;

#endif


}	// namespace igros::arch

