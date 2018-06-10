////////////////////////////////////////////////////////////////
//
//	Kernel-space memset for x86
//
//	File:	memset.hpp
//	Date:	05 Jun. 2017
//
//	Copyright (c) 2017, Igor Baklykov
//	All rights reserved.
//


#ifndef IGROS_ARCH_MEMSET_HPP
#define IGROS_ARCH_MEMSET_HPP


#include <include/types.hpp>


namespace arch {


	// Set required memory with specified byte
	extern "C" t_ptr	kmemset8(const t_ptr, const t_u32, const t_u8);
	// Set required memory with specified word
	extern "C" t_ptr	kmemset16(const t_ptr, const t_u32, const t_u16);
	// Set required memory with specified double word
	extern "C" t_ptr	kmemset32(const t_ptr, const t_u32, const t_u32);


}	// namespace arch


#endif	// IGROS_ARCH_MEMSET_HPP


