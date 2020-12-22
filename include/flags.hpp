////////////////////////////////////////////////////////////////
//
//	Bit flags template datatype
//
//	File:	flags.hpp
//	Date:	21 Dec 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <type_traits>
#include <utility>

#include <arch/types.hpp>


// OS namespace
namespace igros {


	// Kernel flags template class
	template<typename T, typename U = typename std::enable_if_t<std::is_enum_v<T>, typename std::underlying_type_t<T>>>
	class kflags {

		static_assert(std::is_enum_v<T>, "Parameter T must be an enum!");

		U	mValue;		// Real flags value


	public:

		// Default c-tor
		constexpr kflags() = default;
		// From initializer list
		template<typename ...Args>
		constexpr explicit kflags(Args &&...args) noexcept; 

		// Type conversion operator
		[[nodiscard]]
		constexpr explicit operator T() const noexcept;
		// Type conversion operator
		[[nodiscard]]
		constexpr explicit operator U() const noexcept;

		// Comparison operator
		constexpr bool operator==(const kflags<T, U> &other) const noexcept;
		// Comparison operator
		constexpr bool operator!=(const kflags<T, U> &other) const noexcept;

		// Comparison operator
		constexpr bool operator==(const T &other) const noexcept;
		// Comparison operator
		constexpr bool operator!=(const T &other) const noexcept;

		// Bitwise AND operator
		constexpr kflags& operator&=(const kflags<T, U> &other) noexcept;
		// Bitwise OR operator
		constexpr kflags& operator|=(const kflags<T, U> &other) noexcept;
		// Bitwise XOR operator
		constexpr kflags& operator^=(const kflags<T, U> &other) noexcept;

		// Bitwise AND operator
		constexpr kflags operator&(const kflags<T, U> &other) const noexcept;
		// Bitwise OR operator
		constexpr kflags operator|(const kflags<T, U> &other) const noexcept;
		// Bitwise XOR operator
		constexpr kflags operator^(const kflags<T, U> &other) const noexcept;

		// Bitwise AND operator
		constexpr kflags& operator&=(const T &other) noexcept;
		// Bitwise OR operator
		constexpr kflags& operator|=(const T &other) noexcept;
		// Bitwise XOR operator
		constexpr kflags& operator^=(const T &other) noexcept;

		// Bitwise AND operator
		constexpr kflags operator&(const T &other) const noexcept;
		// Bitwise OR operator
		constexpr kflags operator|(const T &other) const noexcept;
		// Bitwise XOR operator
		constexpr kflags operator^(const T &other) const noexcept;

		// Bitwise NOT operator
		constexpr kflags operator~() const noexcept;

		// Copy c-tor
		constexpr kflags(const kflags &value) = default;
		// Copy assignment
		constexpr kflags& operator=(const kflags &value) = default;

		// Move c-tor
		constexpr kflags(kflags &&value) = default;
		// Move assignment
		constexpr kflags& operator=(kflags &&value) = default;

		// Type copy c-tor
		constexpr kflags(const T &value) noexcept;
		// Type copy assignment
		constexpr kflags& operator=(const T &value) noexcept;

		// Type move c-tor
		constexpr kflags(T &&value) noexcept;
		// Type move assignment
		constexpr kflags& operator=(T &&value) noexcept;

		// Get value
		[[nodiscard]]
		constexpr U	value() const noexcept;

		// Test bit
		[[nodiscard]]
		constexpr bool	test(const std::size_t bit = 0u) const noexcept;


	};


	// From initializer list
	template<typename T, typename U>
	template<typename ...Args>
	constexpr kflags<T, U>::kflags(Args &&...args) noexcept
		: mValue((static_cast<U>(args) | ...)) {}


	// Type conversion operator
	template<typename T, typename U>
	[[nodiscard]]
	constexpr kflags<T, U>::operator T() const noexcept {
		return static_cast<T>(mValue);
	}

	// Type conversion operator
	template<typename T, typename U>
	[[nodiscard]]
	constexpr kflags<T, U>::operator U() const noexcept {
		return mValue;
	}


	// Comparison operator
	template<typename T, typename U>
	constexpr bool kflags<T, U>::operator==(const kflags<T, U> &other) const noexcept {
		return mValue == other.mValue;
	}

	// Comparison operator
	template<typename T, typename U>
	constexpr bool kflags<T, U>::operator!=(const kflags<T, U> &other) const noexcept {
		return !(*this == other);
	}


