////////////////////////////////////////////////////////////////
///
///	@brief		Bit flags template datatype
///
///	@file		kFlags.hpp
///	@date		16 Mar 2023
///
///	@copyright	Copyright (c) 2017 - 2022,
///			All rights reserved.
///	@author		Igor Baklykov
///
///


#pragma once


// C++
#include <bit>
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
	/// @brief Bitwise flags template class
	/// @class kFlags
	/// @tparam T Flags internal representation enum type
	/// @tparam U Flags internal underlying representation enum type
	///
	template<typename T, typename U = std::underlying_type_t<T>>
	requires (std::is_enum_v<T> && std::is_same_v<U, std::underlying_type_t<T>>)
	class kFlags {

		static_assert(
			std::is_enum_v<T>,
			"Parameter T must be an enum!"
		);
		static_assert(
			std::is_same_v<U, std::underlying_type_t<T>>,
			"U should match underlying type of enum T!"
		);

		U	mValue;		///< Real flags value


	public:

		/// @brief Default c-tor
		constexpr kFlags() noexcept = default;
		/// @brief From initializer list
		template<typename ...Args>
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
		/// @brief Comparison operator
		constexpr auto	operator!=(const T &other) const noexcept -> bool;

		/// @brief Bitwise AND operator
		[[maybe_unused]]
		constexpr auto	operator&=(const kFlags<T, U> &other) noexcept -> kFlags&;
		/// @brief Bitwise OR operator
		[[maybe_unused]]
		constexpr auto	operator|=(const kFlags<T, U> &other) noexcept -> kFlags&;
		/// @brief Bitwise XOR operator
		[[maybe_unused]]
		constexpr auto	operator^=(const kFlags<T, U> &other) noexcept -> kFlags&;

		/// @brief Bitwise AND operator
		[[nodiscard]]
		constexpr auto	operator&(const kFlags<T, U> &other) const noexcept -> kFlags;
		/// @brief Bitwise OR operator
		[[nodiscard]]
		constexpr auto	operator|(const kFlags<T, U> &other) const noexcept -> kFlags;
		/// @brief Bitwise XOR operator
		[[nodiscard]]
		constexpr auto	operator^(const kFlags<T, U> &other) const noexcept -> kFlags;

		/// @brief Bitwise AND operator
		[[maybe_unused]]
		constexpr auto	operator&=(const T &other) noexcept -> kFlags&;
		/// @brief Bitwise OR operator
		[[maybe_unused]]
		constexpr auto	operator|=(const T &other) noexcept -> kFlags&;
		/// @brief Bitwise XOR operator
		[[maybe_unused]]
		constexpr auto	operator^=(const T &other) noexcept -> kFlags&;

		/// @brief Bitwise AND operator
		[[nodiscard]]
		constexpr auto	operator&(const T &other) const noexcept -> kFlags;
		/// @brief Bitwise OR operator
		[[nodiscard]]
		constexpr auto	operator|(const T &other) const noexcept -> kFlags;
		/// @brief Bitwise XOR operator
		[[nodiscard]]
		constexpr auto	operator^(const T &other) const noexcept -> kFlags;

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

		/// @brief Type move c-tor
		constexpr explicit kFlags(T &&value) noexcept;
		/// @brief Type move assignment
		constexpr auto	operator=(T &&value) noexcept -> kFlags&;

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
	template<typename T, typename U>
	requires (std::is_enum_v<T> && std::is_same_v<U, std::underlying_type_t<T>>)
	template<typename ...Args>
	constexpr kFlags<T, U>::kFlags(std::in_place_t /*tag*/, Args &&...args) noexcept :
		mValue((static_cast<U>(args) | ...)) {}


	////////////////////////////////////////////////////
	///
	/// @brief Type conversion operator
	/// @return Flag value of type @c T
	///
	template<typename T, typename U>
	requires (std::is_enum_v<T> && std::is_same_v<U, std::underlying_type_t<T>>)
	[[nodiscard]]
	constexpr kFlags<T, U>::operator T() const noexcept {
		return static_cast<T>(mValue);
	}

	////////////////////////////////////////////////////
	///
	/// @brief Type conversion operator
	/// @return Flag underlying value of type @c U
	///
	template<typename T, typename U>
	requires (std::is_enum_v<T> && std::is_same_v<U, std::underlying_type_t<T>>)
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
	template<typename T, typename U>
	requires (std::is_enum_v<T> && std::is_same_v<U, std::underlying_type_t<T>>)
	constexpr auto kFlags<T, U>::operator==(const T &other) const noexcept -> bool {
		return mValue == static_cast<U>(other);
	}

	////////////////////////////////////////////////////
	///
	/// @brief Comparison operator
	/// @param[in] other Other flags value of type @c T
	/// @return Indicates if flag values are not equal
	///
	template<typename T, typename U>
	requires (std::is_enum_v<T> && std::is_same_v<U, std::underlying_type_t<T>>)
	constexpr auto kFlags<T, U>::operator!=(const T &other) const noexcept -> bool {
		return !(*this == other);
	}


	////////////////////////////////////////////////////
	///
	/// @brief Bitwise AND operator
	/// @param[in] other Other flags value
	/// @return Reference to @c this for chaining
	///
	template<typename T, typename U>
	requires (std::is_enum_v<T> && std::is_same_v<U, std::underlying_type_t<T>>)
	[[maybe_unused]]
	constexpr auto kFlags<T, U>::operator&=(const kFlags<T, U> &other) noexcept -> kFlags<T, U>& {
		return *this &= static_cast<T>(other);
	}

	////////////////////////////////////////////////////
	///
	/// @brief Bitwise OR operator
	/// @param[in] other Other flags value
	/// @return Reference to @c this for chaining
	///
	template<typename T, typename U>
	requires (std::is_enum_v<T> && std::is_same_v<U, std::underlying_type_t<T>>)
	[[maybe_unused]]
	constexpr auto kFlags<T, U>::operator|=(const kFlags<T, U> &other) noexcept -> kFlags<T, U>& {
		return *this |= static_cast<T>(other);
	}

	////////////////////////////////////////////////////
	///
	/// @brief Bitwise XOR operator
	/// @param[in] other Other flags value
	/// @return Reference to @c this for chaining
	///
	template<typename T, typename U>
	requires (std::is_enum_v<T> && std::is_same_v<U, std::underlying_type_t<T>>)
	[[maybe_unused]]
	constexpr auto kFlags<T, U>::operator^=(const kFlags<T, U> &other) noexcept -> kFlags<T, U>& {
		return *this ^= static_cast<T>(other);
	}


	////////////////////////////////////////////////////
	///
	/// @brief Bitwise AND operator
	/// @param[in] other Other flags value
	/// @return Result of (this AND other)
	///
	template<typename T, typename U>
	requires (std::is_enum_v<T> && std::is_same_v<U, std::underlying_type_t<T>>)
	[[nodiscard]]
	constexpr auto kFlags<T, U>::operator&(const kFlags<T, U> &other) const noexcept -> kFlags<T, U> {
		return *this & static_cast<T>(other);
	}

	////////////////////////////////////////////////////
	///
	/// @brief Bitwise OR operator
	/// @param[in] other Other flags value
	/// @return Result of (this OR other)
	///
	template<typename T, typename U>
	requires (std::is_enum_v<T> && std::is_same_v<U, std::underlying_type_t<T>>)
	[[nodiscard]]
	constexpr auto kFlags<T, U>::operator|(const kFlags<T, U> &other) const noexcept -> kFlags<T, U> {
		return *this | static_cast<T>(other);
	}

	////////////////////////////////////////////////////
	///
	/// @brief Bitwise XOR operator
	/// @param[in] other Other flags value
	/// @return Result of (this XOR other)
	///
	template<typename T, typename U>
	requires (std::is_enum_v<T> && std::is_same_v<U, std::underlying_type_t<T>>)
	[[nodiscard]]
	constexpr auto kFlags<T, U>::operator^(const kFlags<T, U> &other) const noexcept -> kFlags<T, U> {
		return *this ^ static_cast<T>(other);
	}


	////////////////////////////////////////////////////
	///
	/// @brief Bitwise AND operator
	/// @param[in] other Other flags value of type @c T
	/// @return Reference to @c this for chaining
	///
	template<typename T, typename U>
	requires (std::is_enum_v<T> && std::is_same_v<U, std::underlying_type_t<T>>)
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
	template<typename T, typename U>
	requires (std::is_enum_v<T> && std::is_same_v<U, std::underlying_type_t<T>>)
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
	template<typename T, typename U>
	requires (std::is_enum_v<T> && std::is_same_v<U, std::underlying_type_t<T>>)
	[[maybe_unused]]
	constexpr auto kFlags<T, U>::operator^=(const T &other) noexcept -> kFlags<T, U>& {
		// Do XOR
		mValue ^= static_cast<U>(other);
		// Reference chaining
		return *this;
	}


	////////////////////////////////////////////////////
	///
	/// @brief Bitwise AND operator
	/// @param[in] other Other flags value of type @c T
	/// @return Result of (this AND other)
	///
	template<typename T, typename U>
	requires (std::is_enum_v<T> && std::is_same_v<U, std::underlying_type_t<T>>)
	[[nodiscard]]
	constexpr auto kFlags<T, U>::operator&(const T &other) const noexcept -> kFlags<T, U> {
		return kFlags<T, U>(*this) &= other;
	}

	////////////////////////////////////////////////////
	///
	/// @brief Bitwise OR operator
	/// @param[in] other Other flags value of type @c T
	/// @return Result of (this OR other)
	///
	template<typename T, typename U>
	requires (std::is_enum_v<T> && std::is_same_v<U, std::underlying_type_t<T>>)
	[[nodiscard]]
	constexpr auto kFlags<T, U>::operator|(const T &other) const noexcept -> kFlags<T, U> {
		return kFlags<T, U>(*this) |= other;
	}

	////////////////////////////////////////////////////
	///
	/// @brief Bitwise XOR operator
	/// @param[in] other Other flags value of type @c T
	/// @return Result of (this XOR other)
	///
	template<typename T, typename U>
	requires (std::is_enum_v<T> && std::is_same_v<U, std::underlying_type_t<T>>)
	[[nodiscard]]
	constexpr auto kFlags<T, U>::operator^(const T &other) const noexcept -> kFlags<T, U> {
		return kFlags<T, U>(*this) ^= other;
	}


	////////////////////////////////////////////////////
	///
	/// @brief Bitwise NOT operator
	/// @return Result of (NOT this)
	///
	template<typename T, typename U>
	requires (std::is_enum_v<T> && std::is_same_v<U, std::underlying_type_t<T>>)
	[[nodiscard]]
	constexpr auto kFlags<T, U>::operator~() const noexcept -> kFlags<T, U> {
		return kFlags<T, U>(static_cast<T>(~mValue));
	}


	////////////////////////////////////////////////////
	///
	/// @brief Type copy c-tor
	/// @param[in] value Other flags value of type @c T
	///
	template<typename T, typename U>
	requires (std::is_enum_v<T> && std::is_same_v<U, std::underlying_type_t<T>>)
	constexpr kFlags<T, U>::kFlags(const T &value) noexcept :
		mValue(static_cast<U>(value)) {}

	////////////////////////////////////////////////////
	///
	/// @brief Type copy assignment
	/// @param[in] value Other flags value of type @c T
	/// @return Reference to @c this for chaining
	///
	template<typename T, typename U>
	requires (std::is_enum_v<T> && std::is_same_v<U, std::underlying_type_t<T>>)
	constexpr auto kFlags<T, U>::operator=(const T &value) noexcept -> kFlags<T, U>& {
		// Set new value
		mValue = value;
		// Reference chaining
		return *this;
	}


	////////////////////////////////////////////////////
	///
	/// @brief Type move c-tor
	/// @param[in] value Other flags value of type @c T
	///
	template<typename T, typename U>
	requires (std::is_enum_v<T> && std::is_same_v<U, std::underlying_type_t<T>>)
	constexpr kFlags<T, U>::kFlags(T &&value) noexcept :
		mValue(std::move(static_cast<U>(value))) {}

	////////////////////////////////////////////////////
	///
	/// @brief Type move assignment
	/// @param[in] value Other flags value of type @c T
	/// @return Reference to @c this for chaining
	///
	template<typename T, typename U>
	requires (std::is_enum_v<T> && std::is_same_v<U, std::underlying_type_t<T>>)
	constexpr auto kFlags<T, U>::operator=(T &&value) noexcept -> kFlags<T, U>& {
		// Set new value
		mValue = std::move(value);
		// Reference chaining
		return *this;
	}


	////////////////////////////////////////////////////
	///
	/// @brief Get value
	/// @return Flags underlying value of type @c U
	///
	template<typename T, typename U>
	requires (std::is_enum_v<T> && std::is_same_v<U, std::underlying_type_t<T>>)
	[[nodiscard]]
	constexpr auto kFlags<T, U>::value() const noexcept -> U {
		return static_cast<U>(*this);
	}


	////////////////////////////////////////////////////
	///
	/// @brief Check if empty
	/// @return State of checked flag @c value
	///
	template<typename T, typename U>
	requires (std::is_enum_v<T> && std::is_same_v<U, std::underlying_type_t<T>>)
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
	template<typename T, typename U>
	requires (std::is_enum_v<T> && std::is_same_v<U, std::underlying_type_t<T>>)
	[[nodiscard]]
	constexpr auto kFlags<T, U>::isSet(const T value) const noexcept -> bool {
		return std::has_single_bit(static_cast<std::make_unsigned_t<U>>(mValue & static_cast<U>(value)));
	}


	////////////////////////////////////////////////////
	///
	/// @brief Test bit
	/// @param[in] bit Bit number [0 .. MAX]
	/// @return Bit value [false, true] calculated as (1 SHL bit) = (this AND (1 SHL bit))
	///
	template<typename T, typename U>
	requires (std::is_enum_v<T> && std::is_same_v<U, std::underlying_type_t<T>>)
	[[nodiscard]]
	constexpr auto kFlags<T, U>::test(const igros_usize_t bit) const noexcept -> bool {
		return std::has_single_bit(static_cast<std::make_unsigned_t<U>>(mValue & static_cast<U>(1_usize) << bit));
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

