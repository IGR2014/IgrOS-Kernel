////////////////////////////////////////////////////////////////
//
//	IgrOS version info
//
//	File:	version.hpp
//	Date:	08 Dec 2022
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// IgrOS
#include <arch/types.hpp>


// OS namespace
namespace igros {


	// Kernel version constants
	constexpr auto VERSION_MAJOR	{static_cast<igros_byte_t>(IGROS_GIT_VERSION_MAJOR)};
	constexpr auto VERSION_MINOR	{static_cast<igros_byte_t>(IGROS_GIT_VERSION_MINOR)};
	constexpr auto VERSION_PATCH	{static_cast<igros_word_t>(IGROS_GIT_VERSION_PATCH)};
	constexpr auto VERSION_BUILD	{static_cast<igros_dword_t>(IGROS_GIT_VERSION_BUILD)};

	// Kernel version name
	constexpr auto VERSION_NAME	{"BETA"};


	// Kernel version integer
	[[nodiscard]]
	constexpr auto KERNEL_VERSION(const igros_byte_t major, const igros_byte_t minor, const igros_word_t patch, const igros_dword_t build) noexcept -> igros_quad_t {
		return static_cast<igros_quad_t>(build) | ((static_cast<igros_quad_t>(patch) | (static_cast<igros_quad_t>(minor) << 16) | (static_cast<igros_quad_t>(major) << 24)) << 32);
	}

	// Kernel current version integer
	[[nodiscard]]
	constexpr auto KERNEL_VERSION() noexcept -> igros_quad_t {
		return KERNEL_VERSION(VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH, VERSION_BUILD);
	}

	// Kernel version to string
	[[nodiscard]]
	constexpr auto KERNEL_VERSION_STRING() noexcept {
		return "v" IGROS_GIT_VERSION;	// " [ " VERSION_NAME " ]";
	}


}	// namespace igros

