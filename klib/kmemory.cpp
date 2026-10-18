////////////////////////////////////////////////////////////////
//
//	Kernel mem functions
//
//	File:	kmemory.cpp
//	Date:	16 Dec 2022
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


// C++
#include <algorithm>
// IgrOS-Kernel library
#include <klib/kmemory.hpp>


// Kernel library code zone
namespace igros::klib {


	// Copy memory
	[[maybe_unused]]
	auto kmemcpy(const igros_pointer_t dst, const void* const src, const igros_usize_t size) noexcept -> igros_pointer_t {
		// Check arguments
		if ((nullptr == dst) || (nullptr == src)) [[unlikely]] {
			return nullptr;
		}
		// Nothing to copy
		if ((dst == src) || (0_usize == size)) [[unlikely]] {
			return dst;
		}
		// Do actual memcpy
		std::copy_n(static_cast<const igros_byte_t*>(src), size, static_cast<igros_byte_t*>(dst));
		// Return pointer to dst
		return dst;
	}


}	// namespace igros::klib

