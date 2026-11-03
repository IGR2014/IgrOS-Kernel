////////////////////////////////////////////////////////////////
///
///	@file
///	@brief		Kernel driver interface
///	@date		03 Nov 2026
///
///	@copyright	Copyright (c) 2017 - 2026, Igor Baklykov
///			All rights reserved.
///	@author		Igor Baklykov
///


#pragma once


// C++
#include <array>
#include <type_traits>
// IgrOS-Kernel arch
#include <arch/types.hpp>
// IgrOS-Kernel library
#include <klib/kPrint.hpp>


// Kernel drivers
namespace igros::drivers {


	/// @brief Driver init function (returns true on success)
	using driverInit_t = std::add_pointer_t<auto() noexcept -> bool>;


	/// @brief Kernel driver description
	struct driver_t {

		const char*  name; ///< Driver name
		driverInit_t init; ///< Driver init function
	};


	/// @brief Initialize drivers in order and print status of each, returns failed drivers count
	/// @tparam N Number of drivers
	/// @param drivers Drivers in init order
	/// @return Number of drivers that failed to initialize
	template<igros_usize_t N>
	auto init(const std::array<drivers::driver_t, N> &drivers) noexcept -> igros_usize_t {
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


} // namespace igros::drivers

