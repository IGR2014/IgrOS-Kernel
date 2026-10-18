////////////////////////////////////////////////////////////////
//
//	Kernel math functions definitions
//
//	File:	kMath.hpp
//	Date:	19 Mar 2023
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// C++
#include <bit>
#include <concepts>
#include <type_traits>
// IgrOS-Kernel arch
#include <arch/types.hpp>


// Kernel library code zone
namespace igros::klib {


	// Combined division & modulo result
	template<std::integral T>
	struct divmod_t final {
		T	quotient;		// Quotient (truncated towards zero)
		T	remainder;		// Remainder (sign of dividend)

		// Comparison
		constexpr auto	operator==(const divmod_t &other) const noexcept -> bool = default;
	};


	// Integers of the same signedness (mixed ones convert signed to unsigned silently)
	template<class T, class D>
	concept kdivisible = std::integral<T> && std::integral<D> && (std::is_signed_v<T> == std::is_signed_v<D>);


	// 64-bit unsigned division for 32-bit CPU (no 64-bit "/" and "%" without libgcc __udivdi3/__umoddi3)
	// Division by zero returns {0, dividend}
	[[nodiscard]]
	constexpr auto kudivmod64(const igros_quad_t dividend, const igros_quad_t divisor) noexcept -> divmod_t<igros_quad_t> {

#if	defined (IGROS_ARCH_i386)
		// Divisor fits 32 bits: two hardware divisions (high part, then remainder:low part)
		if (!std::is_constant_evaluated() && (0_u64 != divisor) && (0_u64 == (divisor >> 32))) {
			const auto divisor32	{static_cast<igros_dword_t>(divisor)};
			const auto high		{static_cast<igros_dword_t>(dividend >> 32)};
			const auto low		{static_cast<igros_dword_t>(dividend)};
			const auto quotientHigh	{high / divisor32};
			auto remainder		{high % divisor32};
			auto quotientLow	{igros_dword_t {}};
			// EDX:EAX / divisor (EDX < divisor, so quotient fits EAX)
			asm (
				"divl %[divisor]"
				: "=a" (quotientLow), "+d" (remainder)
				: "a" (low), [divisor] "rm" (divisor32)
				: "cc"
			);
			return divmod_t<igros_quad_t> {
				(static_cast<igros_quad_t>(quotientHigh) << 32) | quotientLow,
				remainder
			};
		}
#endif

		// Divisor greater than dividend (or zero)
		if ((0_u64 == divisor) || (divisor > dividend)) {
			return divmod_t<igros_quad_t> {0_u64, dividend};
		}

		// Shift-subtract long division: align divisor highest bit with dividend highest bit
		const auto shift	{std::countl_zero(divisor) - std::countl_zero(dividend)};
		auto aligned		{divisor << shift};
		auto res		{divmod_t<igros_quad_t> {0_u64, dividend}};
		// One quotient bit per shift
		for (auto bit {0}; bit <= shift; ++bit) {
			res.quotient <<= 1;
			if (res.remainder >= aligned) {
				res.remainder	-= aligned;
				res.quotient	|= 1_u64;
			}
			aligned >>= 1;
		}
		return res;

	}


	// Divide integer by integer of the same signedness
	// Returns quotient (truncated towards zero) and remainder (sign of dividend), like C++ "/" and "%"
	// Divisor must not be zero (traps like native division, except software 64-bit division on i386)
	template<std::integral T, std::integral D>
	requires kdivisible<T, D>
	[[nodiscard]]
	constexpr auto kdivmod(const T dividend, const D divisor) noexcept -> divmod_t<T> {

		// Native division for CPU word sized values
		if constexpr ((sizeof(T) <= sizeof(igros_usize_t)) && (sizeof(D) <= sizeof(igros_usize_t))) {
			return divmod_t<T> {
				static_cast<T>(dividend / divisor),
				static_cast<T>(dividend % divisor)
			};

		// Unsigned values wider than CPU word
		} else if constexpr (std::is_unsigned_v<T>) {
			const auto res {kudivmod64(dividend, divisor)};
			return divmod_t<T> {
				static_cast<T>(res.quotient),
				static_cast<T>(res.remainder)
			};

		// Signed values wider than CPU word: divide magnitudes
		} else {
			const auto absDividend	{(dividend < 0) ? (0_u64 - static_cast<igros_quad_t>(dividend)) : static_cast<igros_quad_t>(dividend)};
			const auto absDivisor	{(divisor < 0) ? (0_u64 - static_cast<igros_quad_t>(divisor)) : static_cast<igros_quad_t>(divisor)};
			const auto res		{kudivmod64(absDividend, absDivisor)};
			// Quotient is negative when signs differ, remainder takes dividend sign
			const auto quotient	{((dividend < 0) != (divisor < 0)) ? (0_u64 - res.quotient) : res.quotient};
			const auto remainder	{(dividend < 0) ? (0_u64 - res.remainder) : res.remainder};
			return divmod_t<T> {
				static_cast<T>(quotient),
				static_cast<T>(remainder)
			};
		}

	}


}	// namespace igros::klib

