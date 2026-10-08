////////////////////////////////////////////////////////////////
//
//	Kernel text print functions
//
//	File:	kprint.hpp
//	Date:	17 Mar 2023
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// C++
#include <array>
#include <bit>
#include <concepts>
#include <cstdarg>
#include <cstdint>
#include <limits>
#include <utility>
// IgrOS-Kernel arch
#include <arch/types.hpp>
// IgrOS-Kernel library
#include <klib/kmath.hpp>


// Kernel library code zone
namespace igros::klib {


	// Integer representation types
	enum class [[nodiscard]] radix_t : igros_byte_t {
		BIN	= 0x02_u8,			// (Base 2)	Binary integer format
		OCT	= 0x08_u8,			// (Base 8)	Oct integer format
		DEC	= 0x0A_u8,			// (Base 10)	Decimal integer format
		HEX	= 0x10_u8			// (Base 16)	Hexidemical integer format
	};

	// Kernel large unsigned integer to string function
	template<std::integral T>
	[[maybe_unused]]
	constexpr auto kitoa(char* const buffer, const igros_usize_t size, const T value, const radix_t radix = radix_t::DEC) noexcept -> char* {

		// Constant integer symbols values buffer
		constexpr auto	KITOA_CONST_BUFFER	{std::array {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'}};

		// Unsigned type of the same size
		using U = std::make_unsigned_t<T>;

		// Temporary buffer for value text representation (all bits + sign)
		auto tempBuffer {std::array<char, std::numeric_limits<U>::digits + 1_usize> {}};
		// Is value printed as negative decimal
		const auto isNegative {std::cmp_less(value, static_cast<T>(0)) && (radix_t::DEC == radix)};
		// Temporary unsigned copy of value (magnitude for negative decimals, bit pattern otherwise)
		auto tempValue	{isNegative ? static_cast<U>(static_cast<U>(0) - static_cast<U>(value)) : static_cast<U>(value)};

		// Setup counter to last - 1 position in temporary buffer
		auto pos	{static_cast<igros_usize_t>(tempBuffer.size())};

		// Loop through all digits while number is greater than base.
		// Digits are stored from the end of the start of temporary buffer
		// (this makes easier dealing with reverse routine by removing it)
		do {
			// Calculate divisio/modulo operation
			const auto divResult	{kdivmod<U>(tempValue, static_cast<U>(radix))};
			// Save current digit to temporary buffer
			tempBuffer[--pos]	= KITOA_CONST_BUFFER[divResult.reminder];
			// Divide value by base to remove current digit
			tempValue		= divResult.quotient;
		// The `do-while` instead of `while` here allows to process value fo zero
		} while (tempValue > static_cast<U>(0));

		// Binary, octal and hexidemical values have no sign
		if (isNegative) [[unlikely]] {
			// Write minus sign to buffer
			tempBuffer[--pos] = '-';
		}

		// Resulting string length
		auto strLength {tempBuffer.size() - pos};
		// Check size fit
		if (std::cmp_less_equal(strLength, size)) [[unlikely]] {
			// For values in tempBuffer
			for (auto i {0_usize}; i < std::min(strLength, size); i++) {
				// Revert temporary buffer we created from value to src buffer
				buffer[i] = tempBuffer[i + pos];
			}
		}

		// Return pointer to buffer
		return buffer;

	}


	// Kernel pointer to string function
	[[maybe_unused]]
	constexpr auto kptoa(char* const buffer, const igros_usize_t size, const igros_pointer_t value) noexcept -> char* {
		// Cast pointer value to size type
		return kitoa(buffer, size, std::bit_cast<igros_usize_t>(value), radix_t::HEX);
	}


	// Kernel vsnprintf function
	void	kvsnprintf(char* const buffer, const igros_usize_t size, const char* const format, std::va_list list) noexcept;

	// Kernel snprintf function
	void	ksnprintf(char* const buffer, const igros_usize_t size, const char* const format, ...) noexcept;
	// Kernel sprintf function
	void	ksprintf(char* const buffer, const char* const format, ...) noexcept;

	// Kernel printf function
	void	kprintf(const char* const format, ...) noexcept;


}	// namespace igros::klib

