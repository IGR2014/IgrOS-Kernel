////////////////////////////////////////////////////////////////
//
//	IgrOS version info
//
//	File:	version.hpp
//	Date:	17 Jul 2020
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <arch/types.hpp>


// OS namespace
namespace igros {


	// Kernel version constants
	constexpr auto VERSION_MAJOR	= static_cast<byte_t>(0u);
	constexpr auto VERSION_MINOR	= static_cast<byte_t>(1u);
	constexpr auto VERSION_BUILD	= static_cast<word_t>(34u);

	// Kernel version name
	constexpr auto VERSION_NAME	= u8"BETA";


	// Kernel version integer
	constexpr auto KERNEL_VERSION(const byte_t major, const byte_t minor, const dword_t build) noexcept {
		return (build | (static_cast<dword_t>(minor) << 16) | (static_cast<dword_t>(major) << 24));
	}

	// Kernel current version integer
	constexpr auto KERNEL_VERSION() noexcept {
		return KERNEL_VERSION(VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD);
	}

	// Kernel version to string
	constexpr auto KERNEL_VERSION_STRING() noexcept {
		return u8"v0.1.34 [ BETA ]";
	}


}	// namespace igros

