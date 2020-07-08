////////////////////////////////////////////////////////////////
//
//	Kernel math functions declarations
//
//	File:	kmath.hpp
//	Date:	07 Jul 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#include <klib/kmath.hpp>


// Kernel library code zone
namespace igros::klib {


#if defined(IGROS_ARCH_i386)

	// Divide 64-bit integer by 32-bit integer
	// Returns 64-bit quotient and 64-bit reminder
	udivmod_t kudivmod(quad_t dividend, dword_t divisor) noexcept {

		// Division result
		udivmod_t	res	= {0ULL, dividend};
		// Quotient bit
		quad_t		qbit	= 1ULL;

		// Division by 0
		if (0 == divisor) {
			res.quotient = (1 / divisor);
			res.reminder = 0ULL;
			return res;
		}

		while (0 <= static_cast<sdword_t>(divisor)) {
			divisor <<= 1ULL;
			qbit	<<= 1ULL;
		}

		while (qbit) {
			if (res.reminder >= divisor) {
				res.reminder -= divisor;
				res.quotient |= qbit;
			}
			divisor	>>= 1ULL;
			qbit	>>= 1ULL;
		}

		return res;

	}


        // Divide 64-bit integer by 32-bit integer
	// Returns 64-bit quotient and 64-bit reminder
	divmod_t kdivmod(squad_t dividend, dword_t divisor) noexcept {

		// Division result
		divmod_t	res	= {0LL, dividend};
		// Quotient bit
		squad_t		qbit	= 1LL;

		// Division by 0
		if (0 == divisor) {
			res.quotient = (1 / divisor);
			res.reminder = 0LL;
			return res;
		}

		while (0 <= divisor) {
			divisor <<= 1LL;
			qbit	<<= 1LL;
		}

		while (qbit) {
			if (res.reminder >= divisor) {
				res.reminder -= divisor;
				res.quotient |= qbit;
			}
			divisor	>>= 1LL;
			qbit	>>= 1LL;
		}

		return res;

	}


#else

	// Divide 64-bit integer by 32-bit integer
	// Returns 64-bit quotient and 64-bit reminder
	udivmod_t kudivmod(quad_t dividend, dword_t divisor) noexcept {
		return {
			static_cast<quad_t>(dividend / divisor),
			static_cast<quad_t>(dividend % divisor)
		};
	}


        // Divide 64-bit integer by 32-bit integer
	// Returns 64-bit quotient and 64-bit reminder
	divmod_t kdivmod(squad_t dividend, sdword_t divisor) noexcept {
		return {
			static_cast<squad_t>(dividend / divisor),
			static_cast<squad_t>(dividend % divisor)
		};
	}

#endif


}	// namespace igros::klib

