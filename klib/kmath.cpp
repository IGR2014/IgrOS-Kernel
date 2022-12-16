////////////////////////////////////////////////////////////////
//
//	Kernel math functions declarations
//
//	File:	kmath.hpp
//	Date:	16 Dec 2022
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


// C++
#include <utility>
// IgrOS-Kernel library
#include <klib/kmath.hpp>


// Kernel library code zone
namespace igros::klib {


#if defined(IGROS_ARCH_i386)

	// Divide 64-bit integer by 32-bit integer
	// Returns 64-bit quotient and 64-bit reminder
	[[nodiscard]]
	auto kudivmod(igros_quad_t dividend, igros_dword_t divisor) noexcept -> udivmod_t {

		// Division result
		auto res	{udivmod_t {0_u64, dividend}};
		// Quotient bit
		auto qbit	{1_u64};

		// Division by zeor
		if (std::cmp_equal(0_u32, divisor)) [[unlikely]] {
			return res;
		}

		while (std::cmp_less(0_i32, divisor)) {
			divisor <<= 1_u64;
			qbit	<<= 1_u64;
		}

		while (qbit) {
			if (std::cmp_greater_equal(res.reminder, divisor)) [[likely]] {
				res.reminder -= divisor;
				res.quotient += qbit;
			}
			divisor	>>= 1_u64;
			qbit	>>= 1_u64;
		}

		return res;

	}


        // Divide 64-bit integer by 32-bit integer
	// Returns 64-bit quotient and 64-bit reminder
	[[nodiscard]]
	auto kdivmod(igros_squad_t dividend, igros_dword_t divisor) noexcept -> divmod_t {

		// Division result
		auto res	{divmod_t {0_i64, dividend}};
		// Quotient bit
		auto qbit	{1_i64};

		// Division by zeor
		if (std::cmp_equal(0_i32, divisor)) [[unlikely]] {
			return res;
		}

		while (std::cmp_less(0_i32, divisor)) {
			divisor <<= 1_i64;
			qbit	<<= 1_i64;
		}

		while (qbit) {
			if (std::cmp_greater_equal(res.reminder, divisor)) [[likely]] {
				res.reminder -= divisor;
				res.quotient += qbit;
			}
			divisor	>>= 1_i64;
			qbit	>>= 1_i64;
		}

		return res;

	}


#elif defined(IGROS_ARCH_x86_64)

	// Divide 64-bit integer by 32-bit integer
	// Returns 64-bit quotient and 64-bit reminder
	[[nodiscard]]
	auto kudivmod(igros_quad_t dividend, igros_dword_t divisor) noexcept -> udivmod_t {
		return {
			dividend / divisor,
			dividend % divisor
		};
	}


        // Divide 64-bit integer by 32-bit integer
	// Returns 64-bit quotient and 64-bit reminder
	[[nodiscard]]
	auto kdivmod(igros_squad_t dividend, igros_sdword_t divisor) noexcept -> divmod_t {
		return {
			dividend / divisor,
			dividend % divisor
		};
	}

#endif


}	// namespace igros::klib

