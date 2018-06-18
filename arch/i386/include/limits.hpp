////////////////////////////////////////////////////////////////
//
//	Common C++ types limits
//
//	File:	limits.hpp
//	Date:	21 Nov. 2017
//
//	Copyright (c) 2017, Igor Baklykov
//	All rights reserved.
//


#ifndef IGROS_LIMITS_HPP
#define IGROS_LIMITS_HPP


#include <types.hpp>


// Signed t_i8 limits
static const t_i8	MIN_T_I8	= 0x80;
static const t_i8	MAX_T_I8	= 0x7F;

// Unsigned t_u8 limits
static const t_u8	MIN_T_U8	= 0x00;
static const t_u8	MAX_T_U8	= 0xFF;

// Signed t_i16 limits
static const t_i16	MIN_T_I16	= 0x8000;
static const t_i16	MAX_T_I16	= 0x7FFF;

// Unsigned t_u16 limits
static const t_u16	MIN_T_U16	= 0x0000;
static const t_u16	MAX_T_U16	= 0xFFFF;

// Signed t_i32 limits
static const t_i32	MIN_T_I32	= 0x80000000;
static const t_i32	MAX_T_I32	= 0x7FFFFFFF;

// Unsigned t_u32 limits
static const t_u32	MIN_T_U32	= 0x00000000;
static const t_u32	MAX_T_U32	= 0xFFFFFFFF;

// Signed t_i64 limits
static const t_i64	MIN_T_I64	= 0x8000000000000000;
static const t_i64	MAX_T_I64	= 0x7FFFFFFFFFFFFFFF;

// Unsigned t_u64 limits
static const t_u64	MIN_T_U64	= 0x0000000000000000;
static const t_u64	MAX_T_U64	= 0xFFFFFFFFFFFFFFFF;


#endif	// IGROS_LIMITS_HPP

