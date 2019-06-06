////////////////////////////////////////////////////////////////
//
//	Bit flags template datatype
//
//	File:	flags.hpp
//	Date:	06 Jun 2019
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
//	All rights reserved.
//
//


#pragma once
#ifndef IGROS_ARCH_FLAGS_HPP
#define IGROS_ARCH_FLAGS_HPP


#include <type_traits>

#include <arch/types.hpp>


// Arch-dependent code zone
namespace arch {

/*
        // Flags wrapper struct
        template <typename T, typename U = typename std::enable_if<std::is_enum<T>::value, typename std::underlying_type<T>::type>::type>
	class kflags {

		U	flagField;		// Flags field


	public:

		// C-tor
		kflags() : flagField(0) {}
		// C-tor
		kflags(T flag) : flagField(flag) {}
		// Copy c-tor
		kflags(const kflags<T>& _flags) : flagField(_flags.flagField) {}

		// Convet to bool prevention
		explicit operator bool() { return flagField != 0; };

                // Bitwise OR operator for flags
		constexpr kflags<T> operator|(const T flag);
                // Bitwise AND operator for flags
		constexpr kflags<T> operator&(const T flag);
                // Bitwise XOR operator for flags
		constexpr kflags<T> operator^(const T flag);

		// Bitwise NOT operator for flags
		constexpr kflags<T> operator~();

		// Bitwise OR assignment operator for flags
		constexpr kflags<T>& operator|=(const T flag);
		// Bitwise AND assignment operator for flags
		constexpr kflags<T>& operator&=(const T flag);
		// Bitwise XOR assignment operator for flags
		constexpr kflags<T>& operator^=(const T flag);


        };
*/


	// Bitwise OR operator for flags
        template <typename T>
	typename std::enable_if<std::is_enum<T>::value, T>::type
	inline constexpr operator| (const T left, const T right) {

		// Underlying type acquisition
		using U = typename std::underlying_type<T>::type;

		// OR on underlying types
		return	static_cast<T>(
			static_cast<U>(left)	|
			static_cast<U>(right)
			);

	}

	// Bitwise AND operator for flags
        template <typename T>
	typename std::enable_if<std::is_enum<T>::value, T>::type
	inline constexpr operator& (const T left, const T right) {

		// Underlying type acquisition
		using U = typename std::underlying_type<T>::type;

		// AND on underlying types
		return	static_cast<T>(
			static_cast<U>(left)	&
			static_cast<U>(right)
			);

	}

	// Bitwise XOR operator for flags
        template <typename T>
	typename std::enable_if<std::is_enum<T>::value, T>::type
	inline constexpr operator^ (const T left, const T right) {

		// Underlying type acquisition
		using U = typename std::underlying_type<T>::type;

		// XOR on underlying types
		return	static_cast<T>(
			static_cast<U>(left)	^
			static_cast<U>(right)
			);

	}

	// Bitwise NOT operator for flags
        template <typename T>
	typename std::enable_if<std::is_enum<T>::value, T>::type
	inline constexpr operator~ (const T right) {

		// Underlying type acquisition
		using U = typename std::underlying_type<T>::type;

		// NOT on underlying types
		return	static_cast<T>(
			~static_cast<U>(right)
			);

	}

	// Bitwise OR assignment operator for flags
        template <typename T>
	typename std::enable_if<std::is_enum<T>::value, T>::type
	inline constexpr operator|= (T &left, const T right) {

		// Underlying type acquisition
		using U = typename std::underlying_type<T>::type;

		// OR on underlying types
		left =	static_cast<T>(
			static_cast<U>(left)	|
			static_cast<U>(right)
			);

	}

	// Bitwise AND assignment operator for flags
        template <typename T>
	typename std::enable_if<std::is_enum<T>::value, T>::type
	inline constexpr operator&= (T &left, const T right) {

		// Underlying type acquisition
		using U = typename std::underlying_type<T>::type;

		// AND on underlying types
		left =	static_cast<T>(
			static_cast<U>(left)	&
			static_cast<U>(right)
			);

	}

	// Bitwise XOR assignment operator for flags
        template <typename T>
	typename std::enable_if<std::is_enum<T>::value, T>::type
	inline constexpr operator^= (T &left, const T right) {

		// Underlying type acquisition
		using U = typename std::underlying_type<T>::type;

		// XOR on underlying types
		left =	static_cast<T>(
			static_cast<U>(left)	^
			static_cast<U>(right)
			);

	}


}       // namespace arch


#endif  // IGROS_ARCH_FLAGS_HPP

