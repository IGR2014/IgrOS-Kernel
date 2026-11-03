////////////////////////////////////////////////////////////////
///
///	@file
///	@brief		Keyboard generic handling
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


	// Set keyboard LEDs
	//void	keyboardSetLED();

	/// @brief Setup keyboard function
	/// @return true if keyboard is set up
	[[nodiscard]]
	auto keyboardSetup() noexcept -> bool;


} // namespace igros::arch

