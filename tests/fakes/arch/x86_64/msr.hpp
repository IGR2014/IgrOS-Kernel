////////////////////////////////////////////////////////////////
//
//	Host fake for RDMSR/WRMSR (see hw.hpp): shadows <arch/x86_64/msr.hpp>
//	through include order, types come from the real header
//
//	File:	msr.hpp
//	Date:	29 Oct 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// Real header: types and enums (instruction wrappers renamed, unused)
#define readMSR		igros_test_real_readMSR
#define writeMSR	igros_test_real_writeMSR
#include "../../../../arch/x86_64/msr.hpp"
#undef readMSR
#undef writeMSR

// Tests
#include <tests/hw.hpp>


namespace igros::x86_64 {


	// Read MSR from fake hardware
	[[nodiscard]]
	inline auto readMSR(const MSR reg) noexcept -> igros_quad_t {
		return test::hw::msr[static_cast<igros_dword_t>(reg)];
	}

	// Write MSR to fake hardware
	inline void writeMSR(const MSR reg, const igros_quad_t value) noexcept {
		test::hw::msr[static_cast<igros_dword_t>(reg)] = value;
	}


}	// namespace igros::x86_64
