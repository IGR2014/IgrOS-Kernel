////////////////////////////////////////////////////////////////
///
///	@file
///	@brief		CPU operations
///	@date		21 Mar 2023
///
///	@copyright	Copyright (c) 2017 - 2026, Igor Baklykov
///			All rights reserved.
///	@author		Igor Baklykov
///


#pragma once


// IgrOS-Kernel arch
#include <arch/register.hpp>

#if	defined (IGROS_ARCH_i386) || defined (IGROS_ARCH_x86_64)

// IgrOS-Kernel arch x86
#include <arch/x86/native.hpp>

#else

#error "Unknown architecture!"

#endif


// Arch namespace
namespace igros::arch {


	/// @brief CPU control every architecture must provide
	template<class T>
	concept Cpu = requires(const register_t* const regs) {
		// Halt CPU (never returns)
		T::halt();
		// Dump CPU registers
		T::dumpRegisters(regs);
	};


#if	defined (IGROS_ARCH_i386) || defined (IGROS_ARCH_x86_64)

	/// @brief CPU type
	using x86::cpu;

#endif


	static_assert(Cpu<cpu>, "Architecture CPU doesn't provide CPU interface!");


}	// namespace igros::arch

