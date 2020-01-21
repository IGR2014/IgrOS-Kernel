////////////////////////////////////////////////////////////////
//
//	CR0 - CR4 registers operations
//
//	File:	cr.hpp
//	Date:	20 Jan 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
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


		// Read CR0 register
		[[nodiscard]] dword_t volatile outCR0() noexcept;
		// Read CR2 register
		[[nodiscard]] dword_t volatile outCR2() noexcept;
		// Read CR3 register
		[[nodiscard]] dword_t volatile outCR3() noexcept;
		// Read CR4 register
		[[nodiscard]] dword_t volatile outCR4() noexcept;

		// Write CR0 register
		void volatile inCR0(const dword_t value) noexcept;
		// Write CR3 register
		void volatile inCR3(const dword_t value) noexcept;
		// Write CR4 register
		void volatile inCR4(const dword_t value) noexcept;


#ifdef	__cplusplus

	}	// extern "C"

#endif	// __cplusplus


}	// namespace arch


#endif	// IGROS_ARCH_CONTROL_REGISTERS_HPP

