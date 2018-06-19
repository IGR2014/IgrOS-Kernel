////////////////////////////////////////////////////////////////
//
//	Keyboard generic handling
//
//	File:	keyboard.hpp
//	Date:	19 Jun. 2018
//
//	Copyright (c) 2018, Igor Baklykov
//	All rights reserved.
//


#ifndef IGROS_ARCH_KEYBOARD_HPP
#define IGROS_ARCH_KEYBOARD_HPP


#include <include/types.hpp>


// Arch-dependent code zone
namespace arch {


	// Forward declaration
	struct taskRegs;

	// Keyboard interrupt (#1) handler
	void keyboardInterruptHandler(const taskRegs*);


}	// namespace arch


#endif	// IGROS_ARCH_KEYBOARD_HPP

