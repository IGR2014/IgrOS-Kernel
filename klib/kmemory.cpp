////////////////////////////////////////////////////////////////
//
//	Kernel mem functions
//
//	File:	kmemory.cpp
//	Date:	02 Jul 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#include <klib/kmemory.hpp>


// Kernel library code zone
namespace igros::klib {


	// Set required memory with specified byte
	[[maybe_unused]] pointer_t kmemset8(const pointer_t dst, const std::size_t size, const byte_t val) noexcept {
		// Check arguments
		if (nullptr == dst || 0u == size) {
			return nullptr;
		}
		// Do actual memset
		for (auto i = 0ull; i < size; i++) {
			static_cast<byte_t*>(dst)[i] = val;
		}
		// Return pointer to dst
		return dst;
	}

	// Set required memory with specified word
	[[maybe_unused]] pointer_t kmemset16(const pointer_t dst, const std::size_t size, const word_t val) noexcept {
		// Check arguments
		if (nullptr == dst || 0ull == size) {
			return nullptr;
		}
		// Get temporary pointer to destination
		auto tempDest = dst;
		auto tempSize = size;
		// Handle first unaligned byte
		if (0x00 != (reinterpret_cast<std::size_t>(dst) & 0x01)) {
			// Set unaligned data at the start and end of interval
			static_cast<byte_t*>(dst)[0ull]			= static_cast<byte_t>(val & 0x00FF);
			static_cast<byte_t*>(dst)[tempSize - 1ull]	= static_cast<byte_t>((val & 0xFF00) >> 8);
			// Update address
			tempDest					= &static_cast<byte_t*>(tempDest)[1ull];
			// Update size
			--tempSize;
		}
		// Do actual memset
		for (auto i = 0ull; i < size; i++) {
			static_cast<word_t*>(tempDest)[i] = val;
		}
		// Return pointer to dst
		return dst;
	}

	// Set required memory with specified double word
	[[maybe_unused]] pointer_t kmemset32(const pointer_t dst, const std::size_t size, const dword_t val) noexcept {
		// Check arguments
		if (nullptr == dst || 0ull == size) {
			return nullptr;
		}
		// Get temporary pointer to destination
		auto tempDest = dst;
		auto tempSize = size;
		// Handle first unaligned byte + word (if unaligned)
		if (0x03 == (reinterpret_cast<std::size_t>(dst) & 0x03)) {
			// Set unaligned data at the start and end of interval
			auto first	= &static_cast<byte_t*>(dst)[0ull];
			auto next	= reinterpret_cast<word_t*>(&static_cast<byte_t*>(dst)[1ull]);
			auto last	= &static_cast<byte_t*>(dst)[tempSize - 1ull];
			*next		= static_cast<word_t>((val & 0x0000FFFF));
			*first		= static_cast<byte_t>((val & 0x00FF0000) >> 16);
			*last		= static_cast<byte_t>((val & 0xFF000000) >> 24);
			// Update address
			tempDest	= &static_cast<byte_t*>(tempDest)[3ull];
			// Update size
			--tempSize;
		// Handle first unaligned word (if unaligned)
		} else if (0x02 != (reinterpret_cast<std::size_t>(dst) & 0x03)) {
			// Set unaligned data at the start and end of interval
			auto first	= &static_cast<word_t*>(dst)[0ull];
			auto last	= &static_cast<word_t*>(dst)[tempSize - 1ull];
			*first		= static_cast<word_t>((val & 0x0000FFFF));
			*last		= static_cast<word_t>((val & 0xFFFF0000) >> 16);
			// Update address
			tempDest	= &static_cast<byte_t*>(tempDest)[2ull];
			// Update size
			--tempSize;
		// Handle first unaligned byte (if unaligned)
		} else if (0x01 != (reinterpret_cast<std::size_t>(dst) & 0x03)) {
			// Set unaligned data at the start and end of interval
			auto first	= &static_cast<byte_t*>(dst)[0ull];
			auto next	= reinterpret_cast<word_t*>(&static_cast<byte_t*>(dst)[tempSize - 3ull]);
			auto last	= &static_cast<byte_t*>(dst)[tempSize - 1ull];
			*first		= static_cast<byte_t>((val & 0x000000FF));
			*last		= static_cast<byte_t>((val & 0x0000FF00) >> 8);
			*next		= static_cast<word_t>((val & 0xFFFF0000) >> 16);
			// Update address
			tempDest	= &static_cast<byte_t*>(tempDest)[1ull];
			// Update size
			--tempSize;
		}
		// Do actual memset
		for (auto i = 0ull; i < size; i++) {
			static_cast<dword_t*>(tempDest)[i] = val;
		}
		// Return pointer to dst
		return dst;
	}


	// Copy memory
	[[maybe_unused]] pointer_t kmemcpy(const pointer_t dst, const pointer_t src, const std::size_t size) noexcept {
		// Check arguments
		if (nullptr == dst || nullptr == src || dst == src || 0ull == size) {
			return nullptr;
		}
		// Do actual memcpy
		for (auto i = 0ull; i < size; i++) {
			static_cast<byte_t*>(dst)[i] = static_cast<byte_t*>(src)[i];
		}
		// Return pointer to dst
		return dst;
	}


}	// namespace igros::klib

