////////////////////////////////////////////////////////////////
//
//	Kernel string functions
//
//	File:	string.cpp
//	Date:	12 Sep 2019
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
//	All rights reserved.
//
//


#include <cstdint>

#include <klib/kstring.hpp>


// Kernel library code zone
namespace klib {


	// Calculate string length
	std::size_t kstrlen(const sbyte_t* src) {

		// Check src pointer and size
		if (src == nullptr) {
			return 0;
		}

                // Copy string start pointer
                auto iter = src;
                // Find string end pointer
                for (;*iter != '\0'; ++iter);
                // Return string length
                return iter - src;

        }


	// Copy string from one to other
	sbyte_t* kstrcpy(sbyte_t* src, sbyte_t* dst, std::size_t size) {

		// Save destination pointer
		auto tempDst = dst;
		// Check src, dst pointers and size
		if (src == nullptr || dst == nullptr || size == 0) {
			// Pointer to empty dst
			// Fill with null terminator for sanity
			return &(*tempDst = '\0');
		}

		// Copy first symbol
		*dst = *src;
		// Copy string
		do {
			// Copy string byte by byte
			*++dst = *++src;
		// Stop on size overflow or null terminator
		} while (--size > 0 && *src != '\0');

		// Return pointer to dst tring
		return tempDst;

	}

	// Copy string from one to other
	const sbyte_t* kstrcpy(const sbyte_t* src, sbyte_t* dst, std::size_t size) {

		// Save destination pointer
		auto tempDst = dst;
		// Check src, dst pointers and size
		if (src == nullptr || dst == nullptr || size == 0) {
			// Pointer to empty dst
			// Fill with null terminator for sanity
			return &(*tempDst = '\0');
		}

		// Copy first symbol
		*dst = *src;
		// Copy string
		do {
			// Copy string byte by byte
			*++dst = *++src;
		// Stop on size overflow or null terminator
		} while (--size > 0 && *src != '\0');

		// Return pointer to dst tring
		return tempDst;

	}


	// Compare strings
	sdword_t kstrcmp(const sbyte_t* src1, const sbyte_t* src2, std::size_t size) {

		// Check src1 and src2 pointers
		if (src1 == nullptr || src2 == nullptr || size == 0) {
			// Handle wrong input
			return ((src1 == nullptr) ? ((src2 == nullptr) ? 0 : -1) : 1);
		}

		// Compare string symbol by symbol
		for (;--size > 0 && *src1 != '\0' && *src1 == *src2; ++src1, ++src2);
		// Return string difference
		return byte_t(*src1) - byte_t(*src2);

	}


	// Find char occurrence in string
	sbyte_t* kstrchr(sbyte_t* src, sbyte_t chr, std::size_t size) {

		// Check src pointer and size
		if (src == nullptr || size == 0) {
			return nullptr;
		}

		// Find symbol inside string
		for (;--size > 0 && *src != '\0' && *src != chr; ++src);
		// Return address of first occurrence or null pointer
		return (*src == chr) ? src : nullptr;

	}

	// Find char occurrence in string
	const sbyte_t* kstrchr(const sbyte_t* src, sbyte_t chr, std::size_t size) {

		// Check src pointer and size
		if (src == nullptr || size == 0) {
			return nullptr;
		}

		// Find symbol inside string
		for (;--size > 0 && *src != '\0' && *src != chr; ++src);
		// Return address of first occurrence or null pointer
		return (*src == chr) ? src : nullptr;

	}


}	// namespace klib

