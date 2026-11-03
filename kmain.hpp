////////////////////////////////////////////////////////////////
///
///	@file
///	@brief		Kernel main function (called from boot.s)
///	@date		03 Nov 2026
///
///	@copyright	Copyright (c) 2017 - 2026, Igor Baklykov
///			All rights reserved.
///	@author		Igor Baklykov
///


#pragma once


// IgrOS-Kernel arch
#include <arch/types.hpp>


#ifdef __cplusplus

extern "C" {

#endif // __cplusplus


	/// @brief Kernel main function (called from boot.s)
	/// @param multibootAddress Physical address of multiboot information
	/// @param magic Bootloader magic (multiboot check)
	[[noreturn]]
	void kmain(const igros::igros_usize_t multibootAddress, const igros::igros_dword_t magic) noexcept;


#ifdef __cplusplus

} // extern "C"

#endif // __cplusplus

