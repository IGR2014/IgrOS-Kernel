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


// C++
#include <algorithm>
#include <array>
#include <string_view>
// IgrOS-Kernel dev
#include <dev/device.hpp>
// IgrOS-Kernel library
#include <klib/kString.hpp>


// System code zone
namespace igros::sys {


	/// @brief Registered devices (slot without name is free)
	constinit static auto devices {std::array<drivers::driver_t, DEVICES_MAX> {}};


	/// @brief Find registered device slot
	/// @param name Device name
	/// @return Slot or nullptr if not registered
	[[nodiscard]]
	static auto deviceSlot(const std::string_view name) noexcept -> drivers::driver_t* {
		const auto named = [name](const drivers::driver_t &device) noexcept {
			return (nullptr != device.name) && (name == klib::kstrview(device.name));
		};
		auto* const slot {std::ranges::find_if(devices, named)};
		return (devices.end() != slot) ? slot : nullptr;
	}


	// Register device
	[[nodiscard]]
	auto registerDevice(const drivers::driver_t &driver) noexcept -> bool {
		// Named and not registered yet
		if ((nullptr == driver.name) || (nullptr != deviceSlot(klib::kstrview(driver.name)))) [[unlikely]] {
			return false;
		}
		// Take free slot
		auto* const slot {std::ranges::find(devices, nullptr, &drivers::driver_t::name)};
		if (devices.end() == slot) [[unlikely]] {
			return false;
		}
		*slot = driver;
		return true;
	}

	// Unregister device
	[[nodiscard]]
	auto unregisterDevice(const std::string_view name) noexcept -> bool {
		auto* const slot {deviceSlot(name)};
		if (nullptr == slot) {
			return false;
		}
		// Free slot
		*slot = drivers::driver_t {};
		return true;
	}


	// Find device
	[[nodiscard]]
	auto findDevice(const std::string_view name) noexcept -> const drivers::driver_t* {
		return deviceSlot(name);
	}

	// Registered devices count
	[[nodiscard]]
	auto devicesCount() noexcept -> igros_usize_t {
		return static_cast<igros_usize_t>(std::ranges::count_if(devices, [](const drivers::driver_t &device) noexcept {
			return nullptr != device.name;
		}));
	}


} // namespace igros::sys
