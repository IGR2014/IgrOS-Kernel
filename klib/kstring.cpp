////////////////////////////////////////////////////////////////
//
//	Kernel string functions
//
//	File:	string.cpp
//	Date:	06 Jun 2019
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
//	All rights reserved.
//
//


#include <klib/kstring.hpp>
#include <klib/memset.hpp>


// Kernel library code zone
namespace klib {


	// Calculate string length
	dword_t kstrlen(const sbyte_t* src) {

                // String length value
                dword_t strLength = 0;

                // Calculate string length
                while (*src++ != '\0') {};

                // Return result
                return strLength;

        }

	// Copy string from one to other
	void kstrcpy(const sbyte_t* src, sbyte_t* dst, dword_t size) {

                // Copy string
                while (--size > 0) {

                        // Copy string byte by byte
                        *dst++ = *src++;

                }

        }


}	// namespace klib

