////////////////////////////////////////////////////////////////
//
//	Common C++ types redefinition
//
//	File:	types.hpp
//	Date:	05 Dec 2022
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// C++
#include <cstddef>
#include <cstdint>
#include <type_traits>


// OS namespace
namespace igros {


	//////////////////////////////////////
	///
	/// Type groups
	/// @addtogroup Types
	/// @{

	//////////////////////////////////////
	///
	/// Signed integers
	///
	using	igros_sbyte_t		= std::int8_t;
	using	igros_sword_t		= std::int16_t;
	using	igros_sdword_t		= std::int32_t;
	using	igros_squad_t		= std::int64_t;

	//////////////////////////////////////
	///
	/// Unsigned integers
	///
	using	igros_byte_t		= std::uint8_t;
	using	igros_word_t		= std::uint16_t;
	using	igros_dword_t		= std::uint32_t;
	using	igros_quad_t		= std::uint64_t;

	//////////////////////////////////////
	///
	/// Floating-point reals
	///
	using	igros_real32_t		= float;
	using	igros_real64_t		= double;

	// Size types
	using	igros_usize_t		= std::size_t;
	using	igros_ssize_t		= std::ptrdiff_t;

	// Common pointer type
	using	igros_pointer_t		= std::add_pointer_t<void>;


	/// @}


	//////////////////////////////////////
	///
	/// Project literals
	/// @addtogroup Literals
	/// @{

	//////////////////////////////////////
	///
	/// @brief Literal for @c igros_sbyte_t creation
	/// @param[in] value Integral value
	/// @return @c igros_sbyte_t value
	/// @see @c igros_sbyte_t
	///
	[[nodiscard]]
	constexpr auto operator""_i8(const unsigned long long value) noexcept -> igros_sbyte_t {
		// Return value
		return static_cast<igros_sbyte_t>(value);
	}

	//////////////////////////////////////
	///
	/// @brief Literal for @c igros_sword_t creation
	/// @param[in] value Integral value
	/// @return @c igros_sword_t value
	/// @see @c igros_sword_t
	///
	[[nodiscard]]
	constexpr auto operator""_i16(const unsigned long long value) noexcept -> igros_sword_t {
		// Return value
		return static_cast<igros_sword_t>(value);
	}

	//////////////////////////////////////
	///
	/// @brief Literal for @c igros_sdword_t creation
	/// @param[in] value Integral value
	/// @return @c igros_sdword_t value
	/// @see @c igros_sdword_t
	///
	[[nodiscard]]
	constexpr auto operator""_i32(const unsigned long long value) noexcept -> igros_sdword_t {
		// Return value
		return static_cast<igros_sdword_t>(value);
	}

	//////////////////////////////////////
	///
	/// @brief Literal for @c igros_squad_t creation
	/// @param[in] value Integral value
	/// @return @c igros_squad_t value
	/// @see @c igros_squad_t
	///
	[[nodiscard]]
	constexpr auto operator""_i64(const unsigned long long value) noexcept -> igros_squad_t {
		// Return value
		return static_cast<igros_squad_t>(value);
	}


	//////////////////////////////////////
	///
	/// @brief Literal for @c igros_byte_t creation
	/// @param[in] value Integral value
	/// @return @c igros_byte_t value
	/// @see @c igros_byte_t
	///
	[[nodiscard]]
	constexpr auto operator""_u8(const unsigned long long value) noexcept -> igros_byte_t {
		// Return value
		return static_cast<igros_byte_t>(value);
	}

	//////////////////////////////////////
	///
	/// @brief Literal for @c igros_word_t creation
	/// @param[in] value Integral value
	/// @return @c igros_word_t value
	/// @see @c igros_word_t
	///
	[[nodiscard]]
	constexpr auto operator""_u16(const unsigned long long value) noexcept -> igros_word_t {
		// Return value
		return static_cast<igros_word_t>(value);
	}

	//////////////////////////////////////
	///
	/// @brief Literal for @c igros_dword_t creation
	/// @param[in] value Integral value
	/// @return @c igros_dword_t value
	/// @see @c igros_dword_t
	///
	[[nodiscard]]
	constexpr auto operator""_u32(const unsigned long long value) noexcept -> igros_dword_t {
		// Return value
		return static_cast<igros_dword_t>(value);
	}

	//////////////////////////////////////
	///
	/// @brief Literal for @c igros_quad_t creation
	/// @param[in] value Integral value
	/// @return @c igros_quad_t value
	/// @see @c igros_quad_t
	///
	[[nodiscard]]
	constexpr auto operator""_u64(const unsigned long long value) noexcept -> igros_quad_t {
		// Return value
		return static_cast<igros_quad_t>(value);
	}


	//////////////////////////////////////
	///
	/// @brief Literal for @c igros_real32_t creation
	/// @param[in] value Integral value
	/// @return @c igros_real32_t value
	/// @see @c igros_real32_t
	///
	[[nodiscard]]
	constexpr auto operator""_f32(const unsigned long long value) noexcept -> igros_real32_t {
		// Return value
		return static_cast<igros_real32_t>(value);
	}

	//////////////////////////////////////
	///
	/// @brief Literal for @c igros_real32_t creation
	/// @param[in] value Floating-point value
	/// @return @c igros_real32_t value
	/// @see @c igros_real32_t
	///
	[[nodiscard]]
	constexpr auto operator""_f32(const long double value) noexcept -> igros_real32_t {
		// Return value
		return static_cast<igros_real32_t>(value);
	}

	//////////////////////////////////////
	///
	/// @brief Literal for @c igros_real64_t creation
	/// @param[in] value Integral value
	/// @return @c igros_real64_t value
	/// @see @c igros_real64_t
	///
	[[nodiscard]]
	constexpr auto operator""_f64(const unsigned long long value) noexcept -> igros_real64_t {
		// Return value
		return static_cast<igros_real64_t>(value);
	}

	//////////////////////////////////////
	///
	/// @brief Literal for @c igros_real64_t creation
	/// @param[in] value Floating-point value
	/// @return @c igros_real64_t value
	/// @see @c igros_real64_t
	///
	[[nodiscard]]
	constexpr auto operator""_f64(const long double value) noexcept -> igros_real64_t {
		// Return value
		return static_cast<igros_real64_t>(value);
	}


	//////////////////////////////////////
	///
	/// @brief Literal for @c igros_usize_t creation
	/// @param[in] value Integral value
	/// @return @c igros_usize_t value
	/// @see @c igros_usize_t
	///
	[[nodiscard]]
	constexpr auto operator""_usize(const unsigned long long value) noexcept -> igros_usize_t {
		// Return value
		return static_cast<igros_usize_t>(value);
	}

	//////////////////////////////////////
	///
	/// @brief Literal for @c igros_ssize_t creation
	/// @param[in] value Integral value
	/// @return @c igros_ssize_t value
	/// @see @c igros_ssize_t
	///
	[[nodiscard]]
	constexpr auto operator""_ssize(const unsigned long long value) noexcept -> igros_ssize_t {
		// Return value
		return static_cast<igros_ssize_t>(value);
	}

	/// @}


}	// namespace igros

