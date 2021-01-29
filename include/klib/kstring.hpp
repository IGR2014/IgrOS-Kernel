////////////////////////////////////////////////////////////////
//
//	Kernel string functions
//
//	File:	kstring.hpp
//	Date:	15 Jan 2021
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <cstdint>

#include <arch/types.hpp>


// Kernel library code zone
namespace igros::klib {


	// Find string end
	[[nodiscard]]
	sbyte_t*	kstrend(sbyte_t* src) noexcept;
	// Find string end
	[[nodiscard]]
	const sbyte_t*	kstrend(const sbyte_t* src) noexcept;

	// Calculate string length
	[[nodiscard]]
	std::size_t	kstrlen(const sbyte_t* src) noexcept;

	// Copy string from one to other
	[[maybe_unused]]
	sbyte_t*	kstrcpy(sbyte_t* src, sbyte_t* dst, std::size_t size) noexcept;
	// Copy string from one to other (const version)
	[[maybe_unused]]
	const sbyte_t*	kstrcpy(const sbyte_t* src, sbyte_t* dst, std::size_t size) noexcept;

	// Compare strings
	[[nodiscard]]
	sdword_t	kstrcmp(const sbyte_t* src1, const sbyte_t* src2, std::size_t size) noexcept;

	// Concatenate string
	[[maybe_unused]]
	sbyte_t*	kstrcat(const sbyte_t* src, sbyte_t* dst, std::size_t size) noexcept;

	// Find char occurrence in string
	[[nodiscard]]
	sbyte_t*	kstrchr(sbyte_t* src, sbyte_t chr, std::size_t size) noexcept;
	// Find char occurrence in string
	[[nodiscard]]
	const sbyte_t*	kstrchr(const sbyte_t* src, sbyte_t chr, std::size_t size) noexcept;

	// Invert string
	[[maybe_unused]]
	sbyte_t*	kstrinv(sbyte_t* src, std::size_t size) noexcept;


}	// namespace igros::arch

