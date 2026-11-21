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
#include <concepts>
#include <cstddef>
#include <span>
#include <string_view>
#include <type_traits>
// IgrOS-Kernel arch
#include <arch/types.hpp>
// IgrOS-Kernel library
#include <klib/kPrint.hpp>


// Kernel drivers
namespace igros::drivers {


	/// @brief Driver instance context (device object, see make)
	using driverContext_t = void*;

	/// @brief Driver init function (returns true on success)
	using driverInit_t = std::add_pointer_t<auto(driverContext_t context) noexcept -> bool>;
	/// @brief Driver deinit function
	using driverDeinit_t = std::add_pointer_t<void(driverContext_t context) noexcept>;
	/// @brief Driver read function (returns number of bytes read into buffer)
	using driverRead_t = std::add_pointer_t<auto(driverContext_t context, const std::span<std::byte> buffer) noexcept -> igros_usize_t>;
	/// @brief Driver write function (returns number of bytes written from data)
	using driverWrite_t = std::add_pointer_t<auto(driverContext_t context, const std::span<const std::byte> data) noexcept -> igros_usize_t>;
	/// @brief Driver control function (driver-specific request and its argument, returns true on success)
	using driverIOCTL_t = std::add_pointer_t<auto(driverContext_t context, const igros_dword_t request, const igros_pointer_t arg) noexcept -> bool>;


	/// @brief Kernel driver description: one device instance (operation the driver doesn't support is nullptr)
	struct driver_t {

		const char*	name {nullptr};	   ///< Device name
		driverContext_t context {nullptr}; ///< Instance context (passed to every operation)
		driverInit_t	init {nullptr};	   ///< Initialize device
		driverDeinit_t	deinit {nullptr};  ///< Release device
		driverRead_t	read {nullptr};	   ///< Read from device
		driverWrite_t	write {nullptr};   ///< Write to device
		driverIOCTL_t	ioctl {nullptr};   ///< Device control
	};


	/// @brief Device can be initialized: "auto init() noexcept -> bool"
	template<typename T>
	concept Initializable = requires(T &device) {
		{ device.init() } noexcept -> std::same_as<bool>;
	};
	/// @brief Device can be released: "void deinit() noexcept"
	template<typename T>
	concept Releasable = requires(T &device) {
		{ device.deinit() } noexcept -> std::same_as<void>;
	};
	/// @brief Device can be read: "auto read(std::span<std::byte>) noexcept -> igros_usize_t"
	template<typename T>
	concept Readable = requires(T &device, const std::span<std::byte> buffer) {
		{ device.read(buffer) } noexcept -> std::same_as<igros_usize_t>;
	};
	/// @brief Device can be written: "auto write(std::span<const std::byte>) noexcept -> igros_usize_t"
	template<typename T>
	concept Writable = requires(T &device, const std::span<const std::byte> data) {
		{ device.write(data) } noexcept -> std::same_as<igros_usize_t>;
	};
	/// @brief Device can be controlled: "auto ioctl(igros_dword_t, igros_pointer_t) noexcept -> bool"
	template<typename T>
	concept Controllable = requires(T &device, const igros_dword_t request, const igros_pointer_t arg) {
		{ device.ioctl(request, arg) } noexcept -> std::same_as<bool>;
	};

	/// @brief Driver description for device object (operations device class doesn't have are nullptr)
	/// @tparam T Device class
	/// @param name Device name
	/// @param device Device object (must outlive driver description)
	/// @return Driver which operations call device methods
	template<typename T>
	[[nodiscard]]
	constexpr auto make(const char* const name, T &device) noexcept -> driver_t {
		auto driver {driver_t {.name = name, .context = &device}};
		if constexpr (Initializable<T>) {
			driver.init = [](driverContext_t context) noexcept -> bool {
				return static_cast<T*>(context)->init();
			};
		}
		if constexpr (Releasable<T>) {
			driver.deinit = [](driverContext_t context) noexcept {
				static_cast<T*>(context)->deinit();
			};
		}
		if constexpr (Readable<T>) {
			driver.read = [](driverContext_t context, const std::span<std::byte> buffer) noexcept -> igros_usize_t {
				return static_cast<T*>(context)->read(buffer);
			};
		}
		if constexpr (Writable<T>) {
			driver.write = [](driverContext_t context, const std::span<const std::byte> data) noexcept -> igros_usize_t {
				return static_cast<T*>(context)->write(data);
			};
		}
		if constexpr (Controllable<T>) {
			driver.ioctl = [](driverContext_t context, const igros_dword_t request, const igros_pointer_t arg) noexcept -> bool {
				return static_cast<T*>(context)->ioctl(request, arg);
			};
		}
		return driver;
	}


