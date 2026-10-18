////////////////////////////////////////////////////////////////
//
//	Kernel text print functions
//
//	File:	kPrint.hpp
//	Date:	17 Mar 2023
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// C++
#include <algorithm>
#include <array>
#include <bit>
#include <concepts>
#include <cstdarg>
#include <cstdint>
#include <limits>
#include <span>
#include <string_view>
#include <type_traits>
#include <utility>
// IgrOS-Kernel arch
#include <arch/types.hpp>
// IgrOS-Kernel library
#include <klib/kMath.hpp>


// Kernel library code zone
namespace igros::klib {


	// Integer representation types
	enum class [[nodiscard]] radix_t : igros_byte_t {
		BIN	= 0x02_u8,			// (Base 2)	Binary integer format
		OCT	= 0x08_u8,			// (Base 8)	Oct integer format
		DEC	= 0x0A_u8,			// (Base 10)	Decimal integer format
		HEX	= 0x10_u8			// (Base 16)	Hexidemical integer format
	};


	// Digits of unsigned value written to the end of buffer (no sign, no terminator)
	// Buffer must fit all digits (std::numeric_limits<U>::digits for binary)
	template<std::unsigned_integral U>
	[[nodiscard]]
	constexpr auto kdigits(const std::span<char> buffer, U value, const radix_t radix, const bool upper = false) noexcept -> std::string_view {

		// Digit symbols
		constexpr auto LOWER	{std::string_view {"0123456789abcdef"}};
		constexpr auto UPPER	{std::string_view {"0123456789ABCDEF"}};
		const auto digits	{upper ? UPPER : LOWER};

		// Digits are written backwards from buffer end
		auto pos {buffer.end()};

		// Power of two radix: shifts and masks (no division)
		if (radix_t::DEC != radix) {
			const auto shift	{std::countr_zero(static_cast<igros_dword_t>(radix))};
			const auto mask		{static_cast<U>(static_cast<U>(radix) - 1U)};
			do {
				*--pos	= digits[static_cast<igros_usize_t>(value & mask)];
				value	= static_cast<U>(value >> shift);
			} while (0U != value);
			return std::string_view {pos, buffer.end()};
		}

		// Values wider than CPU word (64-bit on i386) use slow software division:
		// split into 9-digit chunks so it runs per chunk, not per digit
		if constexpr (sizeof(U) > sizeof(igros_usize_t)) {
			constexpr auto CHUNK		{static_cast<U>(1000000000U)};
			constexpr auto CHUNK_DIGITS	{9};
			while (value > std::numeric_limits<igros_usize_t>::max()) {
				const auto res	{kdivmod(value, CHUNK)};
				auto chunk	{static_cast<igros_usize_t>(res.remainder)};
				for (auto i {0}; i < CHUNK_DIGITS; ++i) {
					*--pos	= digits[chunk % 10U];
					chunk	/= 10U;
				}
				value = res.quotient;
			}
		}

		// Rest fits CPU word: native division
		using N = std::conditional_t<(sizeof(U) > sizeof(igros_usize_t)), igros_usize_t, U>;
		auto rest {static_cast<N>(value)};
		do {
			*--pos	= digits[static_cast<igros_usize_t>(rest % 10U)];
			rest	= static_cast<N>(rest / 10U);
		} while (0U != rest);
		return std::string_view {pos, buffer.end()};

	}


	// Kernel integer to string function (sign only for decimal; written only if it fits buffer, no terminator)
	template<std::integral T>
	[[maybe_unused]]
	constexpr auto kitoa(const std::span<char> buffer, const T value, const radix_t radix = radix_t::DEC) noexcept -> char* {

		// Unsigned type of the same size
		using U = std::make_unsigned_t<T>;

		// Is value printed as negative decimal
		const auto isNegative	{std::cmp_less(value, static_cast<T>(0)) && (radix_t::DEC == radix)};
		// Magnitude for negative decimals, bit pattern otherwise
		const auto magnitude	{isNegative ? static_cast<U>(static_cast<U>(0) - static_cast<U>(value)) : static_cast<U>(value)};

		// Temporary buffer for value text representation (sign + all bits)
		auto temp		{std::array<char, std::numeric_limits<U>::digits + 1_usize> {}};
		const auto digits	{kdigits(std::span {temp}.template subspan<1_usize>(), magnitude, radix)};
		// Text start (sign right before digits)
		auto first		{temp.size() - digits.size()};
		if (isNegative) [[unlikely]] {
			temp[--first] = '-';
		}
		const auto text		{std::span {temp}.subspan(first)};

		// Copy only if it fits whole
		if (text.size() <= buffer.size()) [[likely]] {
			std::ranges::copy(text, buffer.begin());
		}

		// Return pointer to buffer
		return buffer.data();

	}


	// Kernel pointer to string function
	[[maybe_unused]]
	constexpr auto kptoa(const std::span<char> buffer, const igros_pointer_t value) noexcept -> char* {
		// Cast pointer value to size type
		return kitoa(buffer, std::bit_cast<igros_usize_t>(value), radix_t::HEX);
	}


	// Kernel vsnprintf function (returns written length without terminator)
	[[gnu::format(printf, 3, 0)]]
	auto	kvsnprintf(char* const buffer, const igros_usize_t size, const char* const format, std::va_list list) noexcept -> igros_usize_t;

	// Kernel snprintf function (returns written length without terminator)
	[[gnu::format(printf, 3, 4)]]
	auto	ksnprintf(char* const buffer, const igros_usize_t size, const char* const format, ...) noexcept -> igros_usize_t;

	// Kernel printf function (arguments are checked against format at compile time)
	[[gnu::format(printf, 1, 2)]]
	void	kprintf(const char* const format, ...) noexcept;


}	// namespace igros::klib

