////////////////////////////////////////////////////////////////
///
///	@file
///	@brief		Kernel string functions
///	@date		02 Nov 2026
///
///	@copyright	Copyright (c) 2017 - 2026, Igor Baklykov
///			All rights reserved.
///	@author		Igor Baklykov
///


#pragma once


// C++
#include <string_view>


// Kernel library code zone
namespace igros::klib {


	/// @brief View of null-terminated string (null pointer is empty string)
	/// @param src Null-terminated string or nullptr
	/// @return View of string (empty for nullptr)
	[[nodiscard]]
	constexpr auto kstrview(const char* const src) noexcept -> std::string_view {
		return (nullptr != src) ? std::string_view {src} : std::string_view {};
	}


}	// namespace igros::klib

