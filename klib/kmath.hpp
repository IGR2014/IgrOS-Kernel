////////////////////////////////////////////////////////////////
//
//	Kernel math functions definitions
//
//	File:	kmath.hpp
//	Date:	16 Dec 2022
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// C++
#include <cstdint>
// IgrOS-Kernel arch
#include <arch/types.hpp>


// Kernel library code zone
namespace igros::klib {


	// Combined division & modulo result (unsigned)
	struct udivmod_t {
		igros_quad_t quotient;
		igros_quad_t reminder;
	};

	// Combined division & modulo result (signed)
	struct divmod_t {
		igros_squad_t quotient;
		igros_squad_t reminder;
	};


	// Divide 64-bit integer by 32-bit integer
	// Returns 64-bit quotient and 64-bit reminder
	[[nodiscard]]
	auto	kudivmod(igros_quad_t dividend, igros_dword_t divisor) noexcept -> udivmod_t;
	// Divide 64-bit integer by 32-bit integer
	// Returns 64-bit quotient and 64-bit reminder
	[[nodiscard]]
	auto	kdivmod(igros_squad_t dividend, igros_sdword_t divisor) noexcept -> divmod_t;


}	// namespace igros::klib

