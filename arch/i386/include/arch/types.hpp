////////////////////////////////////////////////////////////////
//
//	Common C++ types redefinition
//
//	File:	types.hpp
//	Date:	09 Oct 2019
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#pragma once
#ifndef IGROS_TYPES_HPP
#define IGROS_TYPES_HPP


// Unsigned integer types
using   byte_t          = unsigned char;
using	word_t          = unsigned short;
using	dword_t         = unsigned int;
using	quad_t          = unsigned long long int;


// Signed integer types
using	sbyte_t         = char;
using	sword_t         = short;
using	sdword_t        = int;
using	squad_t         = long long int;


// Floating point types
using	real32_t        = float;
using   real64_t        = double;


// Common pointer type
using	pointer_t       = void*;


#endif	// IGROS_TYPES_HPP

