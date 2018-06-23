////////////////////////////////////////////////////////////////
//
//	Common C++ types limits
//
//	File:	limits.hpp
//	Date:	23 Nov. 2017
//
//	Copyright (c) 2017, Igor Baklykov
//	All rights reserved.
//


#ifndef IGROS_LIMITS_HPP
#define IGROS_LIMITS_HPP


#include <types.hpp>


// Signed sbyte_t limits
static const sbyte_t	MIN_T_I8	= 0x80;
static const sbyte_t	MAX_T_I8	= 0x7F;

// Unsigned byte_t limits
static const byte_t	MIN_T_U8	= 0x00;
static const byte_t	MAX_T_U8	= 0xFF;

// Signed sword_t limits
static const sword_t	MIN_T_I16	= 0x8000;
static const sword_t	MAX_T_I16	= 0x7FFF;

// Unsigned word_t limits
static const word_t	MIN_T_U16	= 0x0000;
static const word_t	MAX_T_U16	= 0xFFFF;

// Signed sdword_t limits
static const sdword_t	MIN_T_I32	= 0x80000000;
static const sdword_t	MAX_T_I32	= 0x7FFFFFFF;

// Unsigned dword_t limits
static const dword_t	MIN_T_U32	= 0x00000000;
static const dword_t	MAX_T_U32	= 0xFFFFFFFF;

// Signed squad_t limits
static const squad_t	MIN_T_I64	= 0x8000000000000000;
static const squad_t	MAX_T_I64	= 0x7FFFFFFFFFFFFFFF;

// Unsigned quad_t limits
static const quad_t	MIN_T_U64	= 0x0000000000000000;
static const quad_t	MAX_T_U64	= 0xFFFFFFFFFFFFFFFF;


#endif	// IGROS_LIMITS_HPP

