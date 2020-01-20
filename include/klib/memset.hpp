////////////////////////////////////////////////////////////////
//
//	Kernel-space memset for x86
//
//	File:	memset.hpp
//	Date:	17 Jan 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#pragma once
#ifndef IGROS_ARCH_KERNEL_MEMSET_HPP
#define IGROS_ARCH_KERNEL_MEMSET_HPP


#include <arch/types.hpp>


// Kernel library code zone
namespace klib {


#ifdef	__cplusplus

	extern "C" {

#endif	// __cplusplus


		// Set required memory with specified byte
		[[maybe_unused]] static inline pointer_t kmemset8(const pointer_t, const dword_t, const byte_t) noexcept;

		// Set required memory with specified word
		[[maybe_unused]] static inline pointer_t kmemset16(const pointer_t, const dword_t, const word_t) noexcept;

		// Set required memory with specified double word
		[[maybe_unused]] static inline pointer_t kmemset32(const pointer_t, const dword_t, const dword_t) noexcept;

		// TODO: implement!
		// Set required memory with specified quad word
		//static inline pointer_t	kmemset64(const pointer_t, const dword_t, const quad_t) noexcept;


#ifdef	__cplusplus

	}	// extern "C"

#endif	// __cplusplus


}	// namespace arch


#endif	// IGROS_ARCH_KERNEL_MEMSET_HPP


