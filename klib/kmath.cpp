////////////////////////////////////////////////////////////////
//
//	Kernel math functions declarations
//
//	File:	kmath.hpp
//	Date:	27 Sep 2021
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#include <klib/kmath.hpp>


// Kernel library code zone
namespace igros::klib {


#if defined(IGROS_ARCH_i386)

	// Divide 64-bit integer by 32-bit integer
	// Returns 64-bit quotient and 64-bit reminder
	[[nodiscard]]
	udivmod_t kudivmod(quad_t dividend, dword_t divisor) noexcept {

		// Division result
		auto res	= udivmod_t {0ULL, dividend};
		// Quotient bit
		auto qbit	= quad_t {1ULL};

		// Division by zeor
		if (0 == divisor) {
			return res;
		}

		while (0 < static_cast<sdword_t>(divisor)) {
			divisor <<= 1ULL;
			qbit	<<= 1ULL;
		}

		while (qbit) {
			if (res.reminder >= divisor) {
				res.reminder -= divisor;
				res.quotient += qbit;
			}
			divisor	>>= 1ULL;
			qbit	>>= 1ULL;
		}

		return res;

	}


        // Divide 64-bit integer by 32-bit integer
	// Returns 64-bit quotient and 64-bit reminder
	[[nodiscard]]
	divmod_t kdivmod(squad_t dividend, dword_t divisor) noexcept {

		// Division result
		auto res	= divmod_t {0LL, dividend};
		// Quotient bit
		auto qbit	= squad_t {1LL};

		// Division by zeor
		if (0 == divisor) {
			return res;
		}

		while (0 < divisor) {
			divisor <<= 1LL;
			qbit	<<= 1LL;
		}

		while (qbit) {
			if (res.reminder >= divisor) {
				res.reminder -= divisor;
				res.quotient += qbit;
			}
			divisor	>>= 1LL;
			qbit	>>= 1LL;
		}

		return res;

	}


#elif defined(IGROS_ARCH_x86_64)

	// Divide 64-bit integer by 32-bit integer
	// Returns 64-bit quotient and 64-bit reminder
	udivmod_t kudivmod(quad_t dividend, dword_t divisor) noexcept {
		return {
			dividend / divisor,
			dividend % divisor
		};
	}


        // Divide 64-bit integer by 32-bit integer
	// Returns 64-bit quotient and 64-bit reminder
	divmod_t kdivmod(squad_t dividend, sdword_t divisor) noexcept {
		return {
			dividend / divisor,
			dividend % divisor
		};
	}

#endif


}	// namespace igros::klib

