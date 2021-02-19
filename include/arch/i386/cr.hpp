////////////////////////////////////////////////////////////////
//
//	CR0 - CR4 registers operations
//
//	File:	cr.hpp
//	Date:	12 Feb 2021
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <arch/i386/types.hpp>


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus

	// Read CR0 register
	[[nodiscard]]
	inline igros::dword_t volatile	outCR0() noexcept;
	// Read CR2 register
	[[nodiscard]]
	inline igros::dword_t volatile	outCR2() noexcept;
	// Read CR3 register
	[[nodiscard]]
	inline igros::dword_t volatile	outCR3() noexcept;
	// Read CR4 register
	[[nodiscard]]
	inline igros::dword_t volatile	outCR4() noexcept;

	// Write CR0 register
	inline void volatile	inCR0(const igros::dword_t value) noexcept;
	// Write CR3 register
	inline void volatile	inCR3(const igros::dword_t value) noexcept;
	// Write CR4 register
	inline void volatile	inCR4(const igros::dword_t value) noexcept;

#ifdef	__cplusplus

}	// extern "C"

#endif	// __cplusplus


// i386 platform namespace
namespace igros::i386 {

}	// namespace igros::i386

