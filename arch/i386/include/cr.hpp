////////////////////////////////////////////////////////////////
//
//	CR0 - CR4 registers operations
//
//	File:	exceptions.hpp
//	Date:	01 Feb. 2019
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
//	All rights reserved.
//
//


#ifndef IGROS_ARCH_CONTROL_REGISTERS_HPP
#define IGROS_ARCH_CONTROL_REGISTERS_HPP


#include <include/types.hpp>


// Arch-dependent code zone
namespace arch {


#ifdef	__cplusplus

	extern "C" {

#endif	// __cplusplus


		// Write CR0 register
		void	inCR0(const dword_t registerValue);
		// Write CR3 register
		void	inCR3(const dword_t registerValue);
		// Write CR4 register
		void	inCR4(const dword_t registerValue);

		// Read CR0 register
		dword_t	outCR0();
		// Read CR2 register
		dword_t	outCR2();
		// Read CR3 register
		dword_t	outCR3();
		// Read CR4 register
		dword_t	outCR4();


#ifdef	__cplusplus

	}	// extern "C"

#endif	// __cplusplus


}	// namespace arch


#endif	// IGROS_ARCH_CONTROL_REGISTERS_HPP

