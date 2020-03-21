////////////////////////////////////////////////////////////////
//
//	MSR registers operations
//
//	File:	msr.hpp
//	Date:	12 Feb 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#pragma once
#ifndef IGROS_ARCH_MACHINE_SPECIFIC_REGISTERS_HPP
#define IGROS_ARCH_MACHINE_SPECIFIC_REGISTERS_HPP


#include <types.hpp>


// Arch-dependent code zone
namespace arch {


#ifdef	__cplusplus

	extern "C" {

#endif	// __cplusplus


		// Read MSR register
		[[nodiscard]] dword_t volatile outMSR(const dword_t reg) noexcept;

		// Write MSR register
		void volatile inMSR(const dword_t reg, const dword_t value) noexcept;


#ifdef	__cplusplus

	}	// extern "C"

#endif	// __cplusplus


}	// namespace arch


#endif	// IGROS_ARCH_MACHINE_SPECIFIC_REGISTERS_HPP

