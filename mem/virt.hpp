////////////////////////////////////////////////////////////////
///
///	@file
///	@brief		Virtual address type
///	@date		29 Oct 2026
///
///	@copyright	Copyright (c) 2017 - 2026, Igor Baklykov
///			All rights reserved.
///	@author		Igor Baklykov
///


#pragma once


// C++
#include <bit>
// IgrOS-Kernel arch
#include <arch/types.hpp>


// Memory code zone
namespace igros::mem {


	/// @brief Virtual address: translated by paging, never dereferenced through this type.
	///
	/// Distinct type, so physical addresses (phys_t), plain integers and virtual
	/// addresses can't be mixed up (phys_t and igros_usize_t are the same type on x86_64)
	enum class virt_t : igros_usize_t {};


	/// @brief Virtual address of object
	/// @tparam T Object type
	/// @param object Object
	/// @return Virtual address
	template<class T>
	[[nodiscard]]
	inline auto virtOf(const T* const object) noexcept -> virt_t {
		return static_cast<virt_t>(std::bit_cast<igros_usize_t>(object));
	}

	/// @brief Numeric value of virtual address
	/// @param virt Virtual address
	/// @return Address value
	[[nodiscard]]
	constexpr auto address(const virt_t virt) noexcept -> igros_usize_t {
		return static_cast<igros_usize_t>(virt);
	}

	/// @brief Virtual address at offset
	/// @param virt Virtual address
	/// @param offset Offset in bytes
	/// @return Virtual address at offset
	[[nodiscard]]
	constexpr auto operator+(const virt_t virt, const igros_usize_t offset) noexcept -> virt_t {
		return static_cast<virt_t>(address(virt) + offset);
	}


}	// namespace igros::mem
