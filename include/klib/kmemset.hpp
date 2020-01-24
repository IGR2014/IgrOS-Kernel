////////////////////////////////////////////////////////////////
//
//	Kernel-space memset for x86
//
//	File:	kmemset.hpp
//	Date:	24 Jan 2020
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
		[[maybe_unused]] inline pointer_t kmemset8(const pointer_t dst, const dword_t size, const byte_t val) noexcept;

		// Set required memory with specified word
		[[maybe_unused]] inline pointer_t kmemset16(const pointer_t dst, const dword_t size, const word_t val) noexcept;

		// Set required memory with specified double word
		[[maybe_unused]] inline pointer_t kmemset32(const pointer_t dst, const dword_t size, const dword_t val) noexcept;

		// TODO: implement!
		// Set required memory with specified quad word
		//static inline pointer_t	kmemset64(const pointer_t, const dword_t, const quad_t) noexcept;


#ifdef	__cplusplus

	}	// extern "C"

#endif	// __cplusplus


}	// namespace arch


#endif	// IGROS_ARCH_KERNEL_MEMSET_HPP


