////////////////////////////////////////////////////////////////
//
//	Kernel alignment functions definitions
//
//	File:	kalign.hpp
//	Date:	08 Dec 2022
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <bit>
#include <cstdint>

#include <arch/types.hpp>


// Kernel library code zone
namespace igros::klib {


	// Align mask
	[[nodiscard]]
	constexpr igros_usize_t kalignMask(const igros_usize_t offset) noexcept {
		return ((1_usize << offset) - 1_usize);
	}

	// Align up
	template<typename T>
	[[nodiscard]]
	constexpr T* kalignUp(const T* ptr, const igros_usize_t offset = alignof(T)) noexcept {
		// Address addition
		const auto addition {kalignMask(offset)};
		// Do alignment
		return std::bit_cast<T*>((std::bit_cast<igros_usize_t>(ptr) + addition) & ~addition);
	}

	// Align down
	template<typename T>
	[[nodiscard]]
	constexpr T* kalignDown(const T* ptr, const igros_usize_t offset = alignof(T)) noexcept {
		// Address addition
		const auto addition {kalignMask(offset)};
		// Do alignment
		return std::bit_cast<T*>((std::bit_cast<igros_usize_t>(ptr) - addition) & ~addition);
	}

	// Check alignment
	template<typename T>
	[[nodiscard]]
	constexpr bool kalignCheck(const T* ptr, const igros_usize_t offset = alignof(T)) noexcept {
		return (0_usize == (std::bit_cast<igros_usize_t>(ptr) & kalignMask(offset)));
	}


}	// namespace igros::klib

