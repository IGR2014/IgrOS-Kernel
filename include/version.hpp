////////////////////////////////////////////////////////////////
//
//	IgrOS version info
//
//	File:	version.hpp
//	Date:	16 Jul 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <arch/types.hpp>


// OS namespace
namespace igros {


	constexpr auto	IGROS_VERSION_MAJOR	= 0u;
	constexpr auto	IGROS_VERSION_MINOR	= 1u;
	constexpr auto	IGROS_VERSION_BUILD	= 5u;

	constexpr auto	IGROS_VERSION_NAME	= u8"BETA";


	// Kernel version integer
	constexpr dword_t IGROS_KERNEL_VERSION(const byte_t major, const byte_t minor, const dword_t build) noexcept {
		return (build | (static_cast<dword_t>(minor) << 16u) | (static_cast<dword_t>(major) << 24u));
	}

	// Kernel current version integer
	constexpr dword_t IGROS_KERNEL_VERSION_CURRENT() noexcept {
		return IGROS_KERNEL_VERSION(IGROS_VERSION_MAJOR, IGROS_VERSION_MINOR, IGROS_VERSION_BUILD);
	}


}	// namespace igros

