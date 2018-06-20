////////////////////////////////////////////////////////////////
//
//	Memory paging for x86
//
//	File:	paging.hpp
//	Date:	21 Jun. 2018
//
//	Copyright (c) 2018, Igor Baklykov
//	All rights reserved.
//


#ifndef IGROS_ARCH_PAGING_HPP
#define IGROS_ARCH_PAGING_HPP


#include <type_traits>

#include <include/types.hpp>


// Arch-dependent code zone
namespace arch {


	// Forward declaration
	struct taskRegs_t;


	// Page flags
	enum pagingFlags_t : t_u32 {

		PAGE_CLEAR		= 0x00000000,
		PAGE_PRESENT		= 0x00000001,
		PAGE_WRITABLE		= 0x00000002,
		PAGE_USER_ACCESSIBLE	= 0x00000004,
		PAGE_WRITE_THROUGH	= 0x00000008,
		PAGE_CACHE_DISABLED	= 0x00000010,
		PAGE_ACCESSED		= 0x00000020,
		PAGE_DIRTY		= 0x00000040,
		PAGE_SIZE		= 0x00000080,
		PAGE_GLOBAL		= 0x00000100,
		PAGE_USER_DEFINED	= 0x00000E00,
		PAGE_PHYSICAL_ADDR	= 0xFFFFF000

	};


	extern "C" {


		// Setup page directory
		void	pagingSetupPD(const t_ptr);
		// Flush page directory
		void	pagingFlushPD();

		// Enable paging
		void	pagingEnable();

		// Get address which is caused Page Fault Exception
		t_u32	pagingGetFaultAddres();


	}	// extern "C"


	// Set page directory flags
	void	pagingSetPDFlags(t_ptr, const pagingFlags_t);
	// Set page table flags
	void	pagingSetPTFlags(t_ptr, const pagingFlags_t);

	// Setup paging
	void	pagingSetup();

	// Convert virtual address to physical address
	t_ptr	pagingVirtToPhys(const t_ptr);

	// Page Fault Exception handler
	void pagingFaultExceptionHandler(const taskRegs_t*);


}	// namespace arch


#endif	// IGROS_ARCH_PAGING_HPP

