////////////////////////////////////////////////////////////////
///
///	@file
///	@brief		Bit flags template datatype
///	@date		03 Nov 2026
///
///	@copyright	Copyright (c) 2017 - 2026, Igor Baklykov
///			All rights reserved.
///	@author		Igor Baklykov
///


#pragma once


// C++
#include <concepts>
#include <type_traits>


// Kernel library code zone
namespace igros::klib {


	/// @brief Bit flags of enum type
	/// @tparam T Flags enum type (bit values)
	template<class T>
	requires std::is_enum_v<T>
	class kFlags final {

		/// @brief Underlying type of flags
		using U = std::underlying_type_t<T>;
		/// @brief Type of bit operations (at least unsigned int: narrower types would be promoted to int)
		using W = std::common_type_t<U, unsigned int>;

		U mValue; ///< Flags value


	public:

		/// @brief No flags set
		constexpr kFlags() noexcept = default;
		/// @brief From set flags
		/// @tparam Args Further flags (same enum type)
		/// @param flag First set flag
		/// @param flags Further set flags
		template<std::same_as<T>... Args>
		constexpr explicit kFlags(const T flag, const Args... flags) noexcept :
			mValue(static_cast<U>((static_cast<W>(flag) | ... | static_cast<W>(flags)))) {}

		/// @brief From raw value (hardware register, table entry)
		/// @param value Raw value (unknown bits are kept)
		/// @return Flags with given value
		[[nodiscard]]
		constexpr static auto from(const U value) noexcept -> kFlags {
			auto flags {kFlags {}};
			flags.mValue = value;
			return flags;
		}

		/// @brief Get value
		/// @return Underlying value of flags
		[[nodiscard]]
		constexpr auto value() const noexcept -> U {
			return mValue;
		}

		/// @brief Check if flag is set (all bits of non-empty flag)
		/// @param flag Tested flag (may have several bits)
		/// @return true if all bits of flag are set (false for empty flag)
		[[nodiscard]]
		constexpr auto isSet(const T flag) const noexcept -> bool {
			const auto bits {static_cast<W>(flag)};
			return (0U != bits) && (bits == (static_cast<W>(mValue) & bits));
		}

		/// @brief Set flag (read-modify-write of hardware registers)
		/// @param flag Set flag (may have several bits)
		/// @return Flags with all bits of flag set (other bits are kept)
		[[nodiscard]]
		constexpr auto set(const T flag) const noexcept -> kFlags {
			return kFlags::from(static_cast<U>(static_cast<W>(mValue) | static_cast<W>(flag)));
		}
	};


	/// @brief Flags creation helper function from set flags
	/// @tparam T Flags enum type
	/// @tparam Args Set flags types (same enum type)
	/// @param flags Set flags
	/// @return Flags with given flags set
	template<class T, std::same_as<T>... Args>
	[[nodiscard]]
	constexpr auto make_kflags(const Args... flags) noexcept -> kFlags<T> {
		return kFlags<T> {flags...};
	}


} // namespace igros::klib