	// Comparison operator
	template<typename T, typename U>
	constexpr bool kflags<T, U>::operator==(const T &other) const noexcept {
		return mValue == static_cast<U>(other);
	}

	// Comparison operator
	template<typename T, typename U>
	constexpr bool kflags<T, U>::operator!=(const T &other) const noexcept {
		return !(*this == other);
	}


	// Bitwise AND operator
	template<typename T, typename U>
	constexpr kflags<T, U>& kflags<T, U>::operator&=(const kflags<T, U> &other) noexcept {
		return *this &= static_cast<T>(other);
	}

	// Bitwise OR operator
	template<typename T, typename U>
	constexpr kflags<T, U>& kflags<T, U>::operator|=(const kflags<T, U> &other) noexcept {
		return *this |= static_cast<T>(other);
	}

	// Bitwise XOR operator
	template<typename T, typename U>
	constexpr kflags<T, U>& kflags<T, U>::operator^=(const kflags<T, U> &other) noexcept {
		return *this ^= static_cast<T>(other);
	}


	// Bitwise AND operator
	template<typename T, typename U>
	constexpr kflags<T, U> kflags<T, U>::operator&(const kflags<T, U> &other) const noexcept {
		return *this & static_cast<T>(other);
	}

	// Bitwise OR operator
	template<typename T, typename U>
	constexpr kflags<T, U> kflags<T, U>::operator|(const kflags<T, U> &other) const noexcept {
		return *this | static_cast<T>(other);
	}

	// Bitwise XOR operator
	template<typename T, typename U>
	constexpr kflags<T, U> kflags<T, U>::operator^(const kflags<T, U> &other) const noexcept {
		return *this ^ static_cast<T>(other);
	}


	// Bitwise AND operator
	template<typename T, typename U>
	constexpr kflags<T, U>& kflags<T, U>::operator&=(const T &other) noexcept {
		mValue &= static_cast<U>(other);
		return *this;
	}

	// Bitwise OR operator
	template<typename T, typename U>
	constexpr kflags<T, U>& kflags<T, U>::operator|=(const T &other) noexcept {
		mValue |= static_cast<U>(other);
		return *this;
	}

	// Bitwise XOR operator
	template<typename T, typename U>
	constexpr kflags<T, U>& kflags<T, U>::operator^=(const T &other) noexcept {
		mValue ^= static_cast<U>(other);
		return *this;
	}


	// Bitwise AND operator
	template<typename T, typename U>
	constexpr kflags<T, U> kflags<T, U>::operator&(const T &other) const noexcept {
		return kflags<T, U>(*this) &= other;
	}

	// Bitwise OR operator
	template<typename T, typename U>
	constexpr kflags<T, U> kflags<T, U>::operator|(const T &other) const noexcept {
		return kflags<T, U>(*this) |= other;
	}

	// Bitwise XOR operator
	template<typename T, typename U>
	constexpr kflags<T, U> kflags<T, U>::operator^(const T &other) const noexcept {
		return kflags<T, U>(*this) ^= other;
	}


	// Bitwise NOT operator
	template<typename T, typename U>
	constexpr kflags<T, U> kflags<T, U>::operator~() const noexcept {
		return kflags<T, U>(static_cast<T>(~mValue));
	}


	// Type copy c-tor
	template<typename T, typename U>
	constexpr kflags<T, U>::kflags(const T &value) noexcept
		: mValue(value) {}

	// Type copy assignment
	template<typename T, typename U>
	constexpr kflags<T, U>& kflags<T, U>::operator=(const T &value) noexcept {
		mValue = value;
		return *this;
	}


	// Type move c-tor
	template<typename T, typename U>
	constexpr kflags<T, U>::kflags(T &&value) noexcept
		: mValue(std::move(static_cast<U>(value))) {}

	// Type move assignment
	template<typename T, typename U>
	constexpr kflags<T, U>& kflags<T, U>::operator=(T &&value) noexcept {
		mValue = std::move(value);
		return *this;
	}


	// Get value
	template<typename T, typename U>
	[[nodiscard]]
	constexpr U kflags<T, U>::value() const noexcept {
		return static_cast<U>(*this);
	}


	// Test bit
	template<typename T, typename U>
	[[nodiscard]]
	constexpr bool kflags<T, U>::test(const std::size_t bit) const noexcept {
		return static_cast<U>(1 << bit) == (mValue & static_cast<U>(1 << bit));
	}


}       // namespace igros

