////////////////////////////////////////////////////////////////
//
//	Programmable interrupt timer
//
//	File:	pit.hpp
//	Date:	03 Aug. 2018
//
//	Copyright (c) 2018, Igor Baklykov
//	All rights reserved.
//


#ifndef IGROS_ARCH_PIT_HPP
#define IGROS_ARCH_PIT_HPP


#include <include/types.hpp>


// Arch-dependent code zone
namespace arch {


	// Forward declaration
	struct	taskRegs_t;


	// PIT frequency (1.193180 MHz)
	static const dword_t	PIT_FREQUENCY	= 1193182;

	// Ticks count
	static dword_t		PIT_TICKS	= 0;

	// Setup PIT frequency
	void	pitSetupFrequency(word_t freqency);

	// PIT interrupt (#0) handler
	void	pitInterruptHandler(const taskRegs_t*);

	// Convert expired ticks count to seconds
	//void	pitToSeconds();


}	// namespace arch


#endif	// IGROS_ARCH_PIT_HPP

