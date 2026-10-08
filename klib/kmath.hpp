////////////////////////////////////////////////////////////////
//
//	Kernel math functions definitions
//
//	File:	kmath.hpp
//	Date:	19 Mar 2023
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// C++
#include <concepts>
#include <cstdint>
#include <utility>
// IgrOS-Kernel arch
#include <arch/types.hpp>


// Kernel library code zone
namespace igros::klib {


	// Combined division & modulo result (signed)
	template<std::integral T>
	struct divmod_t final {
		T	quotient;
		T	reminder;
	};


	// Divide signed by integer
	// Returns quotient and reminder
	template<std::integral T1, std::integral T2 = T1>
	[[nodiscard]]
	constexpr auto kdivmod(T1 dividend, T2 divisor) noexcept -> divmod_t<T1> {
		// Simply return result
		return divmod_t<T1> {
			static_cast<T1>(dividend / divisor),
			static_cast<T1>(dividend % divisor)
		};
	}


// Is arch i386 ?
#if	defined(IGROS_ARCH_i386)


	// i386 has no 64-bit division instruction, so 64-bit "/" and "%" would require
	// libgcc helpers (__udivdi3/__umoddi3) - use shift-subtract long division instead

	// Divide unsigned integer by unsigned integer (igros_quad_t / igros_quad_t overload)
	// Returns unsigned quotient and unsigned reminder (division by zero returns {0, dividend})
	template<>
	[[nodiscard]]
	constexpr auto kdivmod(igros_quad_t dividend, igros_quad_t divisor) noexcept -> divmod_t<igros_quad_t> {

		// Division result
		auto res	{divmod_t<igros_quad_t> {0_u64, dividend}};
		// Quotient bit
		auto qbit	{1_u64};

		// Division by zero
		if (0_u64 == divisor) [[unlikely]] {
			return res;
		}

		// Align divisor's highest bit with dividend's highest bit (without overflowing)
		while ((divisor < res.reminder) && (0_u64 == (divisor & 0x8000000000000000_u64))) {
			divisor <<= 1;
			qbit	<<= 1;
		}

		// Subtract shifted divisor from highest to lowest quotient bit
		while (0_u64 != qbit) {
			if (res.reminder >= divisor) {
				res.reminder -= divisor;
				res.quotient |= qbit;
			}
			divisor	>>= 1;
			qbit	>>= 1;
		}

		return res;

	}

	// Divide unsigned integer by unsigned integer (igros_quad_t / igros_dword_t overload)
	// Returns unsigned quotient and unsigned reminder
	template<>
	[[nodiscard]]
	constexpr auto kdivmod(igros_quad_t dividend, igros_dword_t divisor) noexcept -> divmod_t<igros_quad_t> {
		return kdivmod(dividend, static_cast<igros_quad_t>(divisor));
	}


	// Divide signed integer by signed integer (igros_squad_t / igros_squad_t overload)
	// Returns signed quotient and signed reminder (truncation towards zero, like C++ "/" and "%")
	template<>
	[[nodiscard]]
	constexpr auto kdivmod(igros_squad_t dividend, igros_squad_t divisor) noexcept -> divmod_t<igros_squad_t> {

		// Absolute values (computed unsigned, so INT64_MIN doesn't overflow)
		const auto absDividend	{(dividend < 0_i64) ? (0_u64 - static_cast<igros_quad_t>(dividend)) : static_cast<igros_quad_t>(dividend)};
		const auto absDivisor	{(divisor < 0_i64) ? (0_u64 - static_cast<igros_quad_t>(divisor)) : static_cast<igros_quad_t>(divisor)};

		// Unsigned division
		const auto res		{kdivmod(absDividend, absDivisor)};

		// Quotient is negative when signs differ, reminder takes dividend sign
		const auto quotient	{((dividend < 0_i64) != (divisor < 0_i64)) ? (0_u64 - res.quotient) : res.quotient};
		const auto reminder	{(dividend < 0_i64) ? (0_u64 - res.reminder) : res.reminder};

		return divmod_t<igros_squad_t> {
			static_cast<igros_squad_t>(quotient),
			static_cast<igros_squad_t>(reminder)
		};

	}

	// Divide signed integer by signed integer (igros_squad_t / igros_sdword_t overload)
	// Returns signed quotient and signed reminder
	template<>
	[[nodiscard]]
	constexpr auto kdivmod(igros_squad_t dividend, igros_sdword_t divisor) noexcept -> divmod_t<igros_squad_t> {
		return kdivmod(dividend, static_cast<igros_squad_t>(divisor));
	}


#endif


}	// namespace igros::klib

