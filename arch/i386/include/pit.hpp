////////////////////////////////////////////////////////////////
//
//	Programmable interrupt timer
//
//	File:	pit.hpp
//	Date:	13 Aug. 2018
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


#pragma pack(push, 1)

	// Fixed-point pit value
	union pitInterval_t {

		quad_t		fixed;

		struct {

			dword_t	fraction;
			dword_t	integer;

		};


		pitInterval_t& operator+=(const pitInterval_t &interval) {

			fixed += interval.fixed;

			return *this;

		}

		pitInterval_t operator+(const pitInterval_t &interval) {

			pitInterval_t newInterwal;
			newInterwal.fixed = fixed;

			newInterwal += interval;

			return newInterwal;

		}

	};

#pragma pop


	// PIT frequency (1.193182 MHz)
	static const dword_t	PIT_MAIN_FREQUENCY	= 1193182;

	// Ticks count
	static dword_t		PIT_TICKS		= 0;

	// Current frequency
	static word_t		PIT_FREQUENCY		= 0;


	// Setup PIT frequency
	void	pitSetupFrequency(word_t frequency);

	// PIT interrupt (#0) handler
	void	pitInterruptHandler(const taskRegs_t*);

	// Get expired ticks
	dword_t	pitGetTicks();


}	// namespace arch


#endif	// IGROS_ARCH_PIT_HPP

