////////////////////////////////////////////////////////////////
//
//	Programmable interrupt timer
//
//	File:	pit.hpp
//	Date:	02 Feb 2021
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <arch/types.hpp>


// Arch-dependent code zone
namespace igros::arch {


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
	constexpr auto PIT_MAIN_FREQUENCY	= 1193181u;
	// Default pit frequency (100 Hz)
	constexpr auto PIT_DEFAULT_FREQUENCY	= 100u;


	// Setup PIT frequency
	void	pitSetupFrequency(const word_t frequency) noexcept;

	// Get expired ticks
	[[nodiscard]]
	quad_t	pitGetTicks() noexcept;

	// Setup programmable interrupt timer
	void	pitSetup() noexcept;


}	// namespace igros::arch

