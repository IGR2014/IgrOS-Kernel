////////////////////////////////////////////////////////////////
//
//	Kernel memory functions
//
//	File:	kmemory.hpp
//	Date:	21 Mar 2023
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// C++
#include <type_traits>
// IgrOS-Kernel arch
#include <arch/types.hpp>


// Kernel library code zone
namespace igros::klib {


	// Fill count elements of dst with value
	// (value is converted to element type, so "int" expressions can't select wider/narrower fill)
	template<class T>
	[[maybe_unused]]
	constexpr auto kmemset(T* const dst, const igros_usize_t count, const std::type_identity_t<T> value) noexcept -> T* {
		// Fill elements one by one
		for (auto i {0_usize}; i < count; ++i) {
			dst[i] = value;
		}
		// Return pointer to dst
		return dst;
	}


	// Zero count objects (all bytes)
	template<class T>
	requires std::is_trivially_copyable_v<T>
	[[maybe_unused]]
	inline auto kmemzero(T* const dst, const igros_usize_t count = 1_usize) noexcept -> T* {
		// Check arguments
		if (nullptr == dst) [[unlikely]] {
			return nullptr;
		}
		// Zero object bytes
		kmemset(static_cast<igros_byte_t*>(static_cast<igros_pointer_t>(dst)), sizeof(T) * count, 0_u8);
		// Return pointer to dst
		return dst;
	}


	// Copy memory
	[[maybe_unused]]
	auto	kmemcpy(const igros_pointer_t dst, const void* const src, const igros_usize_t size) noexcept -> igros_pointer_t;


}	// namespace igros::klib

