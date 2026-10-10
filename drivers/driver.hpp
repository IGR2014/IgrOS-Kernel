////////////////////////////////////////////////////////////////
//
//	Kernel driver interface
//
//	File:	driver.hpp
//	Date:	11 Oct 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// C++
#include <type_traits>
// IgrOS-Kernel arch
#include <arch/types.hpp>
// IgrOS-Kernel library
#include <klib/kprint.hpp>


// Kernel drivers
namespace igros::drivers {


	// Driver init function (returns true on success)
	using driverInit_t	= std::add_pointer_t<auto () noexcept -> bool>;


	// Kernel driver description
	struct driver_t {

		const char*	name;		// Driver name
		driverInit_t	init;		// Driver init function

	};


	// Initialize drivers in order and print status of each, returns failed drivers count
	template<igros_usize_t N>
	auto init(const driver_t (&drivers)[N]) noexcept -> igros_usize_t {
		// Failed drivers count
		auto failed {0_usize};
		// Init drivers one by one (failed driver doesn't stop the rest)
		for (const auto &driver : drivers) {
			const auto ok {driver.init()};
			klib::kprintf(
				"Driver %s:\t%s",
				driver.name,
				ok ? "OK" : "ERROR"
			);
			failed += ok ? 0_usize : 1_usize;
		}
		// Failed drivers count
		return failed;
	}


}	// namespace igros::drivers

