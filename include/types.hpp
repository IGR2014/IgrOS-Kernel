////////////////////////////////////////////////////////////////
//
//	Common C++ types redefinition
//
//	File:	types.hpp
//	Date:	20 Nov. 2017
//
//	Copyright (c) 2017, Igor Baklykov
//	All rights reserved.
//


#ifndef TYPES_H
#define TYPES_H


// Unsigned integer types
typedef	unsigned char		t_u8;
typedef	unsigned short		t_u16;
typedef	unsigned int		t_u32;
typedef	unsigned long long	t_u64;

// Unsigned integer pointer types
typedef	unsigned char*		t_u8p;
typedef	unsigned short*		t_u16p;
typedef	unsigned int*		t_u32p;
typedef	unsigned long long*	t_u64p;


// Signed integer types
typedef	char		t_i8;
typedef	short		t_i16;
typedef	int		t_i32;
typedef	long long	t_i64;

// Signed integer pointer types
typedef	char*		t_i8p;
typedef	short*		t_i16p;
typedef	int*		t_i32p;
typedef	long long*	t_i64p;


// Floating point types
typedef	float	t_f32;
typedef	double	t_f64;

// Floating point pointer types
typedef	float*	t_f32p;
typedef	double*	t_f64p;


// Common pointer type
typedef void*   t_ptr;


#endif	// TYPES_H

