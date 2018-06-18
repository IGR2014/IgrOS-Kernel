////////////////////////////////////////////////////////////////
//
//	Memory paging for x86
//
//	File:	memset.hpp
//	Date:	18 Jun. 2018
//
//	Copyright (c) 2018, Igor Baklykov
//	All rights reserved.
//


#ifndef IGROS_ARCH_PAGING_HPP
#define IGROS_ARCH_PAGING_HPP


#include <include/types.hpp>


// Arch-dependent code zone
namespace arch {


	extern "C" {

		// Setup page directory
		void	pagingSetupPD(const t_ptr);
		// Flush page directory
		void	pagingFlushPD();

		// Enable paging
		void	pagingEnable();

	}


	// Setup paging
	void pagingSetup();


}	// namespace arch


#endif	// IGROS_ARCH_PAGING_HPP

