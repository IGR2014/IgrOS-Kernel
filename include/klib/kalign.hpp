////////////////////////////////////////////////////////////////
//
//	Kernel alignment functions definitions
//
//	File:	kalign.hpp
//	Date:	12 Feb 2021
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <cstdint>

#include <arch/types.hpp>


// Kernel library code zone
namespace igros::klib {


	// Align mask
	[[nodiscard]]
	constexpr std::size_t kalignMask(const std::size_t offset) noexcept {
		return ((1ULL << offset) - 1ULL);
	}

	// Align up
	template<typename T>
	[[nodiscard]]
	constexpr T* kalignUp(const T* ptr, const std::size_t offset = alignof(T)) noexcept {
		// Address addition
		const auto addition = kalignMask(offset);
		// Do alignment
		return reinterpret_cast<T*>((reinterpret_cast<std::size_t>(ptr) + addition) & ~addition);
	}

	// Align down
	template<typename T>
	[[nodiscard]]
	constexpr T* kalignDown(const T* ptr, const std::size_t offset = alignof(T)) noexcept {
		// Address addition
		const auto addition = kalignMask(offset);
		// Do alignment
		return reinterpret_cast<T*>((reinterpret_cast<std::size_t>(ptr) - addition) & ~addition);
	}

	// Check alignment
	template<typename T>
	[[nodiscard]]
	constexpr bool kalignCheck(const T* ptr, const std::size_t offset = alignof(T)) noexcept {
		return (0ULL == (reinterpret_cast<std::size_t>(ptr) & kalignMask(offset)));
	}


}	// namespace igros::klib

