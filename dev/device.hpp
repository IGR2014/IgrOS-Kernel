////////////////////////////////////////////////////////////////
///
///	@file
///	@brief		Device registry
///	@date		03 Nov 2026
///
///	@copyright	Copyright (c) 2017 - 2026, Igor Baklykov
///			All rights reserved.
///	@author		Igor Baklykov
///


#pragma once


// C++
#include <string_view>
// IgrOS-Kernel arch
#include <arch/types.hpp>
// IgrOS-Kernel drivers
#include <drivers/driver.hpp>


// System code zone
namespace igros::sys {


	/// @brief Registered devices limit
	constexpr auto DEVICES_MAX {16_usize};


	/// @brief Register device (driver description is copied, devices are looked up by name)
	/// @param driver Initialized driver instance
	/// @return false if driver has no name, name is taken or registry is full
	[[nodiscard]]
	auto registerDevice(const drivers::driver_t &driver) noexcept -> bool;
	/// @brief Unregister device
	/// @param name Device name
	/// @return true if device was registered
	[[nodiscard]]
	auto unregisterDevice(const std::string_view name) noexcept -> bool;

	/// @brief Find device
	/// @param name Device name
	/// @return Device driver or nullptr if not registered
	[[nodiscard]]
	auto findDevice(const std::string_view name) noexcept -> const drivers::driver_t*;
	/// @brief Registered devices count
	/// @return Number of registered devices
	[[nodiscard]]
	auto devicesCount() noexcept -> igros_usize_t;


} // namespace igros::sys
