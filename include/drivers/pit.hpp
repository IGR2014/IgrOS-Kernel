////////////////////////////////////////////////////////////////
//
//	Programmable interrupt timer
//
//	File:	pit.hpp
//	Date:	12 Feb 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#pragma once
#ifndef IGROS_ARCH_PIT_HPP
#define IGROS_ARCH_PIT_HPP


#include <types.hpp>


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


		pitInterval_t& operator+=(const pitInterval_t &interval) noexcept {
			fixed += interval.fixed;
			return *this;
		}

		pitInterval_t operator+(const pitInterval_t &interval) noexcept {
			pitInterval_t newInterwal;
			newInterwal.fixed = fixed;
			newInterwal += interval;
			return newInterwal;
		}

	};

#pragma pack(pop)


	// PIT frequency (1.193181(3) MHz)
	constexpr static dword_t PIT_MAIN_FREQUENCY	= 1193181u;
	// Default pit frequency (100 Hz)
	constexpr static dword_t PIT_DEFAULT_FREQUENCY	= 100u;


	// Setup PIT frequency
	void	pitSetupFrequency(const word_t frequency) noexcept;

	// PIT interrupt (#0) handler
	void	pitInterruptHandler(const taskRegs_t* regs) noexcept;

	// Get expired ticks
	[[nodiscard]] quad_t	pitGetTicks() noexcept;

	// Setup programmable interrupt timer
	void	pitSetup() noexcept;


}	// namespace arch


#endif	// IGROS_ARCH_PIT_HPP

