////////////////////////////////////////////////////////////////
//
//	Keyboard generic handling
//
//	File:	keyboard.hpp
//	Date:	12 Feb 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#pragma once
#ifndef IGROS_ARCH_KEYBOARD_HPP
#define IGROS_ARCH_KEYBOARD_HPP


#include <types.hpp>


// Arch-dependent code zone
namespace arch {


	// Forward declaration
	struct	taskRegs_t;

	// Set keyboard LEDs
	//void	keyboardSetLED();

	// Keyboard interrupt (#1) handler
	void	keyboardInterruptHandler(const taskRegs_t* regs) noexcept;

	// Setip keyboard function
	void	keyboardSetup() noexcept;


}	// namespace arch


#endif	// IGROS_ARCH_KEYBOARD_HPP

