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
#include <concepts>
#include <type_traits>


// Kernel library code zone
namespace igros::klib {


	// Bit flags of enum type
	template<class T>
	requires std::is_enum_v<T>
	class kFlags final {

		// Underlying type of flags
		using U = std::underlying_type_t<T>;

		U	mValue;		// Flags value


	public:

		// No flags set
		constexpr kFlags() noexcept = default;
		// From set flags
		template<std::same_as<T> ...Args>
		constexpr explicit kFlags(const T flag, const Args ...flags) noexcept :
			mValue((static_cast<U>(flag) | ... | static_cast<U>(flags))) {}

		// From raw value (hardware register, table entry)
		[[nodiscard]]
		constexpr static auto	from(const U value) noexcept -> kFlags {
			auto flags {kFlags {}};
			flags.mValue = value;
			return flags;
		}

		// Get value
		[[nodiscard]]
		constexpr auto	value() const noexcept -> U {
			return mValue;
		}

		// Check if flag is set (all bits of non-empty flag)
		[[nodiscard]]
		constexpr auto	isSet(const T flag) const noexcept -> bool {
			const auto bits {static_cast<U>(flag)};
			return (static_cast<U>(0) != bits) && (bits == (mValue & bits));
		}

	};


	// Flags creation helper function from set flags
	template<class T, std::same_as<T> ...Args>
	[[nodiscard]]
	constexpr auto make_kflags(const Args ...flags) noexcept -> kFlags<T> {
		return kFlags<T> {flags...};
	}


}	// namespace igros::klib

