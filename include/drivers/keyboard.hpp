////////////////////////////////////////////////////////////////
//
//	Keyboard generic handling
//
//	File:	keyboard.hpp
//	Date:	30 Jun 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <types.hpp>


// Arch-dependent code zone
namespace igros::arch {


	// Forward declaration
	struct	taskRegs_t;

	// Set keyboard LEDs
	//void	keyboardSetLED();

	// Keyboard interrupt (#1) handler
	void	keyboardInterruptHandler(const taskRegs_t* regs) noexcept;

	// Setip keyboard function
	void	keyboardSetup() noexcept;


}	// namespace igros::arch

