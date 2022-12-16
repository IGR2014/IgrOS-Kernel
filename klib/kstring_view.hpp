////////////////////////////////////////////////////////////////
//
//	Kernel string_view class declaration
//
//	File:	kstring_view.hpp
//	Date:	16 Dec 2022
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// IgrOS-Kernel arch
#include <arch/types.hpp>


// Kernel library code zone
namespace igros::klib {


	// String view representation class
	class kstring_view {

		const char*	mData		{nullptr};		// String data
		igros_usize_t	mLength		{0_usize};		// String length


	public:

		// Default c-tor
		constexpr kstring_view() noexcept;
		// C-tor
		constexpr kstring_view(const char* const data, const std::size_t length = 0U) noexcept;

		// Copy c-tor
		constexpr kstring_view(const kstring_view &other) noexcept = default;
		// Copy assignment
		[[nodiscard]]
		constexpr auto	operator=(const kstring_view &other) noexcept -> kstring_view& = default;

		// Move c-tor
		constexpr kstring_view(kstring_view &&other) noexcept = default;
		// Move assignment
		[[nodiscard]]
		constexpr auto	operator=(kstring_view &&other) noexcept -> kstring_view& = default;

		// Equality compare operator
		[[nodiscard]]
		constexpr auto	operator==(const kstring_view &other) noexcept -> bool;
		// Ordering compare operator
		[[nodiscard]]
		constexpr auto	operator<=>(const kstring_view &other) noexcept;

		// Subscript operator
		[[nodiscard]]
		constexpr auto	operator[](const std::size_t index) const noexcept -> char;

		// Get string data
		[[nodiscard]]
		constexpr auto	data() const noexcept -> const char*;

		// Get string length
		[[nodiscard]]
		constexpr auto	length() const noexcept -> igros_usize_t;

		// Is strinv empty ?
		[[nodiscard]]
		constexpr auto	isEmpty() const noexcept -> bool;


	};


}	// namespace igros::klib

