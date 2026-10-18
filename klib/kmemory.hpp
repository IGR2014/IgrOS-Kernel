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
#include <algorithm>
#include <ranges>
#include <span>
#include <type_traits>
// IgrOS-Kernel arch
#include <arch/types.hpp>


// Kernel library code zone
namespace igros::klib {


	// Fill all elements of dst (array, std::array, std::span...) with value
	// (value is converted to element type, so "int" expressions can't select wider/narrower fill)
	template<std::ranges::contiguous_range R>
	requires std::ranges::output_range<R, std::ranges::range_value_t<R>>
	[[maybe_unused]]
	constexpr void kmemset(R &&dst, const std::ranges::range_value_t<R> &value) noexcept {
		std::ranges::fill(dst, value);
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
		std::ranges::fill(std::as_writable_bytes(std::span {dst, count}), std::byte {0});
		// Return pointer to dst
		return dst;
	}


	// Copy memory
	[[maybe_unused]]
	auto	kmemcpy(const igros_pointer_t dst, const void* const src, const igros_usize_t size) noexcept -> igros_pointer_t;


}	// namespace igros::klib