	/// @brief Initialize driver
	/// @param driver Driver
	/// @return true if driver is set up (driver without init has nothing to set up)
	[[nodiscard]]
	inline auto init(const driver_t &driver) noexcept -> bool {
		return (nullptr == driver.init) || driver.init(driver.context);
	}

	/// @brief Release driver
	/// @param driver Driver
	inline void deinit(const driver_t &driver) noexcept {
		if (nullptr != driver.deinit) {
			driver.deinit(driver.context);
		}
	}

	/// @brief Initialize drivers in order and print status of each, returns failed drivers count
	/// @tparam N Number of drivers
	/// @tparam F Ready callback type
	/// @param drivers Drivers in init order
	/// @param ready Called for every successfully initialized driver
	/// @return Number of drivers that failed to initialize
	template<igros_usize_t N, std::invocable<const driver_t &> F>
	auto init(const std::array<driver_t, N> &drivers, const F &ready) noexcept -> igros_usize_t {
		// Failed drivers count
		auto failed {0_usize};
		// Init drivers one by one (failed driver doesn't stop the rest)
		for (const auto &driver : drivers) {
			const auto ok {init(driver)};
			klib::kprintf(
				"Driver %s:\t%s",
				driver.name,
				ok ? "OK" : "ERROR"
			);
			if (ok) {
				ready(driver);
			} else {
				++failed;
			}
		}
		// Failed drivers count
		return failed;
	}

	/// @brief Initialize drivers in order and print status of each, returns failed drivers count
	/// @tparam N Number of drivers
	/// @param drivers Drivers in init order
	/// @return Number of drivers that failed to initialize
	template<igros_usize_t N>
	auto init(const std::array<driver_t, N> &drivers) noexcept -> igros_usize_t {
		return init(drivers, []([[maybe_unused]] const driver_t &driver) noexcept {});
	}

	/// @brief Release drivers in reverse init order
	/// @tparam N Number of drivers
	/// @param drivers Drivers in init order
	template<igros_usize_t N>
	void deinit(const std::array<driver_t, N> &drivers) noexcept {
		// Last initialized is released first (may depend on earlier ones)
		for (auto i {N}; i > 0_usize; --i) {
			deinit(drivers[i - 1_usize]);
		}
	}


	/// @brief Read from device
	/// @param driver Driver
	/// @param buffer Output buffer
	/// @return Number of bytes read (0 if driver can't read)
	[[nodiscard]]
	inline auto read(const driver_t &driver, const std::span<std::byte> buffer) noexcept -> igros_usize_t {
		return (nullptr != driver.read) ? driver.read(driver.context, buffer) : 0_usize;
	}

	/// @brief Write to device
	/// @param driver Driver
	/// @param data Data
	/// @return Number of bytes written (0 if driver can't write)
	[[maybe_unused]]
	inline auto write(const driver_t &driver, const std::span<const std::byte> data) noexcept -> igros_usize_t {
		return (nullptr != driver.write) ? driver.write(driver.context, data) : 0_usize;
	}

	/// @brief Write text to device
	/// @param driver Driver
	/// @param text Text
	/// @return Number of characters written (0 if driver can't write)
	[[maybe_unused]]
	inline auto write(const driver_t &driver, const std::string_view text) noexcept -> igros_usize_t {
		return write(driver, std::as_bytes(std::span {text}));
	}

	/// @brief Device control
	/// @tparam R Driver request enumeration
	/// @param driver Driver
	/// @param request Driver-specific request
	/// @param arg Request argument (see request description)
	/// @return true if request succeeded (false if driver has no controls)
	template<typename R>
	requires std::is_enum_v<R>
	[[maybe_unused]]
	auto ioctl(const driver_t &driver, const R request, const igros_pointer_t arg = nullptr) noexcept -> bool {
		return (nullptr != driver.ioctl) && driver.ioctl(driver.context, static_cast<igros_dword_t>(request), arg);
	}


} // namespace igros::drivers
