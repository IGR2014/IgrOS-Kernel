////////////////////////////////////////////////////////////////
//
//	Kernel string functions
//
//	File:	kString.hpp
//	Date:	02 Nov 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// C++
#include <string_view>


// Kernel library code zone
namespace igros::klib {


	// View of null-terminated string (null pointer is empty string)
	[[nodiscard]]
	constexpr auto kstrview(const char* const src) noexcept -> std::string_view {
		return (nullptr != src) ? std::string_view {src} : std::string_view {};
	}


}	// namespace igros::klib

