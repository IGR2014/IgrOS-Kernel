////////////////////////////////////////////////////////////////
//
//	Kernel-space memset for x86
//
//	File:	memset.hpp
//	Date:	06 Jun 2019
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
//	All rights reserved.
//
//


#pragma once
#ifndef IGROS_ARCH_KERNEL_MEMSET_HPP
#define IGROS_ARCH_KERNEL_MEMSET_HPP


#include <arch/types.hpp>


// Arch-dependent code zone
namespace arch {


#ifdef	__cplusplus

	extern "C" {

#endif	// __cplusplus


		// Set required memory with specified byte
		static inline pointer_t	kmemset8(const pointer_t, const dword_t, const byte_t);

		// Set required memory with specified word
		static inline pointer_t	kmemset16(const pointer_t, const dword_t, const word_t);

		// Set required memory with specified double word
		static inline pointer_t	kmemset32(const pointer_t, const dword_t, const dword_t);

		// Set required memory with specified quad word
		static inline pointer_t	kmemset64(const pointer_t, const dword_t, const quad_t);


#ifdef	__cplusplus

	}	// extern "C"

#endif	// __cplusplus


}	// namespace arch


#endif	// IGROS_ARCH_KERNEL_MEMSET_HPP


