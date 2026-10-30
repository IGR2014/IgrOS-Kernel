////////////////////////////////////////////////////////////////
//
//	Common C++ types redefinition
//
//	File:	types.hpp
//	Date:	30 Oct 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// C++
#include <array>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <utility>


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

	/// @brief Integer literals parsing (literal operator templates get literal characters,
	/// so value range is checked by static_assert at compile time)
	namespace literal {

		/// @brief Parsed integer literal
		struct value_t final {
			unsigned long long	value;		///< Literal value
			bool			isInteger;	///< Integer literal (floating-point literals are not)
			bool			isOverflow;	///< Value doesn't fit unsigned long long
		};

		//////////////////////////////////////
		///
		/// @brief Parse integer literal (decimal, octal, 0x hex, 0b binary, ' digit separators)
		/// @tparam DIGITS Literal characters
		/// @return Parsed literal
		///
		template<char... DIGITS>
		[[nodiscard]]
		consteval auto parse() noexcept -> value_t {
			constexpr auto TEXT {std::array<char, sizeof...(DIGITS)> {DIGITS...}};
			auto result	{value_t {0ULL, true, false}};
			// Base prefix
			auto base	{10ULL};
			auto first	{std::size_t {0}};
			if ((TEXT.size() > 1) && ('0' == TEXT[0])) {
				if (('x' == TEXT[1]) || ('X' == TEXT[1])) {
					base	= 16ULL;
					first	= 2;
				} else if (('b' == TEXT[1]) || ('B' == TEXT[1])) {
					base	= 2ULL;
					first	= 2;
				} else {
					base	= 8ULL;
					first	= 1;
				}
			}
			// Digits
			for (auto i {first}; i < TEXT.size(); ++i) {
				const auto symbol {TEXT[i]};
				if ('\'' == symbol) {
					continue;
				}
				auto digit {base};
				if (('0' <= symbol) && ('9' >= symbol)) {
					digit = static_cast<unsigned long long>(symbol - '0');
				} else if (('a' <= symbol) && ('f' >= symbol)) {
					digit = static_cast<unsigned long long>(symbol - 'a') + 10ULL;
				} else if (('A' <= symbol) && ('F' >= symbol)) {
					digit = static_cast<unsigned long long>(symbol - 'A') + 10ULL;
				}
				// Not a digit of base: floating-point literal ('.', exponent)
				if (digit >= base) {
					result.isInteger = false;
					return result;
				}
				if (result.value > ((std::numeric_limits<unsigned long long>::max() - digit) / base)) {
					result.isOverflow = true;
					return result;
				}
				result.value = result.value * base + digit;
			}
			return result;
		}

		//////////////////////////////////////
		///
		/// @brief Integer literal of given type
		/// @tparam T Integer type
		/// @tparam DIGITS Literal characters
		/// @return Literal value (compile error if it isn't integer or doesn't fit @c T)
		///
		template<class T, char... DIGITS>
		[[nodiscard]]
		consteval auto make() noexcept -> T {
			constexpr auto LITERAL {literal::parse<DIGITS...>()};
			static_assert(LITERAL.isInteger, "IgrOS: integer literal suffix used with floating-point literal");
			static_assert(!LITERAL.isOverflow && std::in_range<T>(LITERAL.value), "IgrOS: integer literal doesn't fit its type");
			return static_cast<T>(LITERAL.value);
		}

	}	// namespace literal

	//////////////////////////////////////
	///
	/// @brief Literal for @c igros_sbyte_t creation
	/// @tparam DIGITS Literal characters
	/// @return @c igros_sbyte_t value (compile error if literal doesn't fit)
	/// @see @c igros_sbyte_t
	///
	template<char... DIGITS>
	[[nodiscard]]
	consteval auto operator""_i8() noexcept -> igros_sbyte_t {
		// Checked value
		return literal::make<igros_sbyte_t, DIGITS...>();
	}

	//////////////////////////////////////
	///
	/// @brief Literal for @c igros_sword_t creation
	/// @tparam DIGITS Literal characters
	/// @return @c igros_sword_t value (compile error if literal doesn't fit)
	/// @see @c igros_sword_t
	///
	template<char... DIGITS>
	[[nodiscard]]
	consteval auto operator""_i16() noexcept -> igros_sword_t {
		// Checked value
		return literal::make<igros_sword_t, DIGITS...>();
	}

	//////////////////////////////////////
	///
	/// @brief Literal for @c igros_sdword_t creation
	/// @tparam DIGITS Literal characters
	/// @return @c igros_sdword_t value (compile error if literal doesn't fit)
	/// @see @c igros_sdword_t
	///
	template<char... DIGITS>
	[[nodiscard]]
	consteval auto operator""_i32() noexcept -> igros_sdword_t {
		// Checked value
		return literal::make<igros_sdword_t, DIGITS...>();
	}

	//////////////////////////////////////
	///
	/// @brief Literal for @c igros_squad_t creation
	/// @tparam DIGITS Literal characters
	/// @return @c igros_squad_t value (compile error if literal doesn't fit)
	/// @see @c igros_squad_t
	///
	template<char... DIGITS>
	[[nodiscard]]
	consteval auto operator""_i64() noexcept -> igros_squad_t {
		// Checked value
		return literal::make<igros_squad_t, DIGITS...>();
	}


	//////////////////////////////////////
	///
	/// @brief Literal for @c igros_byte_t creation
	/// @tparam DIGITS Literal characters
	/// @return @c igros_byte_t value (compile error if literal doesn't fit)
	/// @see @c igros_byte_t
	///
	template<char... DIGITS>
	[[nodiscard]]
	consteval auto operator""_u8() noexcept -> igros_byte_t {
		// Checked value
		return literal::make<igros_byte_t, DIGITS...>();
	}

	//////////////////////////////////////
	///
	/// @brief Literal for @c igros_word_t creation
	/// @tparam DIGITS Literal characters
	/// @return @c igros_word_t value (compile error if literal doesn't fit)
	/// @see @c igros_word_t
	///
	template<char... DIGITS>
	[[nodiscard]]
	consteval auto operator""_u16() noexcept -> igros_word_t {
		// Checked value
		return literal::make<igros_word_t, DIGITS...>();
	}

	//////////////////////////////////////
	///
	/// @brief Literal for @c igros_dword_t creation
	/// @tparam DIGITS Literal characters
	/// @return @c igros_dword_t value (compile error if literal doesn't fit)
	/// @see @c igros_dword_t
	///
	template<char... DIGITS>
	[[nodiscard]]
	consteval auto operator""_u32() noexcept -> igros_dword_t {
		// Checked value
		return literal::make<igros_dword_t, DIGITS...>();
	}

	//////////////////////////////////////
	///
	/// @brief Literal for @c igros_quad_t creation
	/// @tparam DIGITS Literal characters
	/// @return @c igros_quad_t value (compile error if literal doesn't fit)
	/// @see @c igros_quad_t
	///
	template<char... DIGITS>
	[[nodiscard]]
	consteval auto operator""_u64() noexcept -> igros_quad_t {
		// Checked value
		return literal::make<igros_quad_t, DIGITS...>();
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
	/// @tparam DIGITS Literal characters
	/// @return @c igros_usize_t value (compile error if literal doesn't fit)
	/// @see @c igros_usize_t
	///
	template<char... DIGITS>
	[[nodiscard]]
	consteval auto operator""_usize() noexcept -> igros_usize_t {
		// Checked value
		return literal::make<igros_usize_t, DIGITS...>();
	}

	//////////////////////////////////////
	///
	/// @brief Literal for @c igros_ssize_t creation
	/// @tparam DIGITS Literal characters
	/// @return @c igros_ssize_t value (compile error if literal doesn't fit)
	/// @see @c igros_ssize_t
	///
	template<char... DIGITS>
	[[nodiscard]]
	consteval auto operator""_ssize() noexcept -> igros_ssize_t {
		// Checked value
		return literal::make<igros_ssize_t, DIGITS...>();
	}

	/// @}


}	// namespace igros

