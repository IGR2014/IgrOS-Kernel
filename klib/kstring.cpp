////////////////////////////////////////////////////////////////
//
//	Kernel string functions
//
//	File:	string.cpp
//	Date:	30 Jun 2020
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#include <cstdint>

#include <klib/kstring.hpp>


// Kernel library code zone
namespace igros::klib {


	// Find string end
	[[nodiscard]] sbyte_t* kstrend(sbyte_t* src) noexcept {
		// Check src pointer
		if (nullptr == src) {
			return nullptr;
		}
                // Find string end pointer
                for (;*src != u8'\0'; ++src);
		// Return string end
                return src;
	}

	// Find string end
	[[nodiscard]] const sbyte_t* kstrend(const sbyte_t* src) noexcept {
		// Check src pointer
		if (nullptr == src) {
			return nullptr;
		}
		// Copy string start pointer
                auto iter = src;
                // Find string end pointer
                for (;*iter != u8'\0'; ++iter);
		// Return string end
                return iter;
	}


	// Calculate string length
	[[nodiscard]] std::size_t kstrlen(const sbyte_t* src) noexcept {
                // Return string length
                return kstrend(src) - src;
        }


	// Copy string from one to other
	[[maybe_unused]] sbyte_t* kstrcpy(sbyte_t* src, sbyte_t* dst, std::size_t size) noexcept {
		// Check src, dst pointers and size
		if (nullptr == src || nullptr == dst || 0u == size) {
			// Pointer to empty dst
			// Fill with null terminator for sanity
			return dst;
		}
		// Save destination pointer
		const auto tempDst = dst;
		// Copy first symbol
		*dst = *src;
		// Copy string
		do {
			// Copy string byte by byte
			*++dst = *++src;
		// Stop on size overflow or null terminator
		} while (--size > 0u && *src != u8'\0');
		// Return pointer to dst tring
		return tempDst;
	}

	// Copy string from one to other
	[[maybe_unused]] const sbyte_t* kstrcpy(const sbyte_t* src, sbyte_t* dst, std::size_t size) noexcept {
		// Check src, dst pointers and size
		if (nullptr == src || nullptr == dst || 0u == size) {
			// Pointer to empty dst
			// Fill with null terminator for sanity
			return dst;
		}
		// Save destination pointer
		const auto tempDst = dst;
		// Copy first symbol
		*dst = *src;
		// Copy string
		do {
			// Copy string byte by byte
			*++dst = *++src;
		// Stop on size overflow or null terminator
		} while (--size > 0u && *src != u8'\0');
		// Return pointer to dst tring
		return tempDst;
	}


	// Concatenate string
	[[maybe_unused]] sbyte_t* kstrcat(const sbyte_t* src, sbyte_t* dst, std::size_t size) noexcept {
		// Check src, dst pointers and size
		if (nullptr == src || nullptr == dst || 0u == size) {
			// Return nothing
			return nullptr;
		}
		// Return nothing
		return nullptr;
	}


	// Compare strings
	[[nodiscard]] sdword_t kstrcmp(const sbyte_t* src1, const sbyte_t* src2, std::size_t size) noexcept {
		// Check src1 and src2 pointers
		if (nullptr == src1 || nullptr == src2 || 0u == size) {
			// Handle wrong input
			return ((src1 == nullptr) ? ((src2 == nullptr) ? 0 : -1) : 1);
		}
		// Compare string symbol by symbol
		for (;--size > 0u && *src1 != u8'\0' && *src1 == *src2; ++src1, ++src2);
		// Return string difference
		return byte_t(*src1) - byte_t(*src2);
	}


	// Find char occurrence in string
	[[nodiscard]] sbyte_t* kstrchr(sbyte_t* src, sbyte_t chr, std::size_t size) noexcept {
		// Check src pointer and size
		if (nullptr == src || 0u == size) {
			return nullptr;
		}
		// Find symbol inside string
		for (;--size > 0u && *src != u8'\0' && *src != chr; ++src);
		// Return address of first occurrence or null pointer
		return (*src == chr) ? src : nullptr;
	}

	// Find char occurrence in string
	[[nodiscard]] const sbyte_t* kstrchr(const sbyte_t* src, sbyte_t chr, std::size_t size) noexcept {
		// Check src pointer and size
		if (nullptr == src || 0u == size) {
			return nullptr;
		}
		// Find symbol inside string
		for (;--size > 0u && *src != u8'\0' && *src != chr; ++src);
		// Return address of first occurrence or null pointer
		return (*src == chr) ? src : nullptr;
	}


	// Invert string
	[[maybe_unused]] sbyte_t* kstrinv(sbyte_t* src, std::size_t size) noexcept {
		// Check src pointer and size
		if (nullptr == src) {
			return nullptr;
		}
		// Copy pointer to string start
		auto iter = src;
		// Get string length
		const auto strLen	= kstrlen(src);
		// Get required reverting string length
		const auto len		= (strLen < size) ? strLen : (size - 1u);
		// Calculate half length of the reverting string
		const auto halfLen	= (len >> 1);
		// Loop through image
		for (auto i = len; i != halfLen; i--, iter++) {
			// Swap symbols
			const auto temp = *iter;
			*iter = src[i];
			src[i] = temp;
		}
		// Return string address
		return src;
	}


}	// namespace igros::klib

