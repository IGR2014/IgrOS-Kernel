////////////////////////////////////////////////////////////////
///
///	@brief		Bit flags template datatype
///
///	@file		kflags.hpp
///	@date		05 Dec 2022
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


////////////////////////////////////////////////////
///
/// @brief OS namespace
/// @namespace igros
///
namespace igros {


	////////////////////////////////////////////////////
	///
	/// @brief Bitwise flags template class
	/// @class kflags
	/// @tparam T Flags internal representation enum type
	/// @tparam U Flags internal underlying representation enum type
	///
	template<typename T, typename U = std::underlying_type_t<T>>
	requires (std::is_enum_v<T> && std::is_same_v<U, std::underlying_type_t<T>>)
	class kflags {

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
		constexpr kflags() noexcept = default;
		/// @brief From initializer list
		template<typename ...Args>
		constexpr explicit kflags(Args &&...args) noexcept; 

		/// @brief Type conversion operator
		[[nodiscard]]
		constexpr explicit operator T() const noexcept;
		/// @brief Type conversion operator
		[[nodiscard]]
		constexpr explicit operator U() const noexcept;

		/// @brief Comparison operator
		constexpr auto	operator==(const kflags<T, U> &other) const noexcept -> bool = default;

		/// @brief Comparison operator
		constexpr auto	operator==(const T &other) const noexcept -> bool;
		/// @brief Comparison operator
		constexpr auto	operator!=(const T &other) const noexcept -> bool;

		/// @brief Bitwise AND operator
		[[maybe_unused]]
		constexpr auto	operator&=(const kflags<T, U> &other) noexcept -> kflags&;
		/// @brief Bitwise OR operator
		[[maybe_unused]]
		constexpr auto	operator|=(const kflags<T, U> &other) noexcept -> kflags&;
		/// @brief Bitwise XOR operator
		[[maybe_unused]]
		constexpr auto	operator^=(const kflags<T, U> &other) noexcept -> kflags&;

		/// @brief Bitwise AND operator
		[[nodiscard]]
		constexpr auto	operator&(const kflags<T, U> &other) const noexcept -> kflags;
		/// @brief Bitwise OR operator
		[[nodiscard]]
		constexpr auto	operator|(const kflags<T, U> &other) const noexcept -> kflags;
		/// @brief Bitwise XOR operator
		[[nodiscard]]
		constexpr auto	operator^(const kflags<T, U> &other) const noexcept -> kflags;

		/// @brief Bitwise AND operator
		[[maybe_unused]]
		constexpr auto	operator&=(const T &other) noexcept -> kflags&;
		/// @brief Bitwise OR operator
		[[maybe_unused]]
		constexpr auto	operator|=(const T &other) noexcept -> kflags&;
		/// @brief Bitwise XOR operator
		[[maybe_unused]]
		constexpr auto	operator^=(const T &other) noexcept -> kflags&;

		/// @brief Bitwise AND operator
		[[nodiscard]]
		constexpr auto	operator&(const T &other) const noexcept -> kflags;
		/// @brief Bitwise OR operator
		[[nodiscard]]
		constexpr auto	operator|(const T &other) const noexcept -> kflags;
		/// @brief Bitwise XOR operator
		[[nodiscard]]
		constexpr auto	operator^(const T &other) const noexcept -> kflags;

		/// @brief Bitwise NOT operator
		[[nodiscard]]
		constexpr auto	operator~() const noexcept -> kflags;

		/// @brief Copy c-tor
		constexpr kflags(const kflags &value) noexcept = default;
		/// @brief Copy assignment
		constexpr auto	operator=(const kflags &value) noexcept -> kflags& = default;

		/// @brief Move c-tor
		constexpr kflags(kflags &&value) noexcept = default;
		/// @brief Move assignment
		constexpr auto	operator=(kflags &&value) noexcept -> kflags& = default;

		/// @brief Type copy c-tor
		constexpr explicit kflags(const T &value) noexcept;
		/// @brief Type copy assignment
		constexpr auto	operator=(const T &value) noexcept -> kflags&;

		/// @brief Type move c-tor
		constexpr explicit kflags(T &&value) noexcept;
		/// @brief Type move assignment
		constexpr auto	operator=(T &&value) noexcept -> kflags&;

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
	///
	template<typename T, typename U>
	requires (std::is_enum_v<T> && std::is_same_v<U, std::underlying_type_t<T>>)
	template<typename ...Args>
	constexpr kflags<T, U>::kflags(Args &&...args) noexcept :
		mValue((static_cast<U>(args) | ...)) {}


