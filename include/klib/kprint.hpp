////////////////////////////////////////////////////////////////
//
//	Kernel text print functions
//
//	File:	kprint.hpp
//	Date:	08 Dec 2022
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <bit>
#include <cstdint>
#include <cstdarg>

#include <arch/types.hpp>


// Kernel library code zone
namespace igros::klib {


	// Integer representation types
	enum class radix_t : igros_byte_t {
		BIN	= 0x02_u8,			// (Base 2)	Binary integer format
		OCT	= 0x08_u8,			// (Base 8)	Oct integer format
		DEC	= 0x0A_u8,			// (Base 10)	Decimal integer format
		HEX	= 0x10_u8			// (Base 16)	Hexidemical integer format
	};


	// Kernel large unsigned integer to string function
	[[maybe_unused]]
	auto	kitoa(char* const buffer, const igros_usize_t size, const igros_quad_t value, const radix_t radix = radix_t::DEC) noexcept -> char*;
	// Kernel large integer to string function
	[[maybe_unused]]
	auto	kitoa(char* const buffer, const igros_usize_t size, const igros_squad_t value, const radix_t radix = radix_t::DEC) noexcept -> char*;

	// Kernel unsigned integer to string function
	[[maybe_unused]]
	inline auto kitoa(char* const buffer, const igros_usize_t size, const igros_dword_t value, const radix_t radix = radix_t::DEC) noexcept -> char* {
		return kitoa(buffer, size, static_cast<igros_usize_t>(value), radix);
	}
	// Kernel integer to string function
	[[maybe_unused]]
	inline auto kitoa(char* const buffer, const igros_usize_t size, const igros_sdword_t value, const radix_t radix = radix_t::DEC) noexcept -> char* {
		return kitoa(buffer, size, static_cast<igros_ssize_t>(value), radix);
	}

	// Kernel integer to string function
	[[maybe_unused]]
	inline auto kitoa(char* const buffer, const igros_usize_t size, const igros_word_t value, const radix_t radix = radix_t::DEC) noexcept -> char* {
		return kitoa(buffer, size, static_cast<igros_usize_t>(value), radix);
	}
	// Kernel integer to string function
	[[maybe_unused]]
	inline auto kitoa(char* const buffer, const igros_usize_t size, const igros_sword_t value, const radix_t radix = radix_t::DEC) noexcept -> char* {
		return kitoa(buffer, size, static_cast<igros_ssize_t>(value), radix);
	}

	// Kernel integer to string function
	[[maybe_unused]]
	inline auto kitoa(char* const buffer, const igros_usize_t size, const igros_byte_t value, const radix_t radix = radix_t::DEC) noexcept -> char* {
		return kitoa(buffer, size, static_cast<igros_usize_t>(value), radix);
	}
	// Kernel integer to string function
	[[maybe_unused]]
	inline auto kitoa(char* const buffer, const igros_usize_t size, const igros_sbyte_t value, const radix_t radix = radix_t::DEC) noexcept -> char* {
		return kitoa(buffer, size, static_cast<igros_ssize_t>(value), radix);
	}

	// Kernel size type to string function
	[[maybe_unused]]
	auto kstoa(char* const buffer, const igros_usize_t size, const igros_usize_t value, const radix_t radix = radix_t::DEC) noexcept -> char*;

	// Kernel pointer to string function
	[[maybe_unused]]
	inline auto kptoa(char* const buffer, const igros_usize_t size, const igros_pointer_t value) noexcept -> char* {
		return kstoa(buffer, size, std::bit_cast<igros_usize_t>(value), radix_t::HEX);
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

