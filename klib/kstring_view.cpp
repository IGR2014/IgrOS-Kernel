////////////////////////////////////////////////////////////////
//
//	Kernel string_view class definition
//
//	File:	kstring_view.cpp
//	Date:	16 Dec 2022
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


// C++
#include <algorithm>
// IgrOS-Kernel arch
#include <arch/cpu.hpp>
// IgrOS-Kernel library
#include <klib/kstring.hpp>
#include <klib/kstring_view.hpp>


// Kernel library code zone
namespace igros::klib {


	// C-tor
	constexpr kstring_view::kstring_view(const char* const data, const std::size_t length) noexcept :
		mData	(data),
		mLength	(length) {}

	// C-tor
	constexpr kstring_view::kstring_view(const char* const data) noexcept :
		kstring_view(data, kstrlen(data)) {}


	// Ordering compare operator
	[[nodiscard]]
	constexpr auto kstring_view::operator<=>(const kstring_view &other) noexcept {
		// Lexicographical compare
		return std::lexicographical_compare_three_way(
			&mData[0_usize],	&mData[mLength - 1_usize],
			&other.mData[0_usize],	&other.mData[mLength - 1_usize]
		);
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

