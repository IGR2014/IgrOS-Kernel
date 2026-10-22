////////////////////////////////////////////////////////////////
//
//	CPU feature control
//
//	File:	features.hpp
//	Date:	22 Oct 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// IgrOS-Kernel arch
#include <arch/types.hpp>


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus


	// Enable no-execute page protection (EFER.NXE) if CPU supports it
	// (false on i386: NX bit exists only in PAE and long mode page tables)
	[[nodiscard]]
	auto	cpuEnableNoExecute() noexcept -> bool;


#ifdef	__cplusplus

}	// extern "C"

#endif	// __cplusplus

