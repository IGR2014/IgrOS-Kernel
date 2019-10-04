////////////////////////////////////////////////////////////////
//
//	Kernel math functions
//
//	File:	kmath.hpp
//	Date:	04 Oct 2019
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
//	All rights reserved.
//
//


#pragma once
#ifndef IGROS_KLIB_KERNEL_MATH_HPP
#define IGROS_KLIB_KERNEL_MATH_HPP


#include <cstdint>

#include <arch/types.hpp>


// Kernel library code zone
namespace klib {


	// Combined division & modulo result (unsigned)
	struct udivmod_t {

		quad_t quotient;
		quad_t reminder;

	};

	// Combined division & modulo result (signed)
	struct divmod_t {

		squad_t quotient;
		squad_t reminder;

	};


#if defined(IGROS_ARCH_i386)

	// Divide 64-bit integer by 32-bit integer
	// Returns 64-bit quotient and 64-bit reminder
	inline static udivmod_t kdivmod(quad_t dividend, dword_t divisor) {

		// Division result
		udivmod_t	res	= {0ULL, dividend};
		// Quotient bit
		quad_t		qbit	= 1ULL;

		// Division by 0
		if (0 == divisor) {
			res.quotient = quad_t(1 / divisor);
			res.reminder = 0ULL;
			return res;
		}

		while (0 <= sdword_t(divisor)) {
			divisor <<= 1ULL;
			qbit	<<= 1ULL;
		}

		while (qbit) {
			if (res.reminder >= divisor) {
				res.reminder	-= divisor;
				res.quotient	|= qbit;
			}
			divisor	>>= 1ULL;
			qbit	>>= 1ULL;
		}

		return res;

	}

#else

	// Divide 64-bit integer by 32-bit integer
	// Returns 64-bit quotient and 64-bit reminder
	inline udivmod_t kdivmod(quad_t dividend, dword_t divisor) {
		udivmod_t res = {
			(dividend / divisor),
			(dividend % divisor)
		};
		return res;
	}

#endif


}	// namespace arch


#endif	// IGROS_KLIB_KERNEL_MATH_HPP

