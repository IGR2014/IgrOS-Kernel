////////////////////////////////////////////////////////////////
//
//	Kernel string functions
//
//	File:	string.hpp
//	Date:	12 Sep 2019
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
//	All rights reserved.
//
//


#pragma once
#ifndef IGROS_KLIB_KERNEL_STRING_HPP
#define IGROS_KLIB_KERNEL_STRING_HPP


#include <cstdint>

#include <arch/types.hpp>


// Kernel library code zone
namespace klib {


	// Calculate string length
	std::size_t	kstrlen(const sbyte_t* src);

	// Copy string from one to other
	sbyte_t*	kstrcpy(sbyte_t* src, sbyte_t* dst, std::size_t size);
	// Copy string from one to other (const version)
	const sbyte_t*	kstrcpy(const sbyte_t* src, sbyte_t* dst, std::size_t size);

	// Compare strings
	sdword_t	kstrcmp(const sbyte_t* src1, const sbyte_t* src2, std::size_t size);

	// Find char occurrence in string
	sbyte_t*	kstrchr(sbyte_t* src, sbyte_t chr, std::size_t size);
	// Find char occurrence in string
	const sbyte_t*	kstrchr(const sbyte_t* src, sbyte_t chr, std::size_t size);


}	// namespace arch


#endif	// IGROS_KLIB_KERNEL_STRING_HPP

