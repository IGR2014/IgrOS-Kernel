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
// IgrOS-Kernel drivers
#include <drivers/driver.hpp>


// Keyboard driver
namespace igros::arch::keyboard {


	// Set keyboard LEDs
	//void	keyboardSetLED();


	/// @brief PS/2 keyboard
	class ps2_t final {

	public:

		/// @brief Setup keyboard (IRQ handler)
		/// @return true if keyboard is set up
		[[nodiscard]]
		auto init() const noexcept -> bool;
		/// @brief Stop keyboard interrupts
		void deinit() const noexcept;
	};

	/// @brief System keyboard
	inline constinit ps2_t ps2 {};

	/// @brief System keyboard driver description
	inline constexpr drivers::driver_t DRIVER {drivers::make("Keyboard", ps2)};


} // namespace igros::arch::keyboard
