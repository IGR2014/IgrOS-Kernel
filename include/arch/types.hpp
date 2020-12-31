////////////////////////////////////////////////////////////////
//
//	Common C++ types redefinition
//
//	File:	types.hpp
//	Date:	13 Jul 2020
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


//#if	defined (IGROS_ARCH_i386)
#include <arch/i386/types.hpp>
#include <arch/i386/register.hpp>
//#elif	defined (IGROS_ARCH_x86_64)
#include <arch/x86_64/types.hpp>
#include <arch/x86_64/register.hpp>
//#endif


// OS namespace
namespace igros {


#if	defined (IGROS_ARCH_i386)

	using register_t = i386::register_t;

#elif	defined (IGROS_ARCH_x86_64)

	using register_t = x86_64::register_t;

#else

	using register_t = void;

#endif


}	// namespace igros

