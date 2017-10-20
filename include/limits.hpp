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


// Unsigned t_u8 limits
static const t_u8	t_u8_min	= 0x00;
static const t_u8	t_u8_max	= 0xFF;

// Unsigned t_u16 limits
static const t_u16	t_u16_min	= 0x0000;
static const t_u16	t_u16_max	= 0xFFFF;

// Unsigned t_u32 limits
static const t_u32	t_u32_min	= 0x00000000;
static const t_u32	t_u32_max	= 0xFFFFFFFF;

// Unsigned t_u64 limits
static const t_u64	t_u64_min	= 0x0000000000000000;
static const t_u64	t_u64_max	= 0xFFFFFFFFFFFFFFFF;


#endif	// IGROS_LIMITS_HPP

