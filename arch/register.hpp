////////////////////////////////////////////////////////////////
///
///	@file
///	@brief		Architecture register type deduction
///	@date		03 Nov 2026
///
///	@copyright	Copyright (c) 2017 - 2022, Igor Baklykov
///			All rights reserved.
///	@author		Igor Baklykov
///


#pragma once


// IgrOS-Kernel arch i386
#include <arch/i386/register.hpp>
// IgrOS-Kernel arch x86_64
#include <arch/x86_64/register.hpp>


// Arch namespace
namespace igros::arch {


#if defined(IGROS_ARCH_i386)

	/// @brief Register type
	using i386::register_t;

#elif defined(IGROS_ARCH_x86_64)

	/// @brief Register type
	using x86_64::register_t;

#else

#error "Unknown architecture!"

#endif


} // namespace igros::arch

