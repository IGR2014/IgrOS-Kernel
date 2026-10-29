////////////////////////////////////////////////////////////////
//
//	Virtual address type
//
//	File:	virt.hpp
//	Date:	29 Oct 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// C++
#include <bit>
// IgrOS-Kernel arch
#include <arch/types.hpp>


// Memory code zone
namespace igros::mem {


	// Virtual address: translated by paging, never dereferenced through this type.
	// Distinct type, so physical addresses (phys_t), plain integers and virtual
	// addresses can't be mixed up (phys_t and igros_usize_t are the same type on x86_64)
	enum class virt_t : igros_usize_t {};


	// Virtual address of object
	template<class T>
	[[nodiscard]]
	inline auto virtOf(const T* const object) noexcept -> virt_t {
		return static_cast<virt_t>(std::bit_cast<igros_usize_t>(object));
	}

	// Numeric value of virtual address
	[[nodiscard]]
	constexpr auto address(const virt_t virt) noexcept -> igros_usize_t {
		return static_cast<igros_usize_t>(virt);
	}

	// Virtual address at offset
	[[nodiscard]]
	constexpr auto operator+(const virt_t virt, const igros_usize_t offset) noexcept -> virt_t {
		return static_cast<virt_t>(address(virt) + offset);
	}


}	// namespace igros::mem
