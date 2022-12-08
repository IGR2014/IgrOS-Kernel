////////////////////////////////////////////////////////////////
//
//	Kernel string functions
//
//	File:	kstring.hpp
//	Date:	08 Dec 2022
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
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
	constexpr auto kstrend(char* src) noexcept -> char* {
		// Check src pointer
		if (nullptr == src) [[unlikely]] {
			return nullptr;
		}
                // Find string end pointer
                for (;*src != '\0'; ++src);
		// Return string end
                return src;
	}

	// Find string end
	[[nodiscard]]
	constexpr auto kstrend(const char* src) noexcept -> const char* {
		// Check src pointer
		if (nullptr == src) [[unlikely]] {
			return nullptr;
		}
		// Copy string start pointer
                auto iter {src};
                // Find string end pointer
                for (;*iter != '\0'; ++iter);
		// Return string end
                return iter;
	}


	// Calculate string length
	[[nodiscard]]
	constexpr auto kstrlen(const char* src) noexcept -> igros_usize_t {
		// Check src pointer
		if (nullptr == src) [[unlikely]] {
			return 0_usize;
		}
                // Return string length
                return kstrend(src) - src;
        }


	// Copy string from one to other
	[[maybe_unused]]
	constexpr auto kstrcpy(char* src, char* dst, igros_usize_t size) noexcept -> char* {
		// Check src, dst pointers and size
		if ((nullptr == src) || (nullptr == dst) || (0_usize == size)) [[unlikely]] {
			// Pointer to empty dst
			// Fill with null terminator for sanity
			return dst;
		}
		// Save destination pointer
		const auto tempDst {dst};
		// Copy first symbol
		*dst = *src;
		// Copy string
		do {
			// Copy string byte by byte
			*++dst = *++src;
		// Stop on size overflow or null terminator
		} while ((--size > 0_usize) && (*src != '\0'));
		// Return pointer to dst tring
		return tempDst;
	}

	// Copy string from one to other
	[[maybe_unused]]
	constexpr auto kstrcpy(const char* src, char* dst, igros_usize_t size) noexcept -> const char* {
		// Check src, dst pointers and size
		if ((nullptr == src) || (nullptr == dst) || (0_usize == size)) [[unlikely]] {
			// Pointer to empty dst
			// Fill with null terminator for sanity
			return dst;
		}
		// Save destination pointer
		const auto tempDst {dst};
		// Copy first symbol
		*dst = *src;
		// Copy string
		do {
			// Copy string byte by byte
			*++dst = *++src;
		// Stop on size overflow or null terminator
		} while ((--size > 0_usize) && (*src != '\0'));
		// Return pointer to dst tring
		return tempDst;
	}


	// Concatenate string
	[[maybe_unused]]
	constexpr auto kstrcat(const char* src, char* dst, igros_usize_t size) noexcept -> char* {
		// Check src, dst pointers and size
		if ((nullptr == src) || (nullptr == dst) || (0_usize == size)) [[unlikely]] {
			// Return nothing
			return nullptr;
		}
		// Return nothing
		return nullptr;
	}


	// Compare strings
	[[nodiscard]]
	constexpr auto kstrcmp(const char* src1, const char* src2, igros_usize_t size) noexcept -> igros_sdword_t {
		// Check src1 and src2 pointers
		if ((nullptr == src1) || (nullptr == src2) || (0_usize == size)) [[unlikely]] {
			// Handle wrong input
			if (src1 == nullptr) {
				return (src2 == nullptr) ? 0_i32 : -1_i32;
			} else {
				return 1_i32;
			}
		}
		// Compare string symbol by symbol
		for (;(--size > 0_usize) && (*src1 != '\0') && (*src1 == *src2); ++src1, ++src2);
		// Return string difference
		return static_cast<igros_byte_t>(*src1) - static_cast<igros_byte_t>(*src2);
	}


	// Find char occurrence in string
	[[nodiscard]]
	constexpr auto kstrchr(char* src, char chr, igros_usize_t size) noexcept -> char* {
		// Check src pointer and size
		if ((nullptr == src) || (0_usize == size)) [[unlikely]] {
			return nullptr;
		}
		// Find symbol inside string
		for (;(--size > 0_usize) && (*src != '\0') && (*src != chr); ++src);
		// Return address of first occurrence or null pointer
		return (*src == chr) ? src : nullptr;
	}

	// Find char occurrence in string
	[[nodiscard]]
	constexpr auto kstrchr(const char* src, char chr, igros_usize_t size) noexcept -> const char* {
		// Check src pointer and size
		if ((nullptr == src) || (0_usize == size)) [[unlikely]] {
			return nullptr;
		}
		// Find symbol inside string
		for (;(--size > 0_usize) && (*src != '\0') && (*src != chr); ++src);
		// Return address of first occurrence or null pointer
		return (*src == chr) ? src : nullptr;
	}


	// Invert string
	[[maybe_unused]]
	constexpr auto kstrinv(char* src, igros_usize_t size) noexcept -> char* {
		// Check src pointer and size
		if (nullptr == src) [[unlikely]] {
			return nullptr;
		}
		// Copy pointer to string start
		auto iter		{src};
		// Get string length
		const auto strLen	{kstrlen(src) - 1_usize};
		// Get required reverting string length
		const auto len		{(strLen < size) ? strLen : (size - 1_usize)};
		// Calculate half length of the reverting string
		const auto halfLen	{len >> 1};
		// Loop through image
		for (auto i {len}; i != halfLen; i--, iter++) {
			// Swap symbols
			std::swap(*iter, src[i]);
		}
		// Return string address
		return src;
	}


}	// namespace igros::arch

