////////////////////////////////////////////////////////////////
//
//	Kernel text print functions
//
//	File:	kprint.hpp
//	Date:	15 May 2019
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
//	All rights reserved.
//
//


#pragma once
#ifndef IGROS_KLIB_KERNEL_PRINT_HPP
#define IGROS_KLIB_KERNEL_PRINT_HPP


#include <arch/types.hpp>


// Kernel library code zone
namespace klib {


	// Integer representation types
	enum class base : byte_t {

		BIN	= 0x02,			// Binary integer format
		OCT	= 0x08,			// Oct integer format
		DEC	= 0x0A,			// Decimal integer format
		HEX	= 0x10			// Hexidemical integer format

	};


	// Kernel integer to string function
	void kitoa(sbyte_t* buffer, dword_t size, byte_t value, const base radix = base::DEC);
	// Kernel integer to string function
	void kitoa(sbyte_t* buffer, dword_t size, sbyte_t value, const base radix = base::DEC);

	// Kernel integer to string function
	void kitoa(sbyte_t* buffer, dword_t size, word_t value, const base radix = base::DEC);
	// Kernel integer to string function
	void kitoa(sbyte_t* buffer, dword_t size, sword_t value, const base radix = base::DEC);

	// Kernel integer to string function
	void kitoa(sbyte_t* buffer, dword_t size, dword_t value, const base radix = base::DEC);
	// Kernel integer to string function
	void kitoa(sbyte_t* buffer, dword_t size, sdword_t value, const base radix = base::DEC);

	// Kernel integer to string function
	void kitoa(sbyte_t* buffer, dword_t size, quad_t value, const base radix = base::DEC);
	// Kernel integer to string function
	void kitoa(sbyte_t* buffer, dword_t size, squad_t value, const base radix = base::DEC);


	// TODO Implement kernel print function
	void kprint(const sbyte_t* format, ...);


}	// namespace klib


#endif	// IGROS_KLIB_KERNEL_PRINT_HPP

