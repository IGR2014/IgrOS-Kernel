////////////////////////////////////////////////////////////////
///
///	@file
///	@brief		Long mode low-level preparation operations
///	@date		03 Nov 2026
///
///	@copyright	Copyright (c) 2017, Igor Baklykov
///			All rights reserved.
///	@author		Igor Baklykov
///


#pragma once


// IgrOS-Kernel arch
#include <arch/types.hpp>


#ifdef __cplusplus

extern "C" {

#endif // __cplusplus


	/// @brief Check CPUID instruction is available (non-zero if available)
	/// @return Non-zero if CPUID is available
	[[nodiscard]]
	auto checkCPUID() noexcept -> igros::igros_byte_t;

	/// @brief Check CPU supports long mode (non-zero if supported)
	/// @return Non-zero if long mode is supported
	[[nodiscard]]
	auto checkLongMode() noexcept -> igros::igros_byte_t;


#ifdef __cplusplus
}

#endif // __cplusplus

