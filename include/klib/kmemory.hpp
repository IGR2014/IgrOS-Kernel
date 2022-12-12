////////////////////////////////////////////////////////////////
//
//	Kernel-space memset for x86
//
//	File:	kmemset.hpp
//	Date:	12 Dec 2022
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <cstdint>
#include <array>
#include <algorithm>
#include <type_traits>

#include <arch/types.hpp>

#include <klib/kalign.hpp>


// Kernel library code zone
namespace igros::klib {


	// Set required memory with specified byte
	[[maybe_unused]]
	auto	kmemset8(igros_byte_t* const dst, const igros_usize_t size, const igros_byte_t val) noexcept -> igros_pointer_t;

	// Set required memory with specified word
	[[maybe_unused]]
	auto	kmemset16(igros_word_t* const dst, const igros_usize_t size, const igros_word_t val) noexcept -> igros_pointer_t;

	// Set required memory with specified double word
	[[maybe_unused]]
	auto	kmemset32(igros_dword_t* const dst, const igros_usize_t size, const igros_dword_t val) noexcept -> igros_pointer_t;

	// Set required memory with specified quad word
	[[maybe_unused]]
	auto	kmemset64(igros_quad_t* const dst, const igros_usize_t size, const igros_quad_t val) noexcept -> igros_pointer_t;


	// Set required memory with specified byte
	[[maybe_unused]]
	inline auto kmemset(const igros_pointer_t dst, const igros_usize_t size, const char val) noexcept -> igros_pointer_t {
		return kmemset8(std::bit_cast<igros_byte_t* const>(dst), size, val);
	}

	// Set required memory with specified byte
	[[maybe_unused]]
	inline auto kmemset(const igros_pointer_t dst, const igros_usize_t size, const igros_byte_t val) noexcept -> igros_pointer_t {
		return kmemset8(static_cast<igros_byte_t* const>(dst), size, val);
	}

	// Set required memory with specified word
	[[maybe_unused]]
	inline auto kmemset(const igros_pointer_t dst, const igros_usize_t size, const igros_word_t val) noexcept -> igros_pointer_t {
		return kmemset16(static_cast<igros_word_t* const>(dst), size, val);
	}

	// Set required memory with specified double word
	[[maybe_unused]]
	inline auto kmemset(const igros_pointer_t dst, const igros_usize_t size, const igros_dword_t val) noexcept -> igros_pointer_t {
		return kmemset32(static_cast<igros_dword_t* const>(dst), size, val);
	}

	// Set required memory with specified quad word
	[[maybe_unused]]
	inline auto kmemset(const igros_pointer_t dst, const igros_usize_t size, const igros_quad_t val) noexcept -> igros_pointer_t {
		return kmemset64(static_cast<igros_quad_t* const>(dst), size, val);
	}


	// Set required memory with specified signed byte
	[[maybe_unused]]
	inline auto kmemset(const igros_pointer_t dst, const igros_usize_t size, const igros_sbyte_t val) noexcept -> igros_pointer_t {
		return kmemset8(static_cast<igros_byte_t* const>(dst), size, static_cast<igros_byte_t>(val));
	}

	// Set required memory with specified signed word
	[[maybe_unused]]
	inline auto kmemset(const igros_pointer_t dst, const igros_usize_t size, const igros_sword_t val) noexcept -> igros_pointer_t {
		return kmemset16(static_cast<igros_word_t* const>(dst), size, static_cast<igros_word_t>(val));
	}

	// Set required memory with specified signed double word
	[[maybe_unused]]
	inline auto kmemset(const igros_pointer_t dst, const igros_usize_t size, const igros_sdword_t val) noexcept -> igros_pointer_t {
		return kmemset32(static_cast<igros_dword_t* const>(dst), size, static_cast<igros_dword_t>(val));
	}

	// Set required memory with specified signed quad word
	[[maybe_unused]]
	inline auto kmemset(const igros_pointer_t dst, const igros_usize_t size, const igros_squad_t val) noexcept -> igros_pointer_t {
		return kmemset64(static_cast<igros_quad_t* const>(dst), size, static_cast<igros_quad_t>(val));
	}


	// Set required memory with specified data
	template<igros_usize_t S, typename T>
	[[maybe_unused]]
	inline auto kmemset(T (&arr)[S], const T val) noexcept -> igros_pointer_t {
		return kmemset(arr, S, val);
	}


	// Copy memory
	[[maybe_unused]]
	auto	kmemcpy(igros_pointer_t dst, const igros_pointer_t src, const igros_usize_t size) noexcept -> igros_pointer_t;


}	// namespace igros::klib


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus


	// Memset to make GCC/Clang happy
	[[maybe_unused]]
	auto memset(igros::igros_pointer_t dst, const igros::igros_byte_t val, const igros::igros_usize_t size) noexcept -> igros::igros_pointer_t;


#ifdef	__cplusplus

}

#endif	// __cplusplus

