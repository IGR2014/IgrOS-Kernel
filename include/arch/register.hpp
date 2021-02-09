////////////////////////////////////////////////////////////////
//
//	Architecture register type deduction
//
//	File:	types.hpp
//	Date:	03 Feb 2021
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
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
	using register_t = i386::registeri386_t;
#elif	defined (IGROS_ARCH_x86_64)
	// Register type
	using register_t = x86_64::registerx86_64_t;
#else
	// Register type
	using register_t = void;
	static_assert(false, u8"Unknown architecture!!!");
#endif


}	// namespace igros::arch

