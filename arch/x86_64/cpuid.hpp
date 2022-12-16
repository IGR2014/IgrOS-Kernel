////////////////////////////////////////////////////////////////
//
//	CPUID detection
//
//	File:	cpuid.hpp
//	Date:	16 Dec 2022
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// IgrOS-Kernel arch
#include <arch/types.hpp>


// x86_64 namespace
namespace igros::x86_64 {


	// CPUID EAX value (e.g. flag)
	enum class cpuidFlags_t : igros_dword_t {

		// "Intel" features list
		FEATURES_INTEL		= 0x00000000_u32,		//
		INFO_PROC_VERSION	= 0x00000001_u32,		//
		INFO_CACHE_TLB		= 0x00000002_u32,		//
		INFO_PENTIUM_III_SERIAL	= 0x00000003_u32,		//

		// "AMD" features list
		FEATURES_AMD		= 0x80000000_u32		//

	};


	// CPUID registers values holder
	struct cpuidRegs_t {
		igros_dword_t		eax;			// EAX register value
		igros_dword_t		ebx;			// EBX register value
		igros_dword_t		ecx;			// ECX register value
		igros_dword_t		edx;			// EDX register value
	};


	// Check if CPUID exists (Looks like on i386 not)
	[[nodiscard]]
	auto	cpuidCheck() noexcept -> bool;

	// CPUID instruction call
	[[nodiscard]]
	auto	cpuid(const cpuidFlags_t flag) noexcept -> cpuidRegs_t;


}	// namespace igros::x86_64

