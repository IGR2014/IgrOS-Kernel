////////////////////////////////////////////////////////////////
//
//	Long mode low-level preparation operations
//
//	File:	longMode.hpp
//	Date:	16 Dec 2022
//
//	Copyright (c) 2017, Igor Baklykov
//	All rights reserved.
//
//


#pragma	once


// IgrOS-Kernel arch
#include <arch/types.hpp>


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus


	// Check CPUID instruction is available (non-zero if available)
	[[nodiscard]]
	auto	checkCPUID() noexcept -> igros::igros_byte_t;

	// Check CPU supports long mode (non-zero if supported)
	[[nodiscard]]
	auto	checkLongMode() noexcept -> igros::igros_byte_t;


#ifdef	__cplusplus

}

#endif	// __cplusplus

