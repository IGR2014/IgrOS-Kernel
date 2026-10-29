////////////////////////////////////////////////////////////////
///
///	@brief		Bit flags template datatype
///
///	@file		kFlags.hpp
///	@date		21 Mar 2023
///
///	@copyright	Copyright (c) 2017 - 2022,
///			All rights reserved.
///	@author		Igor Baklykov
///
///


#pragma once


// C++
#include <bit>
#include <concepts>
#include <type_traits>
#include <utility>
// IgrOS-Kernel arch
#include <arch/types.hpp>


////////////////////////////////////////////////////////////////
///
/// @brief IgrOS Kernel Library namespace
/// @namespace igros::klib
///
namespace igros::klib {


	////////////////////////////////////////////////////
	///
	/// @brief Enum type with given underlying type
	/// @tparam T Enum type
	/// @tparam U Underlying type of @c T
	///
	template<class T, class U>
	concept kflags_enum = std::is_enum_v<T> && std::same_as<U, std::underlying_type_t<T>>;


	////////////////////////////////////////////////////
	///
	/// @brief Bitwise flags template class
	/// @class kFlags
	/// @tparam T Flags internal representation enum type
	/// @tparam U Flags internal underlying representation enum type
	///
	template<class T, class U = std::underlying_type_t<T>>
	requires kflags_enum<T, U>
	class kFlags {

		U	mValue;		///< Real flags value


	public:

		/// @brief Default c-tor
		constexpr kFlags() noexcept = default;
		/// @brief From initializer list
		template<class ...Args>
		constexpr explicit kFlags(std::in_place_t /*tag*/, Args &&...args) noexcept; 

		/// @brief Type conversion operator
		[[nodiscard]]
		constexpr explicit operator T() const noexcept;
		/// @brief Type conversion operator
		[[nodiscard]]
		constexpr explicit operator U() const noexcept;

		/// @brief Comparison operator
		constexpr auto	operator==(const kFlags<T, U> &other) const noexcept -> bool = default;

		/// @brief Comparison operator
		constexpr auto	operator==(const T &other) const noexcept -> bool;

		/// @brief Bitwise AND operator
		[[maybe_unused]]
		constexpr auto	operator&=(const kFlags<T, U> &other) noexcept -> kFlags&;
		/// @brief Bitwise OR operator
		[[maybe_unused]]
		constexpr auto	operator|=(const kFlags<T, U> &other) noexcept -> kFlags&;
		/// @brief Bitwise XOR operator
		[[maybe_unused]]
		constexpr auto	operator^=(const kFlags<T, U> &other) noexcept -> kFlags&;

		/// @brief Bitwise AND operator (hidden friend: found by ADL only)
		[[nodiscard]]
		friend constexpr auto	operator&(kFlags lhs, const kFlags &rhs) noexcept -> kFlags {
			lhs &= rhs;
			return lhs;
		}
		/// @brief Bitwise OR operator (hidden friend: found by ADL only)
		[[nodiscard]]
		friend constexpr auto	operator|(kFlags lhs, const kFlags &rhs) noexcept -> kFlags {
			lhs |= rhs;
			return lhs;
		}
		/// @brief Bitwise XOR operator (hidden friend: found by ADL only)
		[[nodiscard]]
		friend constexpr auto	operator^(kFlags lhs, const kFlags &rhs) noexcept -> kFlags {
			lhs ^= rhs;
			return lhs;
		}

		/// @brief Bitwise AND operator
		[[maybe_unused]]
		constexpr auto	operator&=(const T &other) noexcept -> kFlags&;
		/// @brief Bitwise OR operator
		[[maybe_unused]]
		constexpr auto	operator|=(const T &other) noexcept -> kFlags&;
		/// @brief Bitwise XOR operator
		[[maybe_unused]]
		constexpr auto	operator^=(const T &other) noexcept -> kFlags&;

		/// @brief Bitwise AND operator (hidden friend: found by ADL only)
		[[nodiscard]]
		friend constexpr auto	operator&(kFlags lhs, const T &rhs) noexcept -> kFlags {
			lhs &= rhs;
			return lhs;
		}
		/// @brief Bitwise OR operator (hidden friend: found by ADL only)
		[[nodiscard]]
		friend constexpr auto	operator|(kFlags lhs, const T &rhs) noexcept -> kFlags {
			lhs |= rhs;
			return lhs;
		}
		/// @brief Bitwise XOR operator (hidden friend: found by ADL only)
		[[nodiscard]]
		friend constexpr auto	operator^(kFlags lhs, const T &rhs) noexcept -> kFlags {
			lhs ^= rhs;
			return lhs;
		}

		/// @brief Bitwise NOT operator
		[[nodiscard]]
		constexpr auto	operator~() const noexcept -> kFlags;

