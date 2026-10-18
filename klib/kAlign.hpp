////////////////////////////////////////////////////////////////
///
///	@brief		Kernel alignment functions definitions
///
///	@file		kAlign.hpp
///	@date		21 Mar 2023
///
///	@copyright	Copyright (c) 2017 - 2026,
///			All rights reserved.
///	@author		Igor Baklykov
///
///


#pragma once


// C++
#include <bit>
#include <concepts>
#include <type_traits>
// IgrOS-Kernel arch
#include <arch/types.hpp>


// Kernel library code zone
namespace igros::klib {


	// Alignable value: unsigned integer (address) or pointer
	template<class T>
	concept kalignable = std::unsigned_integral<T> || std::is_pointer_v<T>;

	// Integer type for alignment arithmetic (pointers as addresses, integers as is: 64-bit on i386 too)
	template<kalignable T>
	using kalign_int_t = std::conditional_t<std::is_pointer_v<T>, igros_usize_t, T>;


	// Kernel alignment handling (ALIGN in bytes, power of two; 0 - natural alignment of pointed type)
	class kAlign final {

		// Alignment in bytes
		template<igros_usize_t ALIGN, kalignable T>
		[[nodiscard]]
		consteval static auto	alignment() noexcept -> igros_usize_t;

		// Alignment mask
		template<igros_usize_t ALIGN, kalignable T>
		[[nodiscard]]
		consteval static auto	mask() noexcept -> kalign_int_t<T>;

		// Value as integer
		template<kalignable T>
		[[nodiscard]]
		constexpr static auto	integer(const T value) noexcept -> kalign_int_t<T>;

		// Integer as value
		template<kalignable T>
		[[nodiscard]]
		constexpr static auto	from(const kalign_int_t<T> value) noexcept -> T;


	public:

		// Check alignment
		template<igros_usize_t ALIGN = 0_usize, kalignable T>
		requires (std::has_single_bit(ALIGN) || (0_usize == ALIGN))
		[[nodiscard]]
		constexpr static auto	check(const T value) noexcept -> bool;

		// Align up
		template<igros_usize_t ALIGN = 0_usize, kalignable T>
		requires (std::has_single_bit(ALIGN) || (0_usize == ALIGN))
		[[nodiscard]]
		constexpr static auto	up(const T value) noexcept -> T;
		// Align down
		template<igros_usize_t ALIGN = 0_usize, kalignable T>
		requires (std::has_single_bit(ALIGN) || (0_usize == ALIGN))
		[[nodiscard]]
		constexpr static auto	down(const T value) noexcept -> T;


	};


	// Alignment in bytes
	template<igros_usize_t ALIGN, kalignable T>
	[[nodiscard]]
	consteval auto kAlign::alignment() noexcept -> igros_usize_t {
		if constexpr (0_usize != ALIGN) {
			return ALIGN;
		} else if constexpr (std::is_pointer_v<T> && !std::is_void_v<std::remove_pointer_t<T>>) {
			return alignof(std::remove_pointer_t<T>);
		} else {
			return 1_usize;
		}
	}

	// Alignment mask
	template<igros_usize_t ALIGN, kalignable T>
	[[nodiscard]]
	consteval auto kAlign::mask() noexcept -> kalign_int_t<T> {
		return static_cast<kalign_int_t<T>>(kAlign::alignment<ALIGN, T>() - 1_usize);
	}

	// Value as integer
	template<kalignable T>
	[[nodiscard]]
	constexpr auto kAlign::integer(const T value) noexcept -> kalign_int_t<T> {
		if constexpr (std::is_pointer_v<T>) {
			return std::bit_cast<igros_usize_t>(value);
		} else {
			return value;
		}
	}

	// Integer as value
	template<kalignable T>
	[[nodiscard]]
	constexpr auto kAlign::from(const kalign_int_t<T> value) noexcept -> T {
		if constexpr (std::is_pointer_v<T>) {
			return std::bit_cast<T>(value);
		} else {
			return value;
		}
	}


	// Check alignment
	template<igros_usize_t ALIGN, kalignable T>
	requires (std::has_single_bit(ALIGN) || (0_usize == ALIGN))
	[[nodiscard]]
	constexpr auto kAlign::check(const T value) noexcept -> bool {
		return 0 == (kAlign::integer(value) & kAlign::mask<ALIGN, T>());
	}

	// Align up
	template<igros_usize_t ALIGN, kalignable T>
	requires (std::has_single_bit(ALIGN) || (0_usize == ALIGN))
	[[nodiscard]]
	constexpr auto kAlign::up(const T value) noexcept -> T {
		constexpr auto MASK {kAlign::mask<ALIGN, T>()};
		return kAlign::from<T>(static_cast<kalign_int_t<T>>((kAlign::integer(value) + MASK) & ~MASK));
	}

	// Align down
	template<igros_usize_t ALIGN, kalignable T>
	requires (std::has_single_bit(ALIGN) || (0_usize == ALIGN))
	[[nodiscard]]
	constexpr auto kAlign::down(const T value) noexcept -> T {
		constexpr auto MASK {kAlign::mask<ALIGN, T>()};
		return kAlign::from<T>(static_cast<kalign_int_t<T>>(kAlign::integer(value) & ~MASK));
	}


}	// namespace igros::klib

