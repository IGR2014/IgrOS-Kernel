////////////////////////////////////////////////////////////////
//
//	Bit flags template datatype
//
//	File:	flags.hpp
//	Date:	25 Jul 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <type_traits>

#include <arch/types.hpp>


// OS namespace
namespace igros {


	// Flags wrapper struct
	template <typename T, typename U = typename std::enable_if_t<std::is_enum_v<T>, typename std::underlying_type_t<T>>>
	class kflags {

		U	mFlags;		// Flags field


	public:

		// C-tor
		constexpr kflags() noexcept = default;
		// C-tor
		constexpr kflags(const T flag) noexcept;
		// C-tor
		constexpr kflags(const U flag) noexcept;
		// Copy c-tor
		constexpr kflags(const kflags<T, U> &flags) noexcept;

		// Convet to bool
		constexpr operator bool() const noexcept;
		// Convert to underlying type
		constexpr operator U() const noexcept;

		// Comparison
		constexpr bool operator== (const kflags<T, U> other) const noexcept;
		// Comparison
		constexpr bool operator!= (const kflags<T, U> other) const noexcept;

		// Bitwise OR operator for flags
		constexpr kflags<T, U> operator|(const kflags<T, U> flag) const noexcept;
		// Bitwise AND operator for flags
		constexpr kflags<T, U> operator&(const kflags<T, U> flag) const noexcept;
		// Bitwise XOR operator for flags
		constexpr kflags<T, U> operator^(const kflags<T, U> flag) const noexcept;

		// Bitwise NOT operator for flags
		constexpr kflags<T, U> operator~() noexcept;

		// Bitwise OR assignment operator for flags
		constexpr kflags<T, U>& operator|=(const kflags<T, U> flag) noexcept;
		// Bitwise AND assignment operator for flags
		constexpr kflags<T, U>& operator&=(const kflags<T, U> flag) noexcept;
		// Bitwise XOR assignment operator for flags
		constexpr kflags<T, U>& operator^=(const kflags<T, U> flag) noexcept;


	};


	// C-tor
	template <typename T, typename U>
	constexpr kflags<T, U>::kflags(const T flag) noexcept
		: mFlags(static_cast<U>(flag)) {}

	// C-tor
	template <typename T, typename U>
	constexpr kflags<T, U>::kflags(const U flag) noexcept
		: mFlags(flag) {}

	// Copy c-tor
	template <typename T, typename U>
	constexpr kflags<T, U>::kflags(const kflags<T, U> &flags) noexcept
		: mFlags(flags.mFlags) {}



	// Convet to bool
	template <typename T, typename U>
	constexpr kflags<T, U>::operator bool() const noexcept {
		return (0u != mFlags);
	}

	// Convert to underlying type
	template <typename T, typename U>
	constexpr kflags<T, U>::operator U() const noexcept {
		return mFlags;
	}


	// Comparison
	template <typename T, typename U>
	constexpr bool kflags<T, U>::operator== (const kflags<T, U> other) const noexcept {
		return mFlags == other.mFlags;
	}

	// Comparison
	template <typename T, typename U>
	constexpr bool kflags<T, U>::operator!= (const kflags<T, U> other) const noexcept {
		return !(*this == other);
	}


	// Bitwise OR operator for flags
	template <typename T, typename U>
	constexpr kflags<T, U> kflags<T, U>::operator| (const kflags<T, U> flag) const noexcept {
		// OR on underlying types
		return static_cast<T>(mFlags | flag.mFlags);
	}

	// Bitwise AND operator for flags
	template <typename T, typename U>
	constexpr kflags<T, U> kflags<T, U>::operator& (const kflags<T, U> flag) const noexcept {
		// AND on underlying types
		return static_cast<T>(mFlags & flag.mFlags);
	}

	// Bitwise XOR operator for flags
	template <typename T, typename U>
	constexpr kflags<T, U> kflags<T, U>::operator^ (const kflags<T, U> flag) const noexcept {
		// XOR on underlying types
		return static_cast<T>(mFlags ^ flag.mFlags);
	}

	// Bitwise NOT operator for flags
	template <typename T, typename U>
	constexpr kflags<T, U> kflags<T, U>::operator~ () noexcept {
		// NOT on underlying types
		return static_cast<T>(~mFlags);
	}

	// Bitwise OR assignment operator for flags
	template <typename T, typename U>
	constexpr kflags<T, U>& kflags<T, U>::operator|= (const kflags<T, U> flag) noexcept {
		// OR on underlying types
		mFlags |= flag.mFlags;
		// Return reference
		return *this;
	}

	// Bitwise AND assignment operator for flags
	template <typename T, typename U>
	constexpr kflags<T, U>& kflags<T, U>::operator&= (const kflags<T, U> flag) noexcept {
		// AND on underlying types
		mFlags &= flag.mFlags;
		// Return reference
		return *this;
	}

	// Bitwise XOR assignment operator for flags
	template <typename T, typename U>
	constexpr kflags<T, U>& kflags<T, U>::operator^= (const kflags<T, U> flag) noexcept {
		// XOR on underlying types
		mFlags ^= flag.mFlags;
		// Return reference
		return *this;
	}


}       // namespace igros

