////////////////////////////////////////////////////////////////
///
///	@file
///	@brief		Programmable interrupt timer
///	@date		03 Nov 2026
///
///	@copyright	Copyright (c) 2017 - 2022, Igor Baklykov
///			All rights reserved.
///	@author		Igor Baklykov
///


#pragma once


// IgrOS-Kernel arch
#include <arch/types.hpp>


// Arch-dependent code zone
namespace igros::arch {


	/// @brief PIT frequency (1.193181(3) MHz)
	constexpr auto PIT_MAIN_FREQUENCY = 1193181_u32;
	/// @brief Default pit frequency (100 Hz)
	constexpr auto PIT_DEFAULT_FREQUENCY = 100_u32;


	/// @brief Setup PIT frequency
	/// @param frequency Interrupt frequency in Hz
	void pitSetupFrequency(const igros_word_t frequency) noexcept;

	/// @brief Get expired ticks
	/// @return Timer interrupts since setup
	[[nodiscard]]
	auto pitGetTicks() noexcept -> igros_quad_t;

	/// @brief Setup programmable interrupt timer
	/// @return true if timer is set up
	[[nodiscard]]
	auto pitSetup() noexcept -> bool;


} // namespace igros::arch

