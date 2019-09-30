////////////////////////////////////////////////////////////////
//
//	Kernel string functions
//
//	File:	string.cpp
//	Date:	30 Sep 2019
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
//	All rights reserved.
//
//


#include <cstdint>
//#include <algorithm>

#include <klib/kstring.hpp>


// Kernel library code zone
namespace klib {


	// Find string end
	sbyte_t* kstrend(sbyte_t* src) {

		// Check src pointer
		if (nullptr == src) {
			return nullptr;
		}

                // Find string end pointer
                for (;*src != '\0'; ++src);
		// Return string end
                return src;

	}

	// Find string end
	const sbyte_t* kstrend(const sbyte_t* src) {

		// Check src pointer
		if (nullptr == src) {
			return nullptr;
		}

		// Copy string start pointer
                auto iter = src;
                // Find string end pointer
                for (;*iter != '\0'; ++iter);
		// Return string end
                return iter;

	}


	// Calculate string length
	std::size_t kstrlen(const sbyte_t* src) {

		// Check src pointer
		if (nullptr == src) {
			return 0;
		}
                // Return string length
                return kstrend(src) - src;

        }


	// Copy string from one to other
	sbyte_t* kstrcpy(sbyte_t* src, sbyte_t* dst, std::size_t size) {

		// Save destination pointer
		const auto tempDst = dst;
		// Check src, dst pointers and size
		if (nullptr == src || nullptr == dst || 0 == size) {
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
		const auto tempDst = dst;
		// Check src, dst pointers and size
		if (nullptr == src || nullptr == dst || 0 == size) {
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


	// Concatenate string
	sbyte_t* kstrcat(const sbyte_t* src, sbyte_t* dst, std::size_t size) {

		// Check src, dst pointers and size
		if (nullptr == src || nullptr == dst || 0 == size) {
			// return nothing
			return nullptr;
		}

	}


	// Compare strings
	sdword_t kstrcmp(const sbyte_t* src1, const sbyte_t* src2, std::size_t size) {

		// Check src1 and src2 pointers
		if (nullptr == src1 || nullptr == src2 || 0 == size) {
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
		if (nullptr == src || 0 == size) {
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
		if (nullptr == src || 0 == size) {
			return nullptr;
		}

		// Find symbol inside string
		for (;--size > 0 && *src != '\0' && *src != chr; ++src);
		// Return address of first occurrence or null pointer
		return (*src == chr) ? src : nullptr;

	}


	// Invert string
	sbyte_t* kstrinv(sbyte_t* src, std::size_t size) {

		// Check src pointer and size
		if (nullptr == src) {
			return nullptr;
		}

		//
		auto iter = src;
		//
		const auto strLen	= kstrlen(src);
		// Get string length
		const auto len		= (strLen < size) ? strLen : (size - 1);
		// Calculate half length of the string
		const auto halfLen	= (len >> 1);
		// Loop through image
		for (dword_t i = len; i != halfLen; i--, iter++) {
			// Swap symbols
			const auto temp = *iter;
			*iter = src[i];
			src[i] = temp;
		}
		// Return string address
		return src;

	}


}	// namespace klib

