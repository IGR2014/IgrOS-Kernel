////////////////////////////////////////////////////////////////
//
//	Kernel-space memset for x86
//
//	File:	kmemset.hpp
//	Date:	30 Jun 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <cstdint>

#include <types.hpp>


// Kernel library code zone
namespace igros::klib {


	// Set required memory with specified byte
	[[maybe_unused]] pointer_t kmemset8(const pointer_t dst, const std::size_t size, const byte_t val) noexcept;

	// Set required memory with specified word
	[[maybe_unused]] pointer_t kmemset16(const pointer_t dst, const std::size_t size, const word_t val) noexcept;

	// Set required memory with specified double word
	[[maybe_unused]] pointer_t kmemset32(const pointer_t dst, const std::size_t size, const dword_t val) noexcept;

	// TODO: implement!
	// Set required memory with specified quad word
	//static inline pointer_t	kmemset64(const pointer_t, std::size_t dword_t, const quad_t) noexcept;


	// Copy memory
	[[maybe_unused]] pointer_t kmemcpy(const pointer_t dst, const pointer_t src, const std::size_t size) noexcept;


}	// namespace igros::arch

