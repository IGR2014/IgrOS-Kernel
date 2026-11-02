////////////////////////////////////////////////////////////////
///
///	@file
///	@brief		CR0 - CR4 registers operations
///	@date		16 Dec 2022
///
///	@copyright	Copyright (c) 2017 - 2022, Igor Baklykov
///			All rights reserved.
///	@author		Igor Baklykov
///


#pragma once


// IgrOS-Kernel arch
#include <arch/types.hpp>


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus


	/// @brief Read CR0 register
	/// @return CR0 value
	[[nodiscard]]
	auto	readCR0() noexcept -> igros::igros_usize_t;
	/// @brief Read CR2 register
	/// @return CR2 value
	[[nodiscard]]
	auto	readCR2() noexcept -> igros::igros_usize_t;
	/// @brief Read CR3 register
	/// @return CR3 value
	[[nodiscard]]
	auto	readCR3() noexcept -> igros::igros_usize_t;
	/// @brief Read CR4 register
	/// @return CR4 value
	[[nodiscard]]
	auto	readCR4() noexcept -> igros::igros_usize_t;

	/// @brief Write CR0 register
	/// @param value New CR0 value
	void	writeCR0(const igros::igros_usize_t value) noexcept;
	/// @brief Write CR3 register
	/// @param value New CR3 value
	void	writeCR3(const igros::igros_usize_t value) noexcept;
	/// @brief Write CR4 register
	/// @param value New CR4 value
	void	writeCR4(const igros::igros_usize_t value) noexcept;

	/// @brief Invalidate TLB entry of page
	/// @param page Virtual address inside the page
	void	pageInvalidate(const void* const page) noexcept;


#ifdef	__cplusplus

}	// extern "C"

#endif	// __cplusplus

