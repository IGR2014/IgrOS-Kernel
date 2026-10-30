////////////////////////////////////////////////////////////////
//
//	Direct map of physical memory
//
//	File:	direct.hpp
//	Date:	30 Oct 2026
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
// IgrOS-Kernel memory
#include <mem/phys.hpp>
// IgrOS-Kernel platform
#include <platform/platform.hpp>


// Memory code zone
namespace igros::mem {


	// Direct map: physical memory at kernel virtual offset
	// (boot code maps low memory, paging extends it up to top of RAM).
	// Bootloader data may lie anywhere in physical memory,
	// so every pointer is checked against directly mapped range
	class direct final {

		// End of directly mapped physical memory
		static inline constinit phys_t	mEnd	{LOW_MEMORY_LIMIT};

		// Copy c-tor
		direct(const direct &other) = delete;
		// Copy assignment
		auto	operator=(const direct &other) -> direct& = delete;


	public:

		// End of directly mapped physical memory
		[[nodiscard]]
		static auto	end() noexcept -> phys_t;
		// Set end of directly mapped physical memory (paging mapped more memory)
		static void	setEnd(const phys_t end) noexcept;

		// Check if whole physical range is directly mapped
		[[nodiscard]]
		static auto	isMapped(const phys_t phys, const phys_t size) noexcept -> bool;

		// Pointer to physical memory (nullptr if range is not directly mapped)
		template<class T>
		[[nodiscard]]
		static auto	pointer(const phys_t phys, const phys_t size = sizeof(T)) noexcept -> T*;
		// String at physical memory (nullptr if not directly mapped up to null terminator)
		[[nodiscard]]
		static auto	string(const phys_t phys) noexcept -> const char*;

		// Physical address of directly mapped object
		template<class T>
		[[nodiscard]]
		static auto	physOf(const T* const object) noexcept -> phys_t;

	};


	// End of directly mapped physical memory
	[[nodiscard]]
	inline auto direct::end() noexcept -> phys_t {
		return mEnd;
	}

	// Set end of directly mapped physical memory (paging mapped more memory)
	inline void direct::setEnd(const phys_t end) noexcept {
		mEnd = end;
	}


	// Check if whole physical range is directly mapped
	[[nodiscard]]
	inline auto direct::isMapped(const phys_t phys, const phys_t size) noexcept -> bool {
		return (phys < mEnd) && (size <= (mEnd - phys));
	}


	// Pointer to physical memory (nullptr if range is not directly mapped)
	template<class T>
	[[nodiscard]]
	inline auto direct::pointer(const phys_t phys, const phys_t size) noexcept -> T* {
		if (!direct::isMapped(phys, size)) [[unlikely]] {
			return nullptr;
		}
		return std::bit_cast<T*>(static_cast<igros_usize_t>(phys) + platform::Platform::kernelOffset());
	}

	// String at physical memory (nullptr if not directly mapped up to null terminator)
	[[nodiscard]]
	inline auto direct::string(const phys_t phys) noexcept -> const char* {
		const auto* const text {direct::pointer<const char>(phys)};
		if (nullptr == text) [[unlikely]] {
			return nullptr;
		}
		// Never read past direct map end looking for null terminator
		// (index, not [begin, end) range: end pointer wraps if direct map reaches end of address space)
		const auto available {mEnd - phys};
		for (auto length {0_u64}; length < available; ++length) {
			if ('\0' == text[length]) {
				return text;
			}
		}
		return nullptr;
	}


	// Physical address of directly mapped object
	template<class T>
	[[nodiscard]]
	inline auto direct::physOf(const T* const object) noexcept -> phys_t {
		return static_cast<phys_t>(std::bit_cast<igros_usize_t>(object) - platform::Platform::kernelOffset());
	}


}	// namespace igros::mem
