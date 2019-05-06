////////////////////////////////////////////////////////////////
//
//	Keyboard generic handling
//
//	File:	keyboard.hpp
//	Date:	06 May 2019
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
//	All rights reserved.
//
//


#ifndef IGROS_ARCH_KEYBOARD_HPP
#define IGROS_ARCH_KEYBOARD_HPP


#include <include/types.hpp>


// Arch-dependent code zone
namespace arch {


	// Forward declaration
	struct	taskRegs_t;

	// Set keyboard LEDs
	//void	keyboardSetLED();

	// Keyboard interrupt (#1) handler
	void	keyboardInterruptHandler(const taskRegs_t*);

	// Setip keyboard function
	void	keyboardSetup();


}	// namespace arch


#endif	// IGROS_ARCH_KEYBOARD_HPP

