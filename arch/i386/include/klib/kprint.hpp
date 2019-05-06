////////////////////////////////////////////////////////////////
//
//	Kernel text print functions
//
//	File:	kprint.hpp
//	Date:	06 May 2019
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
//	All rights reserved.
//
//


#ifndef IGROS_ARCH_KERNEL_PRINT_HPP
#define IGROS_ARCH_KERNEL_PRINT_HPP


#include <arch/types.hpp>


// Arch-dependent code zone
namespace arch {


	// TODO Implement kernel print function
	void kprint(const byte_t*, ...);


}	// namespace arch


#endif	// IGROS_ARCH_KERNEL_PRINT_HPP

