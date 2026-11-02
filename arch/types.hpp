////////////////////////////////////////////////////////////////
///
///	@file
///	@brief		Common C++ types redefinition
///	@date		30 Oct 2026
///
///	@copyright	Copyright (c) 2017 - 2026, Igor Baklykov
///			All rights reserved.
///	@author		Igor Baklykov
///


#pragma once


// C++
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <type_traits>
#include <utility>


// OS namespace
namespace igros {


	//////////////////////////////////////
	///
	/// Type groups
	/// @addtogroup Types
	/// @{

	// Signed integers
	using	igros_sbyte_t		= std::int8_t;	///< 8-bit signed integer
	using	igros_sword_t		= std::int16_t;	///< 16-bit signed integer
	using	igros_sdword_t		= std::int32_t;	///< 32-bit signed integer
	using	igros_squad_t		= std::int64_t;	///< 64-bit signed integer

	// Unsigned integers
	using	igros_byte_t		= std::uint8_t;	///< 8-bit unsigned integer
	using	igros_word_t		= std::uint16_t;	///< 16-bit unsigned integer
	using	igros_dword_t		= std::uint32_t;	///< 32-bit unsigned integer
	using	igros_quad_t		= std::uint64_t;	///< 64-bit unsigned integer

	// Floating-point reals
	using	igros_real32_t		= float;	///< 32-bit floating point
	using	igros_real64_t		= double;	///< 64-bit floating point

	// Size types
	using	igros_usize_t		= std::size_t;	///< Unsigned size
	using	igros_ssize_t		= std::ptrdiff_t;	///< Signed size (difference)

	/// @brief Common pointer type
	using	igros_pointer_t		= std::add_pointer_t<void>;


	/// @}


	//////////////////////////////////////
	///
	/// Project literals
	/// @addtogroup Literals
	/// @{

	// Literals are evaluated by compiler only: no runtime code to cover
	// GCOVR_EXCL_START

	/// @brief Integer literals range check (operators are consteval, so a call
	/// to non-constexpr std::abort for out of range value is a compile error)
	namespace literal {

		//////////////////////////////////////
		///
		/// @brief Integer literal of given type
		/// @tparam T Integer type
		/// @param[in] value Literal value
		/// @return @c value as @c T (compile error if it doesn't fit @c T)
		///
		template<class T>
		[[nodiscard]]
		consteval auto make(const unsigned long long value) noexcept -> T {
			if (!std::in_range<T>(value)) {
				std::abort();	// IgrOS: integer literal doesn't fit its type
			}
			return static_cast<T>(value);
		}

	}	// namespace literal

	//////////////////////////////////////
	///
	/// @brief Literal for @c igros_sbyte_t creation
	/// @param[in] value Integral value
	/// @return @c igros_sbyte_t value (compile error if literal doesn't fit)
	/// @see @c igros_sbyte_t
	///
	[[nodiscard]]
	consteval auto operator""_i8(const unsigned long long value) noexcept -> igros_sbyte_t {
		// Checked value
		return literal::make<igros_sbyte_t>(value);
	}

	//////////////////////////////////////
	///
	/// @brief Literal for @c igros_sword_t creation
	/// @param[in] value Integral value
	/// @return @c igros_sword_t value (compile error if literal doesn't fit)
	/// @see @c igros_sword_t
	///
	[[nodiscard]]
	consteval auto operator""_i16(const unsigned long long value) noexcept -> igros_sword_t {
		// Checked value
		return literal::make<igros_sword_t>(value);
	}

	//////////////////////////////////////
	///
	/// @brief Literal for @c igros_sdword_t creation
	/// @param[in] value Integral value
	/// @return @c igros_sdword_t value (compile error if literal doesn't fit)
	/// @see @c igros_sdword_t
	///
	[[nodiscard]]
	consteval auto operator""_i32(const unsigned long long value) noexcept -> igros_sdword_t {
		// Checked value
		return literal::make<igros_sdword_t>(value);
	}

