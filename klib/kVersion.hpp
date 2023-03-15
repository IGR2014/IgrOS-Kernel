////////////////////////////////////////////////////////////////
///
///	@brief		IgrOS version info
///
///	@file		kVersion.hpp
///	@date		16 Mar 2023
///
///	@copyright	Copyright (c) 2017 - 2023,
///			All rights reserved.
///	@author		Igor Baklykov
///
///


#pragma once


// IgrOS-Kernel arch
#include <arch/types.hpp>



////////////////////////////////////////////////////////////////
///
/// @brief IgrOS Kernel Library namespace
/// @namespace igros::klib
///
namespace igros::klib {


	////////////////////////////////////////////////////////////////
	///
	/// @brief Kernel version class
	/// @class kVersion
	///
	class kVersion final {

		constexpr static auto VERSION_MAJOR	{static_cast<igros_byte_t>(IGROS_GIT_VERSION_MAJOR)};		///< Kernel version major
		constexpr static auto VERSION_MINOR	{static_cast<igros_byte_t>(IGROS_GIT_VERSION_MINOR)};		///< Kernel version minor
		constexpr static auto VERSION_PATCH	{static_cast<igros_word_t>(IGROS_GIT_VERSION_PATCH)};		///< Kernel version patch
		constexpr static auto VERSION_BUILD	{static_cast<igros_dword_t>(IGROS_GIT_VERSION_BUILD)};		///< Kernel version build

		constexpr static auto VERSION_NAME	{"BETA"};							///< Kernel version name


	public:

		/// @brief Kernel version to integer
		[[nodiscard]]
		constexpr static auto value() noexcept -> igros_quad_t;

		/// @brief Kernel version to string
		[[nodiscard]]
		constexpr static auto text() noexcept -> const char*;


	};


	////////////////////////////////////////////////////////////////
	///
	/// @brief Kernel version to integer
	/// @return Version integer in format 0xXXYYZZZZWWWWWWWW
	/// @see @c igros_quad_t
	///
	[[nodiscard]]
	constexpr auto kVersion::value() noexcept -> igros_quad_t {
		// Calculates as (VERSION_MAJOR << 56) | (VERSION_MINOR << 48) | (VERSION_PATCH << 32) | VERSION_BUILD
		return	static_cast<igros_quad_t>(VERSION_BUILD) | (
				(static_cast<igros_quad_t>(VERSION_PATCH) <<  0_usize) |
				(static_cast<igros_quad_t>(VERSION_MINOR) << 16_usize) |
				(static_cast<igros_quad_t>(VERSION_MAJOR) << 24_usize)
			) << 32_usize;
	}


	////////////////////////////////////////////////////////////////
	///
	/// @brief Kernel version to string
	/// @return Version c-string in format vX.Y.ZZ.WWWW
	///
	[[nodiscard]]
	constexpr auto kVersion::text() noexcept -> const char* {
		return "v" IGROS_GIT_VERSION;	// " [ " VERSION_NAME " ]";
	}


}	// namespace igros::klib

