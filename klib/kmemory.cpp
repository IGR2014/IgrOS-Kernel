////////////////////////////////////////////////////////////////
//
//	Kernel mem functions
//
//	File:	kmemory.cpp
//	Date:	08 Dec 2022
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


#include <klib/kmemory.hpp>


// Kernel library code zone
namespace igros::klib {


	// Set required memory with specified byte
	[[maybe_unused]]
	auto kmemset8(igros_byte_t* dst, const igros_usize_t size, const igros_byte_t val) noexcept -> igros_pointer_t {
		// Check arguments
		if ((nullptr == dst) || (0_usize == size)) [[unlikely]] {
			return nullptr;
		}
		// Do actual memset
		for (auto i {0_usize}; i < size; i++) {
			dst[i] = val;
		}
		// Return pointer to dst
		return dst;
	}

	// Set required memory with specified word
	[[maybe_unused]]
	auto kmemset16(igros_word_t* dst, const igros_usize_t size, const igros_word_t val) noexcept -> igros_pointer_t {

		// Check arguments
		if ((nullptr == dst) || (0_usize == size)) [[unlikely]] {
			return nullptr;
		}

		// Get pointer and size
		auto mDest {dst};
		auto mSize {size};

		// Handle first unaligned byte
		if (0_usize != (std::bit_cast<igros_usize_t>(dst) & 1_usize)) {
			// Get byte pointer to array
			auto byte	{std::bit_cast<igros_byte_t*>(dst)};
			auto last	{(mSize << 1) - 1_usize};
			// Set unaligned data at the start and end of interval
			byte[0_usize]	= static_cast<igros_byte_t>((val >> 8) & 0xFF_u16);
			byte[last]	= static_cast<igros_byte_t>(val & 0xFF_u16);
			// Update address
			mDest		= std::bit_cast<igros_word_t*>(&byte[1_usize]);
			// Update size
			--mSize;
		}

		// Do actual memset
		for (auto i {0_usize}; i < mSize; i++) {
			mDest[i] = val;
		}

		// Return pointer to dst
		return dst;

	}

	// Set required memory with specified double word
	[[maybe_unused]]
	auto kmemset32(igros_dword_t* dst, const igros_usize_t size, const igros_dword_t val) noexcept -> igros_pointer_t {

		// Check arguments
		if ((nullptr == dst) || (0_usize == size)) [[unlikely]] {
			return nullptr;
		}

		// Get pointer and size
		auto mDest {dst};
		auto mSize {size};

		// Check alignment
		switch (std::bit_cast<igros_usize_t>(dst) & 3_usize) {

			// Handle first unaligned byte + word (if unaligned)
			case 3_usize: {
				// Get byte pointer to array
				const auto byte	{std::bit_cast<igros_byte_t*>(dst)};
				const auto last	{(mSize << 2) - 1_usize};
				// Get word pointer to array
				const auto word	{std::bit_cast<igros_word_t*>(&byte[last - 2_usize])};
				// Set unaligned data at the start and end of interval
				byte[0_usize]	= static_cast<igros_byte_t>((val >> 24) & 0xFF_usize);
				word[0_usize]	= static_cast<igros_word_t>(val & 0xFFFF_usize);
				byte[last]	= static_cast<igros_byte_t>((val >> 16) & 0xFF_usize);
				// Update address
				mDest		= std::bit_cast<igros_dword_t*>(&byte[1_usize]);
				// Update size
				--mSize;
			} break;

			// Handle first unaligned word (if unaligned)
			case 2_usize: {
				// Get word pointer to array
				const auto word	{std::bit_cast<igros_word_t*>(dst)};
				const auto last	{(mSize << 1) - 1_usize};
				// Set unaligned data at the start and end of interval
				word[0_usize]	= static_cast<igros_word_t>((val >> 16) & 0xFFFF_usize);
				word[last]	= static_cast<igros_word_t>(val & 0xFFFF_usize);
				// Update address
				mDest		= std::bit_cast<igros_dword_t*>(&word[1_usize]);
				// Update size
				--mSize;
			} break;

			// Handle first unaligned byte (if unaligned)
			case 1_usize: {
				// Get byte pointer to array
				const auto byte	{std::bit_cast<igros_byte_t*>(dst)};
				const auto last	{(mSize << 2) - 1_usize};
				// Get word pointer to array
				const auto word	{std::bit_cast<igros_word_t*>(&byte[1_usize])};
				// Set unaligned data at the start and end of interval
				byte[0_usize]	= static_cast<igros_byte_t>((val >> 8) & 0xFF_usize);
				word[0_usize]	= static_cast<igros_word_t>((val >> 16) & 0xFFFF_usize);
				byte[last]	= static_cast<igros_byte_t>(val & 0xFF_usize);
				// Update address
				mDest		= std::bit_cast<igros_dword_t*>(&word[1_usize]);
				// Update size
				--mSize;
			} break;

			// Default
			default:
				break;

		}

		// Do actual memset
		for (auto i {0_usize}; i < mSize; i++) {
			mDest[i] = val;
		}

		// Return pointer to dst
		return dst;

	}

	// Set required memory with specified quad word
	[[maybe_unused]]
	auto kmemset64(igros_quad_t* dst, const igros_usize_t size, const igros_quad_t val) noexcept -> igros_pointer_t {
		// Check arguments
		if ((nullptr == dst) || (0_usize == size)) [[unlikely]] {
			return nullptr;
		}
		// Get pointer and size
		auto mDest {dst};
		auto mSize {size};
		// Do actual memset
		for (auto i {0_usize}; i < mSize; i++) {
			mDest[i] = val;
		}
		// Return pointer to dst
		return dst;
	}


	// Copy memory
	[[maybe_unused]]
	auto kmemcpy(const igros_pointer_t dst, const igros_pointer_t src, const igros_usize_t size) noexcept -> igros_pointer_t {
		// Check arguments
		if ((nullptr == dst) || (nullptr == src) || (dst == src) || (0_usize == size)) [[unlikely]] {
			return nullptr;
		}
		// Do actual memcpy
		for (auto i {0_usize}; i < size; i++) {
			static_cast<igros_byte_t*>(dst)[i] = static_cast<igros_byte_t*>(src)[i];
		}
		// Return pointer to dst
		return dst;
	}


}	// namespace igros::klib

