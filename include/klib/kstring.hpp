////////////////////////////////////////////////////////////////
//
//	Kernel string functions
//
//	File:	string.hpp
//	Date:	22 May 2019
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
//	All rights reserved.
//
//


#pragma once
#ifndef IGROS_KLIB_KERNEL_STRING_HPP
#define IGROS_KLIB_KERNEL_STRING_HPP


#include <arch/types.hpp>


// Kernel library code zone
namespace klib {


	// Calculate string length
	dword_t	kstrlen(const sbyte_t* src);

	// Copy string from one to other
	void	kstrcpy(const sbyte_t* src, sbyte_t* dst, const dword_t size);


}	// namespace arch


#endif	// IGROS_KLIB_KERNEL_STRING_HPP

