////////////////////////////////////////////////////////////////
//
//	Kernel-space memset for x86
//
//	File:	memset.hpp
//	Date:	23 Jun. 2018
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
//	All rights reserved.
//
//


#ifndef IGROS_ARCH_MEMSET_HPP
#define IGROS_ARCH_MEMSET_HPP


#include <include/types.hpp>


// Arch-dependent code zone
namespace arch {


#ifdef	__cplusplus

	extern "C" {

#endif	// __cplusplus


		// Set required memory with specified byte
		pointer_t	kmemset8(const pointer_t, const dword_t, const byte_t);
		// Set required memory with specified word
		pointer_t	kmemset16(const pointer_t, const dword_t, const word_t);
		// Set required memory with specified double word
		pointer_t	kmemset32(const pointer_t, const dword_t, const dword_t);


#ifdef	__cplusplus

	}	// extern "C"

#endif	// __cplusplus


}	// namespace arch


#endif	// IGROS_ARCH_MEMSET_HPP


