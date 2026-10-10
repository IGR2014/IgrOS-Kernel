////////////////////////////////////////////////////////////////
//
//	Kernel string functions
//
//	File:	kstring.hpp
//	Date:	16 Dec 2022
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// C++
#include <concepts>
#include <type_traits>
// IgrOS-Kernel arch
#include <arch/types.hpp>


// Kernel library code zone
namespace igros::klib {


	// Character type (optionally const, so const-ness of result follows argument)
	template<class C>
	concept kchar = std::same_as<std::remove_const_t<C>, char>;


	// Find string end
	template<kchar C>
	[[nodiscard]]
	constexpr auto kstrend(C* src) noexcept -> C* {
		// Check src pointer
		if (nullptr == src) [[unlikely]] {
			return nullptr;
		}
		// Find null terminator
		for (; '\0' != *src; ++src);
		// Return pointer to null terminator
		return src;
	}

	// Get string length
	[[nodiscard]]
	constexpr auto kstrlen(const char* const src) noexcept -> igros_usize_t {
		// Check src pointer
		if (nullptr == src) [[unlikely]] {
			return 0_usize;
		}
		// Distance to string end
		return static_cast<igros_usize_t>(kstrend(src) - src);
	}


	// Copy at most size characters of src to dst and null-terminate it
	// (dst must have space for size + 1 characters)
	[[maybe_unused]]
	constexpr auto kstrcpy(char* const dst, const char* const src, const igros_usize_t size) noexcept -> char* {
		// Check pointers
		if ((nullptr == dst) || (nullptr == src)) [[unlikely]] {
			return dst;
		}
		// Copy characters
		auto i {0_usize};
		for (; (i < size) && ('\0' != src[i]); ++i) {
			dst[i] = src[i];
		}
		// Terminate copied string
		dst[i] = '\0';
		// Return destination
		return dst;
	}

	// Append src to null-terminated dst, which buffer holds size characters
	// (result is truncated to fit and always null-terminated)
	[[maybe_unused]]
	constexpr auto kstrcat(char* const dst, const char* const src, const igros_usize_t size) noexcept -> char* {
		// Check pointers
		if ((nullptr == dst) || (nullptr == src)) [[unlikely]] {
			return dst;
		}
		// Find dst end inside buffer
		auto end {0_usize};
		for (; (end < size) && ('\0' != dst[end]); ++end);
		// No null terminator (or no space) - nothing could be appended
		if (end >= size) [[unlikely]] {
			return dst;
		}
		// Append what fits (keeping space for terminator)
		kstrcpy(dst + end, src, size - end - 1_usize);
		// Return destination
		return dst;
	}


	// Compare at most size characters of two strings
	// (null pointer is less than any string)
	[[nodiscard]]
	constexpr auto kstrcmp(const char* src1, const char* src2, igros_usize_t size) noexcept -> igros_sdword_t {
		// Null pointers ordering
		if ((nullptr == src1) || (nullptr == src2)) [[unlikely]] {
			return (src1 == src2) ? 0_i32 : ((nullptr == src1) ? -1_i32 : 1_i32);
		}
		// Find first difference
		for (; size > 0_usize; --size, ++src1, ++src2) {
			if ((*src1 != *src2) || ('\0' == *src1)) {
				return static_cast<igros_sdword_t>(static_cast<igros_byte_t>(*src1)) - static_cast<igros_sdword_t>(static_cast<igros_byte_t>(*src2));
			}
		}
		// Equal within size
		return 0_i32;
	}


	// Find first occurrence of chr within size characters of string
	template<kchar C>
	[[nodiscard]]
	constexpr auto kstrchr(C* src, const char chr, igros_usize_t size) noexcept -> C* {
		// Check src pointer
		if (nullptr == src) [[unlikely]] {
			return nullptr;
		}
		// Find symbol inside string
		for (; size > 0_usize; --size, ++src) {
			if (chr == *src) {
				return src;
			}
			if ('\0' == *src) {
				break;
			}
		}
		// Not found
		return nullptr;
	}


	// Reverse at most size first characters of string
	[[maybe_unused]]
	constexpr auto kstrinv(char* const src, const igros_usize_t size) noexcept -> char* {
		// Check src pointer
		if (nullptr == src) [[unlikely]] {
			return nullptr;
		}
		// Length of the reversed part
		const auto strLen	{kstrlen(src)};
		const auto len		{(strLen < size) ? strLen : size};
		// Swap symbols from both ends
		for (auto i {0_usize}; i < (len >> 1); ++i) {
			const auto symbol	{src[i]};
			src[i]			= src[len - 1_usize - i];
			src[len - 1_usize - i]	= symbol;
		}
		// Return string address
		return src;
	}


}	// namespace igros::klib

