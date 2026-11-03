////////////////////////////////////////////////////////////////
///
///	@file
///	@brief		CPU feature control
///	@date		03 Nov 2026
///
///	@copyright	Copyright (c) 2017 - 2026, Igor Baklykov
///			All rights reserved.
///	@author		Igor Baklykov
///


// IgrOS-Kernel arch x86
#include <arch/x86/features.hpp>

#if defined(IGROS_ARCH_x86_64)

// IgrOS-Kernel arch x86_64
#include <arch/x86_64/cpuid.hpp>
#include <arch/x86_64/msr.hpp>
// IgrOS-Kernel library
#include <klib/kFlags.hpp>

#endif


#ifdef __cplusplus

extern "C" {

#endif // __cplusplus


	// Enable no-execute page protection if CPU supports it
	[[nodiscard]]
	auto cpuEnableNoExecute() noexcept -> bool {

#if defined(IGROS_ARCH_x86_64)

		using namespace igros;
		using namespace igros::x86_64;

		// CPUID.80000001h:EDX.NX
		if (!cpuidHasLeaf(CPUID::EXTENDED_INFO) || !klib::kFlags<CPUID_EXT_EDX>::from(cpuid(CPUID::EXTENDED_INFO).edx).isSet(CPUID_EXT_EDX::NX)) [[unlikely]] {
			return false;
		}
		// Set EFER.NXE: NX bit in page tables is reserved (faults) until then
		writeMSR(MSR::EFER, klib::kFlags<EFER>::from(readMSR(MSR::EFER)).set(EFER::NXE).value());
		return true;

#else

	// No PAE page tables on i386 - no NX bit
	return false;

#endif
	}


#ifdef __cplusplus

} // extern "C"

#endif // __cplusplus

