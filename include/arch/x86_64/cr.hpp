////////////////////////////////////////////////////////////////
//
//	CR0 - CR4 registers operations
//
//	File:	cr.hpp
//	Date:	13 Jul 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <arch/x86_64/types.hpp>


// x86_64 namespace
namespace igros::x86_64 {


#ifdef	__cplusplus

	extern "C" {

#endif	// __cplusplus


		// Read CR0 register
		[[nodiscard]] inline quad_t volatile outCR0() noexcept;
		// Read CR2 register
		[[nodiscard]] inline quad_t volatile outCR2() noexcept;
		// Read CR3 register
		[[nodiscard]] inline quad_t volatile outCR3() noexcept;
		// Read CR4 register
		[[nodiscard]] inline quad_t volatile outCR4() noexcept;

		// Write CR0 register
		inline void volatile inCR0(const quad_t value) noexcept;
		// Write CR3 register
		inline void volatile inCR3(const quad_t value) noexcept;
		// Write CR4 register
		inline void volatile inCR4(const quad_t value) noexcept;


#ifdef	__cplusplus

	}	// extern "C"

#endif	// __cplusplus


}	// namespace igros::x86_64

