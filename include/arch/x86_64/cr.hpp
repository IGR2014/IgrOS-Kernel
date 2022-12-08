////////////////////////////////////////////////////////////////
//
//	CR0 - CR4 registers operations
//
//	File:	cr.hpp
//	Date:	09 Dec 2021
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <arch/types.hpp>


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus


	// Read CR0 register
	[[nodiscard]]
	auto	outCR0() noexcept -> igros::igros_quad_t;
	// Read CR2 register
	[[nodiscard]]
	auto	outCR2() noexcept -> igros::igros_quad_t;
	// Read CR3 register
	[[nodiscard]]
	auto	outCR3() noexcept -> igros::igros_quad_t;
	// Read CR4 register
	[[nodiscard]]
	auto	outCR4() noexcept -> igros::igros_quad_t;

	// Write CR0 register
	void	inCR0(const igros::igros_quad_t value) noexcept;
	// Write CR3 register
	void	inCR3(const igros::igros_quad_t value) noexcept;
	// Write CR4 register
	void	inCR4(const igros::igros_quad_t value) noexcept;


#ifdef	__cplusplus

}	// extern "C"

#endif	// __cplusplus

