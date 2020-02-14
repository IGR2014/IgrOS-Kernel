////////////////////////////////////////////////////////////////
//
//	Kernel text print functions
//
//	File:	kprint.hpp
//	Date:	12 Feb 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#pragma once
#ifndef IGROS_KLIB_KERNEL_PRINT_HPP
#define IGROS_KLIB_KERNEL_PRINT_HPP


#include <cstdint>
#include <cstdarg>

#include <types.hpp>


// Kernel library code zone
namespace klib {


	// Integer representation types
	enum class base : byte_t {
		BIN	= 0x02,			// (Base 2)	Binary integer format
		OCT	= 0x08,			// (Base 8)	Oct integer format
		DEC	= 0x0A,			// (Base 10)	Decimal integer format
		HEX	= 0x10			// (Base 16)	Hexidemical integer format
	};


	// Kernel large unsigned integer to string function
	sbyte_t* kitoa(sbyte_t* buffer, std::size_t size, const quad_t value, const base radix = base::DEC) noexcept;

	// Kernel large integer to string function
	inline sbyte_t* kitoa(sbyte_t* buffer, std::size_t size, const squad_t value, const base radix = base::DEC) noexcept {
		return kitoa(buffer, size, quad_t(value), radix);
	}


	// Kernel unsigned integer to string function
	sbyte_t* kitoa(sbyte_t* buffer, std::size_t size, const dword_t value, const base radix = base::DEC) noexcept;

	// Kernel integer to string function
	inline sbyte_t* kitoa(sbyte_t* buffer, std::size_t size, const sdword_t value, const base radix = base::DEC) noexcept {
		return kitoa(buffer, size, dword_t(value), radix);
	}


	// Kernel integer to string function
	inline sbyte_t* kitoa(sbyte_t* buffer, std::size_t size, const word_t value, const base radix = base::DEC) noexcept {
		return kitoa(buffer, size, dword_t(value), radix);
	}

	// Kernel integer to string function
	inline sbyte_t* kitoa(sbyte_t* buffer, std::size_t size, const sword_t value, const base radix = base::DEC) noexcept {
		return kitoa(buffer, size, dword_t(value), radix);
	}


	// Kernel integer to string function
	inline sbyte_t* kitoa(sbyte_t* buffer, std::size_t size, const byte_t value, const base radix = base::DEC) noexcept {
		return kitoa(buffer, size, dword_t(value), radix);
	}

	// Kernel integer to string function
	inline sbyte_t* kitoa(sbyte_t* buffer, std::size_t size, const sbyte_t value, const base radix = base::DEC) noexcept {
		return kitoa(buffer, size, sdword_t(value), radix);
	}


	// Kernel size type to string function
	sbyte_t* kstoa(sbyte_t* buffer, std::size_t size, const std::size_t value, const base radix = base::DEC) noexcept;


	// Kernel pointer to string function
	inline sbyte_t* kptoa(sbyte_t* buffer, std::size_t size, const void* value) noexcept {
		return kstoa(buffer, size, std::size_t(value), base::HEX);
	}


	/*
	// Kernel variadic arguments list va_list
	struct kvaList {

		std::size_t listArg = 0u;		// Current list arg

		// Kernel variadic arguments list va_start
		template<typename T>
		inline void start(const T &arg) noexcept;
		// Kernel variadic arguments list va_end
		inline void end() noexcept;

		// Kernel variadic arguments list va_arg
		template<typename T>
		inline const T& arg() noexcept;

	};


	// Kernel variadic arguments list va_start macros
	template<typename T>
	void kvaList::start(const T &arg) noexcept {
		listArg = std::size_t(&arg) + sizeof(T);
	}

	// Kernel variadic arguments list va_end macros
	void kvaList::end() noexcept {
		listArg = 0u;
	}

	// Kernel variadic arguments list va_arg macros
	template<typename T>
	const T& kvaList::arg() noexcept {
		const T* arg = reinterpret_cast<const T*>(listArg);
		listArg = std::size_t(arg + 1);
		return *arg;
	}
	*/

	// Kernel vsnprintf function
	void kvsnprintf(sbyte_t* buffer, const std::size_t size, const sbyte_t* format, va_list list/*kvaList &list*/) noexcept;

	// Kernel snprintf function
	void ksnprintf(sbyte_t* buffer, const std::size_t size, const sbyte_t* format, ...) noexcept;
	// Kernel sprintf function
	void ksprintf(sbyte_t* buffer, const sbyte_t* format, ...) noexcept;

	// Kernel printf function
	void kprintf(const sbyte_t* format, ...) noexcept;


}	// namespace klib


#endif	// IGROS_KLIB_KERNEL_PRINT_HPP