	////////////////////////////////////////////////////
	///
	/// @brief Type conversion operator
	/// @return Flag value of type @c T
	///
	template<typename T, typename U>
	requires (std::is_enum_v<T> && std::is_same_v<U, std::underlying_type_t<T>>)
	[[nodiscard]]
	constexpr kflags<T, U>::operator T() const noexcept {
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
	constexpr kflags<T, U>::operator U() const noexcept {
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
	constexpr auto kflags<T, U>::operator==(const T &other) const noexcept -> bool {
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
	constexpr auto kflags<T, U>::operator!=(const T &other) const noexcept -> bool {
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
	constexpr auto kflags<T, U>::operator&=(const kflags<T, U> &other) noexcept -> kflags<T, U>& {
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
	constexpr auto kflags<T, U>::operator|=(const kflags<T, U> &other) noexcept -> kflags<T, U>& {
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
	constexpr auto kflags<T, U>::operator^=(const kflags<T, U> &other) noexcept -> kflags<T, U>& {
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
	constexpr auto kflags<T, U>::operator&(const kflags<T, U> &other) const noexcept -> kflags<T, U> {
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
	constexpr auto kflags<T, U>::operator|(const kflags<T, U> &other) const noexcept -> kflags<T, U> {
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
	constexpr auto kflags<T, U>::operator^(const kflags<T, U> &other) const noexcept -> kflags<T, U> {
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
	constexpr auto kflags<T, U>::operator&=(const T &other) noexcept -> kflags<T, U>& {
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
	constexpr auto kflags<T, U>::operator|=(const T &other) noexcept -> kflags<T, U>& {
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
	constexpr auto kflags<T, U>::operator^=(const T &other) noexcept -> kflags<T, U>& {
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
	constexpr auto kflags<T, U>::operator&(const T &other) const noexcept -> kflags<T, U> {
		return kflags<T, U>(*this) &= other;
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
	constexpr auto kflags<T, U>::operator|(const T &other) const noexcept -> kflags<T, U> {
		return kflags<T, U>(*this) |= other;
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
	constexpr auto kflags<T, U>::operator^(const T &other) const noexcept -> kflags<T, U> {
		return kflags<T, U>(*this) ^= other;
	}


	////////////////////////////////////////////////////
	///
	/// @brief Bitwise NOT operator
	/// @return Result of (NOT this)
	///
	template<typename T, typename U>
	requires (std::is_enum_v<T> && std::is_same_v<U, std::underlying_type_t<T>>)
	[[nodiscard]]
	constexpr auto kflags<T, U>::operator~() const noexcept -> kflags<T, U> {
		return kflags<T, U>(static_cast<T>(~mValue));
	}


	////////////////////////////////////////////////////
	///
	/// @brief Type copy c-tor
	/// @param[in] value Other flags value of type @c T
	///
	template<typename T, typename U>
	requires (std::is_enum_v<T> && std::is_same_v<U, std::underlying_type_t<T>>)
	constexpr kflags<T, U>::kflags(const T &value) noexcept :
		mValue(value) {}

	////////////////////////////////////////////////////
	///
	/// @brief Type copy assignment
	/// @param[in] value Other flags value of type @c T
	/// @return Reference to @c this for chaining
	///
	template<typename T, typename U>
	requires (std::is_enum_v<T> && std::is_same_v<U, std::underlying_type_t<T>>)
	constexpr auto kflags<T, U>::operator=(const T &value) noexcept -> kflags<T, U>& {
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
	constexpr kflags<T, U>::kflags(T &&value) noexcept :
		mValue(std::move(static_cast<U>(value))) {}

	////////////////////////////////////////////////////
	///
	/// @brief Type move assignment
	/// @param[in] value Other flags value of type @c T
	/// @return Reference to @c this for chaining
	///
	template<typename T, typename U>
	requires (std::is_enum_v<T> && std::is_same_v<U, std::underlying_type_t<T>>)
	constexpr auto kflags<T, U>::operator=(T &&value) noexcept -> kflags<T, U>& {
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
	constexpr auto kflags<T, U>::value() const noexcept -> U {
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
	constexpr auto kflags<T, U>::isEmpty() const noexcept -> bool {
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
	constexpr auto kflags<T, U>::isSet(const T value) const noexcept -> bool {
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
	constexpr auto kflags<T, U>::test(const igros_usize_t bit) const noexcept -> bool {
		return std::has_single_bit(static_cast<std::make_unsigned_t<U>>(mValue & static_cast<U>(bit)));
	}


}       // namespace igros

