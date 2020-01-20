////////////////////////////////////////////////////////////////
//
//	Common C++ types limits
//
//	File:	limits.hpp
//	Date:	17 Jan 2020
//
//	Copyright (c) 2017, Igor Baklykov
//	All rights reserved.
//
//


#pragma once
#ifndef IGROS_LIMITS_HPP
#define IGROS_LIMITS_HPP


#include <limits>

#include <types.hpp>


// Signed sbyte_t limits
static const sbyte_t	MIN_T_I8	= std::numeric_limits<sbyte_t>::min();
static const sbyte_t	MAX_T_I8	= std::numeric_limits<sbyte_t>::max();

// Unsigned byte_t limits
static const byte_t	MIN_T_U8	= std::numeric_limits<byte_t>::min();
static const byte_t	MAX_T_U8	= std::numeric_limits<byte_t>::max();

// Signed sword_t limits
static const sword_t	MIN_T_I16	= std::numeric_limits<sword_t>::min();
static const sword_t	MAX_T_I16	= std::numeric_limits<sword_t>::max();

// Unsigned word_t limits
static const word_t	MIN_T_U16	= std::numeric_limits<word_t>::min();
static const word_t	MAX_T_U16	= std::numeric_limits<word_t>::max();

// Signed sdword_t limits
static const sdword_t	MIN_T_I32	= std::numeric_limits<sdword_t>::min();
static const sdword_t	MAX_T_I32	= std::numeric_limits<sdword_t>::max();

// Unsigned dword_t limits
static const dword_t	MIN_T_U32	= std::numeric_limits<dword_t>::min();
static const dword_t	MAX_T_U32	= std::numeric_limits<dword_t>::max();

// Signed squad_t limits
static const squad_t	MIN_T_I64	= std::numeric_limits<squad_t>::min();
static const squad_t	MAX_T_I64	= std::numeric_limits<squad_t>::max();

// Unsigned quad_t limits
static const quad_t	MIN_T_U64	= std::numeric_limits<quad_t>::min();
static const quad_t	MAX_T_U64	= std::numeric_limits<quad_t>::max();


#endif	// IGROS_LIMITS_HPP

