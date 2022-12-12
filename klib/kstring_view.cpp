////////////////////////////////////////////////////////////////
//
//	Kernel string_view class definition
//
//	File:	kstring_view.cpp
//	Date:	01 Oct 2021
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


// C++
#include <algorithm>
// Arch
#include <arch/cpu.hpp>
// Klib
#include <klib/kstring.hpp>
#include <klib/kstring_view.hpp>


// Kernel library code zone
namespace igros::klib {


	// Default c-tor
	constexpr kstring_view::kstring_view() noexcept :
		mData	(nullptr),
		mLength	(0_usize) {}

	// C-tor
	constexpr kstring_view::kstring_view(const char* const data, const std::size_t length) noexcept :
		mData	(data),
		mLength	(0_usize != length ? length : kstrlen(data)) {}


	// Equality compare operator
	[[nodiscard]]
	constexpr bool kstring_view::operator==(const kstring_view &other) noexcept {
		// String equality
		return (0_usize == kstrcmp(mData, other.mData, std::min(mLength, other.mLength)));
	}

	// Ordering compare operator
	[[nodiscard]]
	constexpr auto kstring_view::operator<=>(const kstring_view &other) noexcept {
		// TODO
	}


	// Subscript operator
	[[nodiscard]]
	constexpr char kstring_view::operator[](const std::size_t index) const noexcept {
		// Check range
		if (index >= mLength) [[unlikely]] {
			// Hang cpu
			arch::cpu::get().halt();
		}
		// Return symbol
		return mData[index];
	}


	// Get string data
	[[nodiscard]]
	constexpr const char* kstring_view::data() const noexcept {
		return mData;
	}


	// Get string length
	[[nodiscard]]
	constexpr std::size_t kstring_view::length() const noexcept {
		return mLength;
	}


	// Is strinv empty ?
	[[nodiscard]]
	constexpr bool kstring_view::isEmpty() const noexcept {
		return 0U == mLength;
	}


}	// namespace igros::klib

