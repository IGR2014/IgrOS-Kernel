////////////////////////////////////////////////////////////////
//
//	Kernel alignment functions definitions
//
//	File:	kalign.hpp
//	Date:	07 Jul 2020
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


	// Align mask
	constexpr inline std::size_t kalignMask(const std::size_t offset) noexcept {
		return ((1ull << offset) - 1ull);
	}

	// Align up
	template<typename T>
	inline T* kalignUp(const T* ptr, const std::size_t offset = alignof(T)) noexcept {
		// Address addition
		auto addition = kalignMask(offset);
		// Do alignment
		return reinterpret_cast<T*>((reinterpret_cast<std::size_t>(ptr) + addition) & ~addition);
	}

	// Align down
	template<typename T>
	inline T* kalignDown(const T* ptr, const std::size_t offset = alignof(T)) noexcept {
		// Address addition
		auto addition = kalignMask(offset);
		// Do alignment
		return reinterpret_cast<T*>((reinterpret_cast<std::size_t>(ptr) - addition) & ~addition);
	}

	// Check alignment
	template<typename T>
	inline bool kalignCheck(const T* ptr, const std::size_t offset = alignof(T)) noexcept {
		return (static_cast<std::size_t>(0) == (reinterpret_cast<std::size_t>(ptr) & kalignMask(offset)));
	}


}	// namespace igros::klib

