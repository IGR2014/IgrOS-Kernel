////////////////////////////////////////////////////////////////
///
///	@file
///	@brief		Direct map of physical memory
///	@date		03 Nov 2026
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
// IgrOS-Kernel memory
#include <mem/phys.hpp>
// IgrOS-Kernel platform
#include <platform/platform.hpp>


// Memory code zone
namespace igros::mem {


	/// @brief Direct map: physical memory at kernel virtual offset
	///
	/// (boot code maps low memory, paging extends it up to top of RAM).
	/// Bootloader data may lie anywhere in physical memory,
	/// so every pointer is checked against directly mapped range
	class direct final {

		/// @brief End of directly mapped physical memory
		static inline constinit phys_t mEnd {LOW_MEMORY_LIMIT};


	public:

		/// @brief D-tor
		~direct() noexcept = default;

		/// @brief Copy c-tor
		direct(const direct &other) = delete;
		/// @brief Copy assignment
		auto operator=(const direct &other) -> direct & = delete;

		/// @brief End of directly mapped physical memory
		/// @return Physical address where direct map ends
		[[nodiscard]]
		static auto end() noexcept -> phys_t;
		/// @brief Set end of directly mapped physical memory (paging mapped more memory)
		/// @param end Physical address where direct map ends
		static void setEnd(const phys_t end) noexcept;

		/// @brief Check if whole physical range is directly mapped
		/// @param phys Physical address
		/// @param size Range size
		/// @return true if range is directly mapped
		[[nodiscard]]
		static auto isMapped(const phys_t phys, const phys_t size) noexcept -> bool;

		/// @brief Pointer to physical memory (nullptr if range is not directly mapped)
		/// @tparam T Object type
		/// @param phys Physical address
		/// @param size Range that must be mapped
		/// @return Pointer, nullptr if not directly mapped
		template<class T>
		[[nodiscard]]
		static auto pointer(const phys_t phys, const phys_t size = sizeof(T)) noexcept -> T*;
		/// @brief String at physical memory (nullptr if not directly mapped up to null terminator)
		/// @param phys Physical address
		/// @return String, nullptr if not directly mapped
		[[nodiscard]]
		static auto string(const phys_t phys) noexcept -> const char*;

		/// @brief Physical address of directly mapped object
		/// @tparam T Object type
		/// @param object Directly mapped object
		/// @return Physical address
		template<class T>
		[[nodiscard]]
		static auto physOf(const T* const object) noexcept -> phys_t;
	};


	/// @brief End of directly mapped physical memory
	[[nodiscard]]
	inline auto direct::end() noexcept -> phys_t {
		return mEnd;
	}

	/// @brief Set end of directly mapped physical memory (paging mapped more memory)
	inline void direct::setEnd(const phys_t end) noexcept {
		mEnd = end;
	}


	/// @brief Check if whole physical range is directly mapped
	[[nodiscard]]
	inline auto direct::isMapped(const phys_t phys, const phys_t size) noexcept -> bool {
		return (phys < mEnd) && (size <= (mEnd - phys));
	}


	/// @brief Pointer to physical memory (nullptr if range is not directly mapped)
	template<class T>
	[[nodiscard]]
	inline auto direct::pointer(const phys_t phys, const phys_t size) noexcept -> T* {
		if (!direct::isMapped(phys, size)) [[unlikely]] {
			return nullptr;
		}
		return std::bit_cast<T*>(static_cast<igros_usize_t>(phys) + platform::Platform::kernelOffset());
	}

	/// @brief String at physical memory (nullptr if not directly mapped up to null terminator)
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


	/// @brief Physical address of directly mapped object
	template<class T>
	[[nodiscard]]
	inline auto direct::physOf(const T* const object) noexcept -> phys_t {
		return static_cast<phys_t>(std::bit_cast<igros_usize_t>(object) - platform::Platform::kernelOffset());
	}


} // namespace igros::mem
