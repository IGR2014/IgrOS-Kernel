////////////////////////////////////////////////////////////////
//
//	Common C++ types redefinition
//
//	File:	types.hpp
//	Date:	24 Sep 2021
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// i386
#include <arch/i386/types.hpp>
#include <arch/i386/register.hpp>
// x86_64
#include <arch/x86_64/types.hpp>
#include <arch/x86_64/register.hpp>


// OS namespace
namespace igros {


#if	defined (IGROS_ARCH_i386)
	// Paging type
	using register_t = i386::register_t;
#elif	defined (IGROS_ARCH_x86_64)
	// Paging type
	using register_t = x86_64::register_t;
#else
	// Paging type
	using register_t = void;
	static_assert(false, "Unknown architecture!!!");
#endif


}	// namespace igros

