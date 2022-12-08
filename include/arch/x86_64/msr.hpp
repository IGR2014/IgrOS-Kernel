////////////////////////////////////////////////////////////////
//
//	MSR registers operations
//
//	File:	msr.hpp
//	Date:	09 Dec 2022
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <arch/types.hpp>


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus


	// Read MSR register
	[[nodiscard]]
	auto	outMSR(const igros::igros_dword_t reg) noexcept -> igros::igros_dword_t;

	// Write MSR register
	void	inMSR(const igros::igros_dword_t reg, const igros::igros_dword_t value) noexcept;


#ifdef	__cplusplus

}	// extern "C"

#endif	// __cplusplus

