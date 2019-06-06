////////////////////////////////////////////////////////////////
//
//	Programmable interrupt timer
//
//	File:	pit.hpp
//	Date:	06 Jun 2019
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
//	All rights reserved.
//
//


#pragma once
#ifndef IGROS_ARCH_PIT_HPP
#define IGROS_ARCH_PIT_HPP


#include <arch/types.hpp>


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

#pragma pack(pop)


	// PIT frequency (1.193181(3) MHz)
	static const dword_t	PIT_MAIN_FREQUENCY	= 1193181;


	// Setup PIT frequency
	void	pitSetupFrequency(word_t frequency);

	// PIT interrupt (#0) handler
	void	pitInterruptHandler(const taskRegs_t*);

	// Get expired ticks
	quad_t	pitGetTicks();

	// Setup programmable interrupt timer
	void	pitSetup();


}	// namespace arch


#endif	// IGROS_ARCH_PIT_HPP

