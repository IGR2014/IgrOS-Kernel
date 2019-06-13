////////////////////////////////////////////////////////////////
//
//	Kernel string functions
//
//	File:	string.cpp
//	Date:	14 Jun 2019
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
//	All rights reserved.
//
//


#include <cstdint>

#include <klib/kstring.hpp>
#include <klib/memset.hpp>


// Kernel library code zone
namespace klib {


	// Calculate string length
	std::size_t kstrlen(const sbyte_t* src) {

		// Soure string's start pointer
		auto srcPtr = src;
		// Get source string end pointer
		for (; *srcPtr != '\0'; ++srcPtr);
		// Calculate string length
		return (srcPtr - src);

        }

	// Copy string from one to other
	sbyte_t* kstrcpy(const sbyte_t* src, sbyte_t* dst, std::size_t size) {

		// Destination pointer
		auto dstPtr = dst;

		// Check src, dst and size
                if (src == nullptr || dst == nullptr || size == 0) {

			return dstPtr;

		}

		// Copy first symbol from src to dst
		*dst = *src;

		// Copy string while possible
		do {

			*++dst = *++src;
                    
		} while (--size > 0 && *src != '\0');

		// Destination address
		return dstPtr;

        }


}	// namespace klib

