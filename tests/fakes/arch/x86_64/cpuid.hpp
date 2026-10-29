////////////////////////////////////////////////////////////////
//
//	Host fake for CPUID (see hw.hpp): shadows <arch/x86_64/cpuid.hpp>
//	through include order, types come from the real header
//
//	File:	cpuid.hpp
//	Date:	29 Oct 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// Real header: types and enums (instruction wrappers renamed, unused)
#define cpuid		igros_test_real_cpuid
#define cpuidHasLeaf	igros_test_real_cpuidHasLeaf
#include "../../../../arch/x86_64/cpuid.hpp"
#undef cpuid
#undef cpuidHasLeaf

// Tests
#include <tests/hw.hpp>


namespace igros::x86_64 {


	// CPUID from fake hardware
	[[nodiscard]]
	inline auto cpuid(const CPUID leaf, [[maybe_unused]] const igros_dword_t subleaf = 0_u32) noexcept -> cpuidRegs_t {
		const auto found {test::hw::cpuid.find(static_cast<igros_dword_t>(leaf))};
		if (test::hw::cpuid.end() == found) {
			return cpuidRegs_t {};
		}
		return cpuidRegs_t {found->second.eax, found->second.ebx, found->second.ecx, found->second.edx};
	}

	// Same logic as real header
	[[nodiscard]]
	inline auto cpuidHasLeaf(const CPUID leaf) noexcept -> bool {
		const auto value	{static_cast<igros_dword_t>(leaf)};
		const auto base		{value & 0x80000000_u32};
		return cpuid(static_cast<CPUID>(base)).eax >= value;
	}


}	// namespace igros::x86_64
