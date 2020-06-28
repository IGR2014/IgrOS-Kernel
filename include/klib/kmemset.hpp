////////////////////////////////////////////////////////////////
//
//	Kernel-space memset for x86
//
//	File:	kmemset.hpp
//	Date:	28 Jun 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <cstdint>

#include <types.hpp>


// Kernel library code zone
namespace klib {


#ifdef	__cplusplus

	extern "C" {

#endif	// __cplusplus


		// Set required memory with specified byte
		[[maybe_unused]] inline pointer_t kmemset8(const pointer_t dst, const std::size_t size, const byte_t val) noexcept;

		// Set required memory with specified word
		[[maybe_unused]] inline pointer_t kmemset16(const pointer_t dst, const std::size_t size, const word_t val) noexcept;

		// Set required memory with specified double word
		[[maybe_unused]] inline pointer_t kmemset32(const pointer_t dst, const std::size_t size, const dword_t val) noexcept;

		// TODO: implement!
		// Set required memory with specified quad word
		//static inline pointer_t	kmemset64(const pointer_t, std::size_t dword_t, const quad_t) noexcept;


#ifdef	__cplusplus

	}	// extern "C"

#endif	// __cplusplus


}	// namespace arch

