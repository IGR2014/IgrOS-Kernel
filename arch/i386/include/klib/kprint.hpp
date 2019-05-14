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
#ifndef IGROS_ARCH_KERNEL_PRINT_HPP
#define IGROS_ARCH_KERNEL_PRINT_HPP


#include <arch/types.hpp>


// Arch-dependent code zone
namespace arch {


	// Integer representation base
	enum class base_t : byte_t {

		BIN	= 0x00,
		OCT	= 0x01,
		DEC	= 0x02,
		HEX	= 0x03

	};

	// TODO Implement integer to string conversion
	void kitoa(const quad_t, const byte_t*, const base_t &);

	// TODO Implement kernel print function
	void kprint(const byte_t*, ...);


}	// namespace arch


#endif	// IGROS_ARCH_KERNEL_PRINT_HPP

