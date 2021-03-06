////////////////////////////////////////////////////////////////
//
//	Kernel mem functions
//
//	File:	kmemory.cpp
//	Date:	12 Feb 2021
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#include <klib/kmemory.hpp>


// Kernel library code zone
namespace igros::klib {


	// Set required memory with specified byte
	[[maybe_unused]]
	pointer_t kmemset8(byte_t* dst, const std::size_t size, const byte_t val) noexcept {
		// Check arguments
		if (nullptr == dst || 0ULL == size) {
			return nullptr;
		}
		// Do actual memset
		for (auto i = 0ULL; i < size; i++) {
			dst[i] = val;
		}
		// Return pointer to dst
		return dst;
	}

	// Set required memory with specified word
	[[maybe_unused]]
	pointer_t kmemset16(word_t* dst, const std::size_t size, const word_t val) noexcept {

		// Check arguments
		if (nullptr == dst || 0ULL == size) {
			return nullptr;
		}

		// Get pointer and size
		auto mDest = dst;
		auto mSize = size;

		// Handle first unaligned byte
		if (0x00 != (reinterpret_cast<std::size_t>(dst) & 0x01)) {
			// Get byte pointer to array
			auto byte	= reinterpret_cast<byte_t*>(dst);
			auto last	= (mSize << 1) - 1ULL;
			// Set unaligned data at the start and end of interval
			byte[0ULL]	= static_cast<byte_t>((val >> 8) & 0xFF);
			byte[last]	= static_cast<byte_t>(val & 0xFF);
			// Update address
			mDest		= reinterpret_cast<word_t*>(&byte[1ULL]);
			// Update size
			--mSize;
		}

		// Do actual memset
		for (auto i = 0ULL; i < mSize; i++) {
			mDest[i] = val;
		}

		// Return pointer to dst
		return dst;

	}

	// Set required memory with specified double word
	[[maybe_unused]]
	pointer_t kmemset32(dword_t* dst, const std::size_t size, const dword_t val) noexcept {

		// Check arguments
		if (nullptr == dst || 0ULL == size) {
			return nullptr;
		}

		// Get pointer and size
		auto mDest = dst;
		auto mSize = size;

		// Check alignment
		switch (reinterpret_cast<std::size_t>(dst) & 0x03) {

			// Handle first unaligned byte + word (if unaligned)
			case 0x03: {
				// Get byte pointer to array
				const auto byte	= reinterpret_cast<byte_t*>(dst);
				const auto last	= (mSize << 2) - 1ULL;
				// Get word pointer to array
				const auto word	= reinterpret_cast<word_t*>(&byte[last - 2ULL]);
				// Set unaligned data at the start and end of interval
				byte[0ULL]	= static_cast<byte_t>((val >> 24) & 0xFF);
				word[0ULL]	= static_cast<word_t>(val & 0xFFFF);
				byte[last]	= static_cast<byte_t>((val >> 16) & 0xFF);
				// Update address
				mDest		= reinterpret_cast<dword_t*>(&byte[1ULL]);
				// Update size
				--mSize;
			} break;

			// Handle first unaligned word (if unaligned)
			case 0x02: {
				// Get word pointer to array
				const auto word	= reinterpret_cast<word_t*>(dst);
				const auto last	= (mSize << 1) - 1ULL;
				// Set unaligned data at the start and end of interval
				word[0ULL]	= static_cast<word_t>((val >> 16) & 0xFFFF);
				word[last]	= static_cast<word_t>(val & 0xFFFF);
				// Update address
				mDest		= reinterpret_cast<dword_t*>(&word[1ULL]);
				// Update size
				--mSize;
			} break;

			// Handle first unaligned byte (if unaligned)
			case 0x01: {
				// Get byte pointer to array
				const auto byte	= reinterpret_cast<byte_t*>(dst);
				const auto last	= (mSize << 2) - 1ULL;
				// Get word pointer to array
				const auto word	= reinterpret_cast<word_t*>(&byte[1ULL]);
				// Set unaligned data at the start and end of interval
				byte[0ULL]	= static_cast<byte_t>((val >> 8) & 0xFF);
				word[0ULL]	= static_cast<word_t>((val >> 16) & 0xFFFF);
				byte[last]	= static_cast<byte_t>(val & 0xFF);
				// Update address
				mDest		= reinterpret_cast<dword_t*>(&word[1ULL]);
				// Update size
				--mSize;
			} break;

			// Default
			default:
				break;

		}

		// Do actual memset
		for (auto i = 0ULL; i < mSize; i++) {
			mDest[i] = val;
		}

		// Return pointer to dst
		return dst;

	}

	// Set required memory with specified quad word
	[[maybe_unused]]
	pointer_t kmemset64(quad_t* dst, const std::size_t size, const quad_t val) noexcept {

		// Check arguments
		if (
			(nullptr == dst)	||
			(0ULL	 == size)
		) {
			return nullptr;
		}

		// Get pointer and size
		auto mDest = dst;
		auto mSize = size;

		// Do actual memset
		for (auto i = 0ULL; i < mSize; i++) {
			mDest[i] = val;
		}

		// Return pointer to dst
		return dst;

	}


	// Copy memory
	[[maybe_unused]]
	pointer_t kmemcpy(const pointer_t dst, const pointer_t src, const std::size_t size) noexcept {
		// Check arguments
		if (
			(nullptr == dst)	||
			(nullptr == src)	||
			(dst	 == src)	||
			(0ULL	 == size)
		) {
			return nullptr;
		}
		// Do actual memcpy
		for (auto i = 0ULL; i < size; i++) {
			static_cast<byte_t*>(dst)[i] = static_cast<byte_t*>(src)[i];
		}
		// Return pointer to dst
		return dst;
	}


}	// namespace igros::klib

