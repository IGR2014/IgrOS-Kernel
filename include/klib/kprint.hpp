////////////////////////////////////////////////////////////////
//
//	Kernel text print functions
//
//	File:	kprint.hpp
//	Date:	01 Feb 2021
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <cstdint>
#include <cstdarg>

#include <arch/types.hpp>


// Kernel library code zone
namespace igros::klib {


	// Integer representation types
	enum class radix_t : byte_t {
		BIN	= 0x02,			// (Base 2)	Binary integer format
		OCT	= 0x08,			// (Base 8)	Oct integer format
		DEC	= 0x0A,			// (Base 10)	Decimal integer format
		HEX	= 0x10			// (Base 16)	Hexidemical integer format
	};


	// Kernel large unsigned integer to string function
	sbyte_t* kitoa(sbyte_t* buffer, const std::size_t size, const quad_t value, const radix_t radix = radix_t::DEC) noexcept;
	// Kernel large integer to string function
	sbyte_t* kitoa(sbyte_t* buffer, const std::size_t size, const squad_t value, const radix_t radix = radix_t::DEC) noexcept;

	// Kernel unsigned integer to string function
	inline sbyte_t* kitoa(sbyte_t* buffer, const std::size_t size, const dword_t value, const radix_t radix = radix_t::DEC) noexcept {
		return kitoa(buffer, size, static_cast<quad_t>(value), radix);
	}
	// Kernel integer to string function
	inline sbyte_t* kitoa(sbyte_t* buffer, const std::size_t size, const sdword_t value, const radix_t radix = radix_t::DEC) noexcept {
		return kitoa(buffer, size, static_cast<squad_t>(value), radix);
	}

	// Kernel integer to string function
	inline sbyte_t* kitoa(sbyte_t* buffer, const std::size_t size, const word_t value, const radix_t radix = radix_t::DEC) noexcept {
		return kitoa(buffer, size, static_cast<quad_t>(value), radix);
	}
	// Kernel integer to string function
	inline sbyte_t* kitoa(sbyte_t* buffer, const std::size_t size, const sword_t value, const radix_t radix = radix_t::DEC) noexcept {
		return kitoa(buffer, size, static_cast<squad_t>(value), radix);
	}

	// Kernel integer to string function
	inline sbyte_t* kitoa(sbyte_t* buffer, const std::size_t size, const byte_t value, const radix_t radix = radix_t::DEC) noexcept {
		return kitoa(buffer, size, static_cast<quad_t>(value), radix);
	}
	// Kernel integer to string function
	inline sbyte_t* kitoa(sbyte_t* buffer, const std::size_t size, const sbyte_t value, const radix_t radix = radix_t::DEC) noexcept {
		return kitoa(buffer, size, static_cast<squad_t>(value), radix);
	}

	// Kernel size type to string function
	sbyte_t* kstoa(sbyte_t* buffer, const std::size_t size, const std::size_t value, const radix_t radix = radix_t::DEC) noexcept;

	// Kernel pointer to string function
	inline sbyte_t* kptoa(sbyte_t* buffer, const std::size_t size, const pointer_t value) noexcept {
		return kstoa(buffer, size, reinterpret_cast<std::size_t>(value), radix_t::HEX);
	}


	// Kernel vsnprintf function
	void kvsnprintf(sbyte_t* buffer, const std::size_t size, const sbyte_t* format, va_list list) noexcept;

	// Kernel snprintf function
	void ksnprintf(sbyte_t* buffer, const std::size_t size, const sbyte_t* format, ...) noexcept;
	// Kernel sprintf function
	void ksprintf(sbyte_t* buffer, const sbyte_t* format, ...) noexcept;

	// Kernel printf function
	void kprintf(const sbyte_t* format, ...) noexcept;


}	// namespace igros::klib

