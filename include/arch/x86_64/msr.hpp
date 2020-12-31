////////////////////////////////////////////////////////////////
//
//	MSR registers operations
//
//	File:	msr.hpp
//	Date:	13 Jul 2020
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
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


		// Read MSR register
		[[nodiscard]] dword_t volatile outMSR(const dword_t reg) noexcept;

		// Write MSR register
		void volatile inMSR(const dword_t reg, const dword_t value) noexcept;


#ifdef	__cplusplus

	}	// extern "C"

#endif	// __cplusplus


}	// namespace igros::x86_64

