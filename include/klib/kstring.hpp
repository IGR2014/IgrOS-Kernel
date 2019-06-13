////////////////////////////////////////////////////////////////
//
//	Kernel string functions
//
//	File:	string.hpp
//	Date:	14 Jun 2019
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
	sbyte_t*	kstrcpy(const sbyte_t* src, sbyte_t* dst, std::size_t size);


}	// namespace arch


#endif	// IGROS_KLIB_KERNEL_STRING_HPP

