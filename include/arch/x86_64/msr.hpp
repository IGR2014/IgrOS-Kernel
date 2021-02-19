////////////////////////////////////////////////////////////////
//
//	MSR registers operations
//
//	File:	msr.hpp
//	Date:	12 Feb 2021
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <arch/x86_64/types.hpp>


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus

	// Read MSR register
	[[nodiscard]]
	inline igros::dword_t volatile	outMSR(const igros::dword_t reg) noexcept;

	// Write MSR register
	inline void volatile	inMSR(const igros::dword_t reg, const igros::dword_t value) noexcept;

#ifdef	__cplusplus

}	// extern "C"

#endif	// __cplusplus


// x86_64 namespace
namespace igros::x86_64 {

}	// namespace igros::x86_64

