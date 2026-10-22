////////////////////////////////////////////////////////////////
//
//	CPU feature control
//
//	File:	features.cpp
//	Date:	22 Oct 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


// IgrOS-Kernel arch x86
#include <arch/x86/features.hpp>

#if	defined (IGROS_ARCH_x86_64)

// IgrOS-Kernel arch x86_64
#include <arch/x86_64/cpuid.hpp>
#include <arch/x86_64/msr.hpp>

#endif


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus


	// Enable no-execute page protection if CPU supports it
	[[nodiscard]]
	auto cpuEnableNoExecute() noexcept -> bool {

#if	defined (IGROS_ARCH_x86_64)

		using namespace igros;
		using namespace igros::x86_64;

		// CPUID.80000001h:EDX.NX
		constexpr auto CPUID_EDX_NX {1_u32 << 20};
		if (!cpuidHasLeaf(CPUID::EXTENDED_INFO) || (0_u32 == (cpuid(CPUID::EXTENDED_INFO).edx & CPUID_EDX_NX))) [[unlikely]] {
			return false;
		}
		// Set EFER.NXE: NX bit in page tables is reserved (faults) until then
		writeMSR(MSR::EFER, readMSR(MSR::EFER) | static_cast<igros_quad_t>(EFER::NXE));
		return true;

#else

		// No PAE page tables on i386 - no NX bit
		return false;

#endif

	}


#ifdef	__cplusplus

}	// extern "C"

#endif	// __cplusplus

