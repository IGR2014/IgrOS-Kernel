////////////////////////////////////////////////////////////////
//
//	Kernel text print functions
//
//	File:	kprint.hpp
//	Date:	04 Oct 2019
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
//	All rights reserved.
//
//


#pragma once
#ifndef IGROS_KLIB_KERNEL_PRINT_HPP
#define IGROS_KLIB_KERNEL_PRINT_HPP


#include <cstdint>

#include <arch/types.hpp>


// Kernel library code zone
namespace klib {


	// Integer representation types
	enum class base : byte_t {

		BIN	= 0x02,			// (Base 2)	Binary integer format
		OCT	= 0x08,			// (Base 8)	Oct integer format
		DEC	= 0x0A,			// (Base 10)	Decimal integer format
		HEX	= 0x10			// (Base 16)	Hexidemical integer format

	};


	// Kernel integer to string function
	sbyte_t*	kitoa(sbyte_t* buffer, std::size_t size, byte_t value, const base radix = base::DEC);
	// Kernel integer to string function
	sbyte_t*	kitoa(sbyte_t* buffer, std::size_t size, sbyte_t value, const base radix = base::DEC);

	// Kernel integer to string function
	sbyte_t*	kitoa(sbyte_t* buffer, std::size_t size, word_t value, const base radix = base::DEC);
	// Kernel integer to string function
	sbyte_t*	kitoa(sbyte_t* buffer, std::size_t size, sword_t value, const base radix = base::DEC);

	// Kernel integer to string function
	sbyte_t*	kitoa(sbyte_t* buffer, std::size_t size, dword_t value, const base radix = base::DEC);
	// Kernel integer to string function
	sbyte_t*	kitoa(sbyte_t* buffer, std::size_t size, sdword_t value, const base radix = base::DEC);

	// Kernel integer to string function
	sbyte_t*	kitoa(sbyte_t* buffer, std::size_t size, quad_t value, const base radix = base::DEC);
	// Kernel integer to string function
	sbyte_t*	kitoa(sbyte_t* buffer, std::size_t size, squad_t value, const base radix = base::DEC);

	// Kernel pointer to string function
	sbyte_t*	kptoa(sbyte_t* buffer, std::size_t size, pointer_t value, const base radix = base::HEX);


	// TODO Implement kernel print function
	void kprint(const sbyte_t* format, ...);


}	// namespace klib


#endif	// IGROS_KLIB_KERNEL_PRINT_HPP

