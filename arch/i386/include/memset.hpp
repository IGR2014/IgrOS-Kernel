////////////////////////////////////////////////////////////////
//
//	Kernel-space memset for x86
//
//	File:	memset.hpp
//	Date:	18 Jun. 2018
//
//	Copyright (c) 2018, Igor Baklykov
//	All rights reserved.
//


#ifndef IGROS_ARCH_MEMSET_HPP
#define IGROS_ARCH_MEMSET_HPP


#include <include/types.hpp>


// Arch-dependent code zone
namespace arch {


	extern "C" {


		// Set required memory with specified byte
		t_ptr	kmemset8(const t_ptr, const t_u32, const t_u8);
		// Set required memory with specified word
		t_ptr	kmemset16(const t_ptr, const t_u32, const t_u16);
		// Set required memory with specified double word
		t_ptr	kmemset32(const t_ptr, const t_u32, const t_u32);


	}	// extern "C"


}	// namespace arch


#endif	// IGROS_ARCH_MEMSET_HPP


