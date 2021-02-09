////////////////////////////////////////////////////////////////
//
//	Keyboard generic handling
//
//	File:	keyboard.hpp
//	Date:	30 Jun 2020
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <arch/types.hpp>


// Arch-dependent code zone
namespace igros::arch {


	// Set keyboard LEDs
	//void	keyboardSetLED();

	// Setip keyboard function
	void	keyboardSetup() noexcept;


}	// namespace igros::arch

