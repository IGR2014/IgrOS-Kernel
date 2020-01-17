////////////////////////////////////////////////////////////////
//
//	Memory paging for x86
//
//	File:	paging.hpp
//	Date:	06 Jun 2019
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
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
	enum class pagingFlags_t : quad_t {

		CLEAR			= 0x0000000000000000,
		PRESENT			= 0x0000000000000001,
		WRITABLE		= 0x0000000000000002,
		USER_ACCESSIBLE		= 0x0000000000000004,
		WRITE_THROUGH		= 0x0000000000000008,
		NON_CACHED		= 0x0000000000000010,
		ACCESSED		= 0x0000000000000020,
		DIRTY			= 0x0000000000000040,
		HUGE			= 0x0000000000000080,
		GLOBAL			= 0x0000000000000100,
		USER_DEFINED		= 0x0000000000000E00,
		PHYSICAL_ADDR_MASK	= 0xFFFFFFFFFFFFF000

	};


	// Setup page directory
	void		pagingSetupPD(const pointer_t);
	// Flush page directory
	void		pagingFlushPD();

	// Enable paging
	void		pagingEnable();

	// Get address which is caused Page Fault Exception
	quad_t		pagingGetFaultAddres();

	// Setup paging
	void		pagingSetup();

	// Convert virtual address to physical address
	pointer_t	pagingVirtToPhys(const pointer_t);

	// Page Fault Exception handler
	void		pagingFaultExceptionHandler(const taskRegs_t*);


}	// namespace arch


#endif	// IGROS_ARCH_PAGING_HPP

