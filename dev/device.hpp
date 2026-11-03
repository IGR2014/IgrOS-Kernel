////////////////////////////////////////////////////////////////
///
///	@file
///	@brief		Device operations description
///	@date		03 Nov 2026
///
///	@copyright	Copyright (c) 2017 - 2022, Igor Baklykov
///			All rights reserved.
///	@author		Igor Baklykov
///


#pragma once


// C++
#include <array>
#include <cstdint>
#include <type_traits>
// IgrOS-Kernel arch
#include <arch/types.hpp>


// System code zone
namespace igros::sys {


	/// @brief Device init function pointer
	using devFuncInit_t = std::add_pointer_t<auto()->igros_pointer_t>;
	/// @brief Device deinit function pointer
	using devFuncDeinit_t = std::add_pointer_t<void(igros_pointer_t)>;

	/// @brief Device open function pointer
	using devFuncOpen_t = std::add_pointer_t<auto(const char*, const igros_dword_t)->igros_pointer_t>;
	/// @brief Device close function pointer
	using devFuncClose_t = std::add_pointer_t<void(igros_pointer_t)>;

	/// @brief Device write function pointer
	using devFuncWrite_t = std::add_pointer_t<auto(const igros_pointer_t, const void* const, const igros_usize_t)->igros_usize_t>;
	/// @brief Device read function pointer
	using devFuncRead_t = std::add_pointer_t<auto(const igros_pointer_t, void* const, const igros_usize_t)->igros_usize_t>;

	/// @brief Device IOCTL function pointer
	using devFuncIOCTL_t = std::add_pointer_t<auto(const igros_pointer_t, const igros_dword_t, ...)->igros_usize_t>;


	/// @brief Device description structure
	struct device_t {

		std::array<const char, 64_usize> name; ///< Get device name

		devFuncInit_t	init;	///< Init device
		devFuncDeinit_t deinit; ///< Deinit device

		devFuncOpen_t  open;  ///< Open device
		devFuncClose_t close; ///< Close device

		devFuncWrite_t write; ///< Write to device
		devFuncRead_t  read;  ///< Read from device

		devFuncIOCTL_t ioctl; ///< Device IOCTL
	};


	/// @brief Register device
	/// @param dev Device
	/// @return true if device is registered
	[[nodiscard]]
	auto registerDevice(const device_t &dev) noexcept -> bool;
	/// @brief Unregister device
	/// @param dev Device
	/// @return true if device was registered
	[[nodiscard]]
	auto unregisterDevice(const device_t &dev) noexcept -> bool;


} // namespace igros::sys

