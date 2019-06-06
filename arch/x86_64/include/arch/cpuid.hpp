////////////////////////////////////////////////////////////////
//
//	CPUID detection
//
//	File:	cpuid.hpp
//	Date:	06 Jun 2019
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
//	All rights reserved.
//
//


#pragma once
#ifndef IGROS_ARCH_CPUID_HPP
#define IGROS_ARCH_CPUID_HPP


#include <arch/types.hpp>


// Arch-dependent code zone
namespace arch {


	// CPUID registers values holder
	struct cpuidRegs_t {

		dword_t		EAX;		// EAX register value
		dword_t		EBX;		// EBX register value
		dword_t		ECX;		// ECX register value
		dword_t		EDX;		// EDX register value

	};


	// Check if CPUID exists (Looks like on i386 not)
	bool		cpuidCheck();

	// CPUID instruction call
        cpuidRegs_t	cpuid(const dword_t eax);


}	// namespace arch


#endif	// IGROS_ARCH_CPUID_HPP

