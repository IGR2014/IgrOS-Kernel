////////////////////////////////////////////////////////////////
//
//	x86 family current (native) architecture selection
//
//	File:	native.hpp
//	Date:	09 Oct 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#if	defined (IGROS_ARCH_i386)

// IgrOS-Kernel arch i386
#include <arch/i386/cpu.hpp>
#include <arch/i386/register.hpp>

#elif	defined (IGROS_ARCH_x86_64)

// IgrOS-Kernel arch x86_64
#include <arch/x86_64/cpu.hpp>
#include <arch/x86_64/register.hpp>

#else

#error "x86 code built for non-x86 architecture!"

#endif


// x86 namespace
namespace igros::x86 {


#if	defined (IGROS_ARCH_i386)

	// Current architecture
	namespace native = igros::i386;
	// Current architecture name
	constexpr auto ARCH_NAME {"i386"};

#elif	defined (IGROS_ARCH_x86_64)

	// Current architecture
	namespace native = igros::x86_64;
	// Current architecture name
	constexpr auto ARCH_NAME {"x86_64"};

#endif


	// CPU of current architecture
	using cpu		= native::cpu;
	// Registers of current architecture
	using register_t	= native::register_t;


}	// namespace igros::x86

