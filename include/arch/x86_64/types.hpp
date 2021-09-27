////////////////////////////////////////////////////////////////
//
//	Common C++ types redefinition
//
//	File:	types.hpp
//	Date:	27 Sep 2021
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// C++
#include <type_traits>


// OS namespace
namespace igros {


	// Unsigned integer types
	using	byte_t		= unsigned char;
	using	word_t		= unsigned short;
	using	dword_t		= unsigned int;
	using	quad_t		= unsigned long long int;

	// Signed integer types
	using	sbyte_t		= char;
	using	sword_t		= short;
	using	sdword_t	= int;
	using	squad_t		= long long int;

	// Floating point types
	using	real32_t	= float;
	using	real64_t	= double;

	// Common pointer type
	using	pointer_t	= std::add_pointer_t<void>;


}	// namespace igros