	//////////////////////////////////////
	///
	/// @brief Literal for @c igros_squad_t creation
	/// @param[in] value Integral value
	/// @return @c igros_squad_t value (compile error if literal doesn't fit)
	/// @see @c igros_squad_t
	///
	[[nodiscard]]
	consteval auto operator""_i64(const unsigned long long value) noexcept -> igros_squad_t {
		// Checked value
		return literal::make<igros_squad_t>(value);
	}


	//////////////////////////////////////
	///
	/// @brief Literal for @c igros_byte_t creation
	/// @param[in] value Integral value
	/// @return @c igros_byte_t value (compile error if literal doesn't fit)
	/// @see @c igros_byte_t
	///
	[[nodiscard]]
	consteval auto operator""_u8(const unsigned long long value) noexcept -> igros_byte_t {
		// Checked value
		return literal::make<igros_byte_t>(value);
	}

	//////////////////////////////////////
	///
	/// @brief Literal for @c igros_word_t creation
	/// @param[in] value Integral value
	/// @return @c igros_word_t value (compile error if literal doesn't fit)
	/// @see @c igros_word_t
	///
	[[nodiscard]]
	consteval auto operator""_u16(const unsigned long long value) noexcept -> igros_word_t {
		// Checked value
		return literal::make<igros_word_t>(value);
	}

	//////////////////////////////////////
	///
	/// @brief Literal for @c igros_dword_t creation
	/// @param[in] value Integral value
	/// @return @c igros_dword_t value (compile error if literal doesn't fit)
	/// @see @c igros_dword_t
	///
	[[nodiscard]]
	consteval auto operator""_u32(const unsigned long long value) noexcept -> igros_dword_t {
		// Checked value
		return literal::make<igros_dword_t>(value);
	}

	//////////////////////////////////////
	///
	/// @brief Literal for @c igros_quad_t creation
	/// @param[in] value Integral value
	/// @return @c igros_quad_t value (compile error if literal doesn't fit)
	/// @see @c igros_quad_t
	///
	[[nodiscard]]
	consteval auto operator""_u64(const unsigned long long value) noexcept -> igros_quad_t {
		// Checked value
		return literal::make<igros_quad_t>(value);
	}


	//////////////////////////////////////
	///
	/// @brief Literal for @c igros_real32_t creation
	/// @param[in] value Integral value
	/// @return @c igros_real32_t value
	/// @see @c igros_real32_t
	///
	[[nodiscard]]
	consteval auto operator""_f32(const unsigned long long value) noexcept -> igros_real32_t {
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
	consteval auto operator""_f32(const long double value) noexcept -> igros_real32_t {
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
	consteval auto operator""_f64(const unsigned long long value) noexcept -> igros_real64_t {
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
	consteval auto operator""_f64(const long double value) noexcept -> igros_real64_t {
		// Return value
		return static_cast<igros_real64_t>(value);
	}


	//////////////////////////////////////
	///
	/// @brief Literal for @c igros_usize_t creation
	/// @param[in] value Integral value
	/// @return @c igros_usize_t value (compile error if literal doesn't fit)
	/// @see @c igros_usize_t
	///
	[[nodiscard]]
	consteval auto operator""_usize(const unsigned long long value) noexcept -> igros_usize_t {
		// Checked value
		return literal::make<igros_usize_t>(value);
	}

	//////////////////////////////////////
	///
	/// @brief Literal for @c igros_ssize_t creation
	/// @param[in] value Integral value
	/// @return @c igros_ssize_t value (compile error if literal doesn't fit)
	/// @see @c igros_ssize_t
	///
	[[nodiscard]]
	consteval auto operator""_ssize(const unsigned long long value) noexcept -> igros_ssize_t {
		// Checked value
		return literal::make<igros_ssize_t>(value);
	}

	// GCOVR_EXCL_STOP

	/// @}


}	// namespace igros

