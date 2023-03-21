////////////////////////////////////////////////////////////////
///
///	@brief		Kernel alignment functions definitions
///
///	@file		kAlign.hpp
///	@date		21 Mar 2023
///
///	@copyright	Copyright (c) 2017 - 2022,
///			All rights reserved.
///	@author		Igor Baklykov
///
///


#pragma once


// C++
#include <bit>
#include <cstdint>
// IgrOS-Kernel arch
#include <arch/types.hpp>


// Kernel library code zone
namespace igros::klib {


	// Kernel pointer alignment handling
	class kAlign final {

		// Align mask
		[[nodiscard]]
		constexpr static auto	mask(const igros_usize_t offset) noexcept -> igros_usize_t;


	public:

		// Check pointer alignment
		template<class T>
		[[nodiscard]]
		constexpr static auto	check(const T* const ptr, const igros_usize_t offset = alignof(T)) noexcept -> bool;

		// Align pointer up
		template<class T>
		[[nodiscard]]
		constexpr static auto	up(const T* const ptr, const igros_usize_t offset = alignof(T)) noexcept -> T*;
		// Align pointer down
		template<class T>
		[[nodiscard]]
		constexpr static auto	down(const T* const ptr, const igros_usize_t offset = alignof(T)) noexcept -> T*;


	};


	// Align mask
	[[nodiscard]]
	constexpr auto kAlign::mask(const igros_usize_t offset) noexcept -> igros_usize_t {
		// Calculate align mask
		return (1_usize << offset) - 1_usize;
	}


	// Check pointer alignment
	template<class T>
	[[nodiscard]]
	constexpr auto kAlign::check(const T* const ptr, const igros_usize_t offset) noexcept -> bool {
		// Address mask
		const auto mask {kAlign::mask(offset)};
		// Do check
		return 0_usize == (std::bit_cast<igros_usize_t>(ptr) & mask);
	}

	// Align pointer up
	template<class T>
	[[nodiscard]]
	constexpr auto kAlign::up(const T* const ptr, const igros_usize_t offset) noexcept -> T* {
		// Address mask
		const auto mask {kAlign::mask(offset)};
		// Do alignment
		return std::bit_cast<T*>((std::bit_cast<igros_usize_t>(ptr) + mask) & ~mask);
	}

	// Align pointer down
	template<class T>
	[[nodiscard]]
	constexpr auto kAlign::down(const T* const ptr, const igros_usize_t offset) noexcept -> T* {
		// Address mask
		const auto mask {kAlign::mask(offset)};
		// Do alignment
		return std::bit_cast<T*>((std::bit_cast<igros_usize_t>(ptr) - mask) & ~mask);
	}


}	// namespace igros::klib

