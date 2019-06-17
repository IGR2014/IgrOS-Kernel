////////////////////////////////////////////////////////////////
//
//	MSR registers operations
//
//	File:	msr.hpp
//	Date:	18 Jun 2019
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
//	All rights reserved.
//
//


#pragma once
#ifndef IGROS_ARCH_MACHINE_SPECIFIC_REGISTERS_HPP
#define IGROS_ARCH_MACHINE_SPECIFIC_REGISTERS_HPP


#include <arch/types.hpp>


// Arch-dependent code zone
namespace arch {


#ifdef	__cplusplus

	extern "C" {

#endif	// __cplusplus


		// Write MSR register
		static inline void volatile	inMSR(const dword_t reg, const dword_t val);

		// Read MSR register
		static inline dword_t volatile	outMSR(const dword_t reg);


#ifdef	__cplusplus

	}	// extern "C"

#endif	// __cplusplus


}	// namespace arch


#endif	// IGROS_ARCH_MACHINE_SPECIFIC_REGISTERS_HPP

