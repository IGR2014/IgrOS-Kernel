////////////////////////////////////////////////////////////////
//
//	CPUID detection
//
//	File:	cpuid.hpp
//	Date:	18 Jun 2019
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#pragma once
#ifndef IGROS_ARCH_CPUID_HPP
#define IGROS_ARCH_CPUID_HPP


#include <arch/types.hpp>


// Arch-dependent code zone
namespace arch {


	// CPUID EAX value (e.g. flag)
	enum class cpuidFlags_t : dword_t {

		// "Intel" features list
		FEATURES_INTEL		= 0x00000000,		//
		INFO_PROC_VERSION	= 0x00000001,		//
		INFO_CACHE_TLB		= 0x00000002,		//
		INFO_PENTIUM_III_SERIAL	= 0x00000003,		//

		FEATURES_AMD		= 0x80000000		//

	};


	// CPUID registers values holder
	struct cpuidRegs_t {

		dword_t		eax;			// EAX register value
		dword_t		ebx;			// EBX register value
		dword_t		ecx;			// ECX register value
		dword_t		edx;			// EDX register value

	};


	// Check if CPUID exists (Looks like on i386 not)
	bool		cpuidCheck();

	// CPUID instruction call
        cpuidRegs_t	cpuid(const cpuidFlags_t flag);


}	// namespace arch


#endif	// IGROS_ARCH_CPUID_HPP

