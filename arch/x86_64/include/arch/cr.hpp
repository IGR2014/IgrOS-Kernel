////////////////////////////////////////////////////////////////
//
//	CR0 - CR4 registers operations
//
//	File:	cr.hpp
//	Date:	06 Jun 2019
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
//	All rights reserved.
//
//


#pragma once
#ifndef IGROS_ARCH_CONTROL_REGISTERS_HPP
#define IGROS_ARCH_CONTROL_REGISTERS_HPP


#include <arch/types.hpp>


// Arch-dependent code zone
namespace arch {


#ifdef	__cplusplus

	extern "C" {

#endif	// __cplusplus


		// Write CR0 register
		static inline void volatile	inCR0(const quad_t registerValue);
		// Write CR3 register
		static inline void volatile	inCR3(const quad_t registerValue);
		// Write CR4 register
		static inline void volatile	inCR4(const quad_t registerValue);

		// Read CR0 register
		static inline quad_t volatile	outCR0();
		// Read CR2 register
		static inline quad_t volatile	outCR2();
		// Read CR3 register
		static inline quad_t volatile	outCR3();
		// Read CR4 register
		static inline quad_t volatile	outCR4();


#ifdef	__cplusplus

	}	// extern "C"

#endif	// __cplusplus


}	// namespace arch


#endif	// IGROS_ARCH_CONTROL_REGISTERS_HPP

