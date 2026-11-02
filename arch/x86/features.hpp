////////////////////////////////////////////////////////////////
///
///	@file
///	@brief		CPU feature control
///	@date		22 Oct 2026
///
///	@copyright	Copyright (c) 2017 - 2026, Igor Baklykov
///			All rights reserved.
///	@author		Igor Baklykov
///


#pragma once


// IgrOS-Kernel arch
#include <arch/types.hpp>


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus


	/// @brief Enable no-execute page protection (EFER.NXE) if CPU supports it
	/// (false on i386: NX bit exists only in PAE and long mode page tables)
	/// @return true if no-execute protection is enabled
	[[nodiscard]]
	auto	cpuEnableNoExecute() noexcept -> bool;


#ifdef	__cplusplus

}	// extern "C"

#endif	// __cplusplus

