////////////////////////////////////////////////////////////////
//
//	CPUID detection
//
//	File:	cpuid.hpp
//	Date:	13 Jul 2020
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <arch/x86_64/types.hpp>


// x86_64 namespace
namespace igros::x86_64 {


	// CPUID EAX value (e.g. flag)
	enum class cpuidFlags_t : dword_t {

		// "Intel" features list
		FEATURES_INTEL		= 0x00000000,		//
		INFO_PROC_VERSION	= 0x00000001,		//
		INFO_CACHE_TLB		= 0x00000002,		//
		INFO_PENTIUM_III_SERIAL	= 0x00000003,		//

		// "AMD" features list
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
	[[nodiscard]] bool cpuidCheck() noexcept;

	// CPUID instruction call
	[[nodiscard]] cpuidRegs_t cpuid(const cpuidFlags_t flag) noexcept;


}	// namespace igros::x86_64

