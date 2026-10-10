////////////////////////////////////////////////////////////////
//
//	CR0 - CR4 registers operations
//
//	File:	cr.hpp
//	Date:	16 Dec 2022
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// IgrOS-Kernel arch
#include <arch/types.hpp>


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus


	// Read CR0 register
	[[nodiscard]]
	auto	readCR0() noexcept -> igros::igros_usize_t;
	// Read CR2 register
	[[nodiscard]]
	auto	readCR2() noexcept -> igros::igros_usize_t;
	// Read CR3 register
	[[nodiscard]]
	auto	readCR3() noexcept -> igros::igros_usize_t;
	// Read CR4 register
	[[nodiscard]]
	auto	readCR4() noexcept -> igros::igros_usize_t;

	// Write CR0 register
	void	writeCR0(const igros::igros_usize_t value) noexcept;
	// Write CR3 register
	void	writeCR3(const igros::igros_usize_t value) noexcept;
	// Write CR4 register
	void	writeCR4(const igros::igros_usize_t value) noexcept;

	// Invalidate TLB entry of page
	void	pageInvalidate(const void* const page) noexcept;


#ifdef	__cplusplus

}	// extern "C"

#endif	// __cplusplus

