////////////////////////////////////////////////////////////////
//
//	Programmable interrupt timer
//
//	File:	pit.hpp
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


#pragma pack(push, 1)

	// Fixed-point pit value
	union pitInterval_t {

		igros_quad_t		fixed;

		struct {

			igros_dword_t	fraction;
			igros_dword_t	integer;

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
	constexpr auto PIT_MAIN_FREQUENCY	= 1193181_u32;
	// Default pit frequency (100 Hz)
	constexpr auto PIT_DEFAULT_FREQUENCY	= 100_u32;


	// Setup PIT frequency
	void	pitSetupFrequency(const igros_word_t frequency) noexcept;

	// Get expired ticks
	[[nodiscard]]
	auto	pitGetTicks() noexcept -> igros_quad_t;

	// Setup programmable interrupt timer
	void	pitSetup() noexcept;


}	// namespace igros::arch

