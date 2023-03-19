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


	// Divide unsigned integer by unsigned integer (igros_quad_t / igros_dword_t overload)
	// Returns unsigned quotient and unsigned reminder
	template<>
	[[nodiscard]]
	constexpr auto kdivmod(igros_quad_t dividend, igros_dword_t divisor) noexcept -> divmod_t<igros_quad_t> {

		// Division result
		auto res	{divmod_t<igros_quad_t> {0_u64, dividend}};
		// Quotient bit
		auto qbit	{1_u64};

		// Division by zero
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

	// Divide unsigned integer by unsigned integer (igros_quad_t / igros_quad_t overload)
	// Returns unsigned quotient and unsigned reminder
	template<>
	[[nodiscard]]
	constexpr auto kdivmod(igros_quad_t dividend, igros_quad_t divisor) noexcept -> divmod_t<igros_quad_t> {
		// Lazy trick
		return kdivmod(dividend, static_cast<igros_dword_t>(divisor));
	}


	// Divide signed integer by signed integer (igros_squad_t / igros_sdword_t overload)
	// Returns signed quotient and signed reminder
	template<>
	[[nodiscard]]
	constexpr auto kdivmod(igros_squad_t dividend, igros_sdword_t divisor) noexcept -> divmod_t<igros_squad_t> {

		// Division result
		auto res	{divmod_t<igros_squad_t> {0_i64, dividend}};
		// Quotient bit
		auto qbit	{1_i64};

		// Division by zero
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

	// Divide signed integer by signed integer (igros_squad_t / igros_squad_t overload)
	// Returns signed quotient and signed reminder
	template<>
	[[nodiscard]]
	constexpr auto kdivmod(igros_squad_t dividend, igros_squad_t divisor) noexcept -> divmod_t<igros_squad_t> {
		// Lazy trick
		return kdivmod(dividend, static_cast<igros_sdword_t>(divisor));
	}


#endif


}	// namespace igros::klib

