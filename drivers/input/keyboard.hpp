////////////////////////////////////////////////////////////////
//
//	Keyboard generic handling
//
//	File:	keyboard.hpp
//	Date:	16 Dec 2022
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// IgrOS-Kernel arch
#include <arch/types.hpp>


// Arch-dependent code zone
namespace igros::arch {


	// Set keyboard LEDs
	//void	keyboardSetLED();

	// Setup keyboard function
	[[nodiscard]]
	auto	keyboardSetup() noexcept -> bool;


}	// namespace igros::arch

