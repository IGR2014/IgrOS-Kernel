////////////////////////////////////////////////////////////////
//
//	Memory paging for x86
//
//	File:	paging.hpp
//	Date:	12 Jun 2019
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
//	All rights reserved.
//
//


#pragma once
#ifndef IGROS_ARCH_PAGING_HPP
#define IGROS_ARCH_PAGING_HPP


#include <flags.hpp>

#include <arch/types.hpp>


// Arch-dependent code zone
namespace arch {


	// Forward declaration
	struct taskRegs_t;


	// Page flags
	enum class pagingFlags_t : dword_t {

		CLEAR			= 0x00000000,
		PRESENT			= 0x00000001,
		WRITABLE		= 0x00000002,
		USER_ACCESSIBLE		= 0x00000004,
		WRITE_THROUGH		= 0x00000008,
		NON_CACHED		= 0x00000010,
		ACCESSED		= 0x00000020,
		DIRTY			= 0x00000040,
		HUGE			= 0x00000080,
		GLOBAL			= 0x00000100,
		USER_DEFINED		= 0x00000E00,
		PHYSICAL_ADDR		= 0xFFFFF000

	};


	// Setup paging
	void		pagingSetup();

	// Convert virtual address to physical address
	pointer_t	pagingVirtToPhys(const pointer_t);

	// Page Fault Exception handler
	void		pagingFaultExceptionHandler(const taskRegs_t*);


}	// namespace arch


#endif	// IGROS_ARCH_PAGING_HPP

