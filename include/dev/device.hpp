////////////////////////////////////////////////////////////////
//
//	Device operations description
//
//	File:	device.hpp
//	Date:	08 Dec 2022
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <type_traits>
#include <cstdint>

#include <arch/types.hpp>


// System code zone
namespace igros::sys {


	// Device init function pointer
	using devFuncInit_t	= std::add_pointer_t<auto () -> igros_pointer_t>;
	// Device deinit function pointer
	using devFuncDeinit_t	= std::add_pointer_t<void (igros_pointer_t)>;

	// Device open function pointer
	using devFuncOpen_t	= std::add_pointer_t<auto (const char*, const igros_dword_t) -> igros_pointer_t>;
	// Device close function pointer
	using devFuncClose_t	= std::add_pointer_t<void (igros_pointer_t)>;

	// Device write function pointer
	using devFuncWrite_t	= std::add_pointer_t<auto (const igros_pointer_t, const void* const, const igros_usize_t) -> igros_usize_t>;
	// Device read function pointer
	using devFuncRead_t	= std::add_pointer_t<auto (const igros_pointer_t, void* const, const igros_usize_t) -> igros_usize_t>;

	// Device IOCTL function pointer
	using devFuncIOCTL_t	= std::add_pointer_t<auto (const igros_pointer_t, const igros_dword_t, ...) -> igros_usize_t>


	// Device description structure
	struct device_t {

		const char		name[64_usize];		// Get device name

		devFuncInit_t		init;			// Init device
		devFuncDeinit_t		deinit;			// Deinit device

		devFuncOpen_t		open;			// Open device
		devFuncClose_t		close;			// Close device

		devFuncWrite_t		write;			// Write to device
		devFuncRead_t		read;			// Read from device

		devFuncIOCTL_t		ioctl;			// Device IOCTL

	};


	// Register device
	[[nodicard]]
	auto	registerDevice(const device_t &dev) noexcept -> bool;
	// Unregister device
	[[nodicard]]
	auto	unregisterDevice(const device_t &dev) noexcept -> bool;


}	// namespace igros::sys