		/// @brief Copy c-tor
		constexpr kFlags(const kFlags &value) noexcept = default;
		/// @brief Copy assignment
		constexpr auto	operator=(const kFlags &value) noexcept -> kFlags& = default;

		/// @brief Move c-tor
		constexpr kFlags(kFlags &&value) noexcept = default;
		/// @brief Move assignment
		constexpr auto	operator=(kFlags &&value) noexcept -> kFlags& = default;

		/// @brief Type copy c-tor
		constexpr explicit kFlags(const T &value) noexcept;
		/// @brief Type copy assignment
		constexpr auto	operator=(const T &value) noexcept -> kFlags&;

		/// @brief Get value
		[[nodiscard]]
		constexpr auto	value() const noexcept -> U;

		/// @brief Check if empty
		[[nodiscard]]
		constexpr auto	isEmpty() const noexcept -> bool;
		/// @brief Check if flag is set
		[[nodiscard]]
		constexpr auto	isSet(const T value) const noexcept -> bool;

		/// @brief Test bit
		[[nodiscard]]
		constexpr auto	test(const igros_usize_t bit = 0_usize) const noexcept -> bool;


	};


	////////////////////////////////////////////////////
	///
	/// @brief From initializer list
	/// @tparam Args List of set flags
	/// @param[in] args List of flags
	/// @param[in] tag Disambiguation tag
	///
	template<class T, class U>
	requires kflags_enum<T, U>
	template<class ...Args>
	constexpr kFlags<T, U>::kFlags(std::in_place_t /*tag*/, Args &&...args) noexcept :
		mValue((static_cast<U>(args) | ...)) {}


	////////////////////////////////////////////////////
	///
	/// @brief Type conversion operator
	/// @return Flag value of type @c T
	///
	template<class T, class U>
	requires kflags_enum<T, U>
	[[nodiscard]]
	constexpr kFlags<T, U>::operator T() const noexcept {
		return static_cast<T>(mValue);
	}

	////////////////////////////////////////////////////
	///
	/// @brief Type conversion operator
	/// @return Flag underlying value of type @c U
	///
	template<class T, class U>
	requires kflags_enum<T, U>
	[[nodiscard]]
	constexpr kFlags<T, U>::operator U() const noexcept {
		return mValue;
	}


	////////////////////////////////////////////////////
	///
	/// @brief Comparison operator
	/// @param[in] other Other flags value of type @c T
	/// @return Indicates if flag values are equal
	///
	template<class T, class U>
	requires kflags_enum<T, U>
	constexpr auto kFlags<T, U>::operator==(const T &other) const noexcept -> bool {
		return mValue == static_cast<U>(other);
	}


	////////////////////////////////////////////////////
	///
	/// @brief Bitwise AND operator
	/// @param[in] other Other flags value
	/// @return Reference to @c this for chaining
	///
	template<class T, class U>
	requires kflags_enum<T, U>
	[[maybe_unused]]
	constexpr auto kFlags<T, U>::operator&=(const kFlags<T, U> &other) noexcept -> kFlags<T, U>& {
		*this &= static_cast<T>(other);
		return *this;
	}

	////////////////////////////////////////////////////
	///
	/// @brief Bitwise OR operator
	/// @param[in] other Other flags value
	/// @return Reference to @c this for chaining
	///
	template<class T, class U>
	requires kflags_enum<T, U>
	[[maybe_unused]]
	constexpr auto kFlags<T, U>::operator|=(const kFlags<T, U> &other) noexcept -> kFlags<T, U>& {
		*this |= static_cast<T>(other);
		return *this;
	}

	////////////////////////////////////////////////////
	///
	/// @brief Bitwise XOR operator
	/// @param[in] other Other flags value
	/// @return Reference to @c this for chaining
	///
	template<class T, class U>
	requires kflags_enum<T, U>
	[[maybe_unused]]
	constexpr auto kFlags<T, U>::operator^=(const kFlags<T, U> &other) noexcept -> kFlags<T, U>& {
		*this ^= static_cast<T>(other);
		return *this;
	}



	////////////////////////////////////////////////////
	///
	/// @brief Bitwise AND operator
	/// @param[in] other Other flags value of type @c T
	/// @return Reference to @c this for chaining
	///
	template<class T, class U>
	requires kflags_enum<T, U>
	[[maybe_unused]]
	constexpr auto kFlags<T, U>::operator&=(const T &other) noexcept -> kFlags<T, U>& {
		// Do AND
		mValue &= static_cast<U>(other);
		// Reference chaining
		return *this;
	}

	////////////////////////////////////////////////////
	///
	/// @brief Bitwise OR operator
	/// @param[in] other Other flags value of type @c T
	/// @return Reference to @c this for chaining
	///
	template<class T, class U>
	requires kflags_enum<T, U>
	[[maybe_unused]]
	constexpr auto kFlags<T, U>::operator|=(const T &other) noexcept -> kFlags<T, U>& {
		// Do OR
		mValue |= static_cast<U>(other);
		// Reference chaining
		return *this;
	}

