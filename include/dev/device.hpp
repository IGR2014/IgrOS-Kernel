////////////////////////////////////////////////////////////////
//
//	Device operations description
//
//	File:	device.hpp
//	Date:	22 Jan 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#pragma once
#ifndef IGROS_DEV_DEVICE_HPP
#define IGROS_DEV_DEVICE_HPP


#include <type_traits>
#include <cstdint>

#include <arch/types.hpp>


// System code zone
namespace sys {


	// Device init function pointer
	using devFuncInit_t	= std::add_pointer_t<pointer_t()>;
	// Device deinit function pointer
	using devFuncDeinit_t	= std::add_pointer_t<void(pointer_t)>;

	// Device open function pointer
	using devFuncOpen_t	= std::add_pointer_t<pointer_t(const char*, const dword_t)>;
	// Device close function pointer
	using devFuncClose_t	= std::add_pointer_t<void(pointer_t)>;

	// Device write function pointer
	using devFuncWrite_t	= std::add_pointer_t<std::size_t(const pointer_t, const void* const, const std::size_t)>;
	// Device read function pointer
	using devFuncRead_t	= std::add_pointer_t<std::size_t(const pointer_t, void* const, const std::size_t)>;

	// Device IOCTL function pointer
	using devFuncIOCTL_t	= std::add_pointer_t<std::size_t(const pointer_t, const dword_t, ...)>


	// Device description structure
	struct device {

		const char		name[64];	// Get device name

		devFuncInit_t		init;		// Init device
		devFuncDeinit_t		deinit;		// Deinit device

		devFuncOpen_t		open;		// Open device
		devFuncClose_t		close;		// Close device

		devFuncWrite_t		write;		// Write to device
		devFuncRead_t		read;		// Read from device

		devFuncIOCTL_t		ioctl;		// Device IOCTL


	};


	// Register device
	bool registerDevice(const device &dev) noexcept;
	// Unregister device
	bool unregisterDevice(const device &dev) noexcept;


}	// namespace sys


#endif	// IGROS_DEV_DEVICE_HPP

