////////////////////////////////////////////////////////////////
//
//	IgrOS version info
//
//	File:	version.hpp
//	Date:	12 Feb 2021
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// IgrOS
#include <arch/types.hpp>


// OS namespace
namespace igros {


	// Kernel version constants
	constexpr auto VERSION_MAJOR	= static_cast<byte_t>(0U);
	constexpr auto VERSION_MINOR	= static_cast<byte_t>(1U);
	constexpr auto VERSION_BUILD	= static_cast<word_t>(34U);

	// Kernel version name
	constexpr auto VERSION_NAME	= u8"BETA";


	// Kernel version integer
	[[nodiscard]]
	constexpr auto KERNEL_VERSION(const byte_t major, const byte_t minor, const word_t build) noexcept {
		return (static_cast<dword_t>(build) | (static_cast<dword_t>(minor) << 16) | (static_cast<dword_t>(major) << 24));
	}

	// Kernel current version integer
	[[nodiscard]]
	constexpr auto KERNEL_VERSION() noexcept {
		return KERNEL_VERSION(VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD);
	}

	// Kernel version to string
	[[nodiscard]]
	constexpr auto KERNEL_VERSION_STRING() noexcept {
		return u8"v0.1.34 [ BETA ]";
	}


}	// namespace igros