	////////////////////////////////////////////////////
	///
	/// @brief Bitwise XOR operator
	/// @param[in] other Other flags value of type @c T
	/// @return Reference to @c this for chaining
	///
	template<class T, class U>
	requires kflags_enum<T, U>
	[[maybe_unused]]
	constexpr auto kFlags<T, U>::operator^=(const T &other) noexcept -> kFlags<T, U>& {
		// Do XOR
		mValue ^= static_cast<U>(other);
		// Reference chaining
		return *this;
	}



	////////////////////////////////////////////////////
	///
	/// @brief Bitwise NOT operator
	/// @return Result of (NOT this)
	///
	template<class T, class U>
	requires kflags_enum<T, U>
	[[nodiscard]]
	constexpr auto kFlags<T, U>::operator~() const noexcept -> kFlags<T, U> {
		return kFlags<T, U>(static_cast<T>(~mValue));
	}


	////////////////////////////////////////////////////
	///
	/// @brief Type copy c-tor
	/// @param[in] value Other flags value of type @c T
	///
	template<class T, class U>
	requires kflags_enum<T, U>
	constexpr kFlags<T, U>::kFlags(const T &value) noexcept :
		mValue(static_cast<U>(value)) {}

	////////////////////////////////////////////////////
	///
	/// @brief Type copy assignment
	/// @param[in] value Other flags value of type @c T
	/// @return Reference to @c this for chaining
	///
	template<class T, class U>
	requires kflags_enum<T, U>
	constexpr auto kFlags<T, U>::operator=(const T &value) noexcept -> kFlags<T, U>& {
		// Set new value
		mValue = static_cast<U>(value);
		// Reference chaining
		return *this;
	}



	////////////////////////////////////////////////////
	///
	/// @brief Get value
	/// @return Flags underlying value of type @c U
	///
	template<class T, class U>
	requires kflags_enum<T, U>
	[[nodiscard]]
	constexpr auto kFlags<T, U>::value() const noexcept -> U {
		return static_cast<U>(*this);
	}


	////////////////////////////////////////////////////
	///
	/// @brief Check if empty
	/// @return State of checked flag @c value
	///
	template<class T, class U>
	requires kflags_enum<T, U>
	[[nodiscard]]
	constexpr auto kFlags<T, U>::isEmpty() const noexcept -> bool {
		return *this == static_cast<T>(0);
	}

	////////////////////////////////////////////////////
	///
	/// @brief Check if flag is set
	/// @param[in] value Tested flag
	/// @return State of checked flag @c value
	///
	template<class T, class U>
	requires kflags_enum<T, U>
	[[nodiscard]]
	constexpr auto kFlags<T, U>::isSet(const T value) const noexcept -> bool {
		// All bits of (non-empty) flag are set
		const auto flag {static_cast<U>(value)};
		return (static_cast<U>(0) != flag) && (flag == (mValue & flag));
	}


	////////////////////////////////////////////////////
	///
	/// @brief Test bit
	/// @param[in] bit Bit number [0 .. MAX]
	/// @return Bit value [false, true] calculated as (1 SHL bit) = (this AND (1 SHL bit))
	///
	template<class T, class U>
	requires kflags_enum<T, U>
	[[nodiscard]]
	constexpr auto kFlags<T, U>::test(const igros_usize_t bit) const noexcept -> bool {
		return static_cast<U>(0) != (mValue & (static_cast<U>(1) << bit));
	}


	////////////////////////////////////////////////////
	///
	/// @brief Deduction guide
	/// @tparam T Flags internal representation enum type
	///
	template<class T>
	kFlags(T) -> kFlags<T>;


	////////////////////////////////////////////////////
	///
	/// @brief Flags creation helper function from flags value
	/// @tparam T Flags internal representation enum type
	/// @param[in] flag Actual flag value
	/// @return Value of type @c kFlags<T>
	///
	template<class T>
	[[nodiscard]]
	constexpr auto make_kflags(T &&flag) noexcept -> kFlags<std::decay_t<T>> {
		return kFlags<std::decay_t<T>> {
			std::forward<T>(flag)
		};
	}

	////////////////////////////////////////////////////
	///
	/// @brief Flags creation helper function from flags
	/// @tparam T Flags internal representation enum type
	/// @tparam Args List of set flags
	/// @param[in] args List of flags
	/// @return Value of type @c kFlags<T>
	///
	template<class T, class ...Args>
	[[nodiscard]]
	constexpr auto make_kflags(Args &&...args) noexcept -> kFlags<T> {
		return kFlags<T> {
			std::in_place,
			std::forward<Args>(args)...
		};
	}


}       // namespace igros::klib

