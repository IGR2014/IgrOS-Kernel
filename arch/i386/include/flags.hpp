////////////////////////////////////////////////////////////////
//
//	Bit flags template datatype
//
//	File:	flags.hpp
//	Date:	13 Aug. 2018
//
//	Copyright (c) 2018, Igor Baklykov
//	All rights reserved.
//


#ifndef IGROS_ARCH_FLAGS_HPP
#define IGROS_ARCH_FLAGS_HPP


#include <type_traits>

#include <include/types.hpp>


// Arch-dependent code zone
namespace arch {


	/*
        // Flaggs wrapper struct
        template <typename T, typename = typename std::enable_if<std::is_enum<T>::value>::type>
        struct flags_t {


                // Bitwise OR operator for flags
                T operator|(T l, T r);
                // Bitwise AND operator for flags
                T operator&(T l, T r);
                // Bitwise XOR operator for flags
                T operator^(T l, T r);

		// Bitwise NOT operator for flags
		T operator~(T l);

		// Bitwise OR assignment operator for flags
		T operator|=(T &l, T r);
		// Bitwise AND assignment operator for flags
		T operator&=(T &l, T r);
		// Bitwise XOR assignment operator for flags
		T operator^=(T &l, T r);


        };
	*/


	// Bitwise OR operator for flags
        template <typename T>
	T operator|(const T l, const T r) {

		// Underlying type acquisition
		using U = typename std::underlying_type<T>::type;

		// OR on underlying types
		return	static_cast<T>(
			static_cast<U>(l) |
			static_cast<U>(r)
			);

	}

	// Bitwise AND operator for flags
        template <typename T>
	T operator&(const T l, const T r) {

		// Underlying type acquisition
		using U = typename std::underlying_type<T>::type;

		// AND on underlying types
		return	static_cast<T>(
			static_cast<U>(l) &
			static_cast<U>(r)
			);

	}

	// Bitwise XOR operator for flags
        template <typename T>
	T operator^(const T l, const T r) {

		// Underlying type acquisition
		using U = typename std::underlying_type<T>::type;

		// XOR on underlying types
		return	static_cast<T>(
			static_cast<U>(l) ^
			static_cast<U>(r)
			);

	}

	// Bitwise NOT operator for flags
        template <typename T>
	T operator~(const T l) {

		// Underlying type acquisition
		using U = typename std::underlying_type<T>::type;

		// NOT on underlying types
		return	static_cast<T>(
			~static_cast<U>(l)
			);

	}

	// Bitwise OR assignment operator for flags
        template <typename T>
	T operator|=(T &l, const T r) {

		// Underlying type acquisition
		using U = typename std::underlying_type<T>::type;

		// OR on underlying types
		l =	static_cast<T>(
			static_cast<U>(l) |
			static_cast<U>(r)
			);

	}

	// Bitwise AND assignment operator for flags
        template <typename T>
	T operator&=(T &l, const T r) {

		// Underlying type acquisition
		using U = typename std::underlying_type<T>::type;

		// AND on underlying types
		l =	static_cast<T>(
			static_cast<U>(l) &
			static_cast<U>(r)
			);

	}

	// Bitwise XOR assignment operator for flags
        template <typename T>
	T operator^=(T &l, const T r) {

		// Underlying type acquisition
		using U = typename std::underlying_type<T>::type;

		// XOR on underlying types
		l =	static_cast<T>(
			static_cast<U>(l) ^
			static_cast<U>(r)
			);

	}


}       // namespace arch


#endif  // IGROS_ARCH_FLAGS_HPP

