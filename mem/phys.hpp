////////////////////////////////////////////////////////////////
///
///	@file
///	@brief		Physical memory allocator
///	@date		30 Oct 2026
///
///	@copyright	Copyright (c) 2017 - 2026, Igor Baklykov
///			All rights reserved.
///	@author		Igor Baklykov
///


#pragma once


// IgrOS-Kernel arch
#include <arch/types.hpp>


// Multiboot code zone
namespace igros::multiboot {


	/// @brief Multiboot information (multiboot.hpp reaches its data through direct map, which needs this header)
	struct info_t;


}	// namespace igros::multiboot


// Memory code zone
namespace igros::mem {


	/// @brief Physical address (64-bit: physical memory may lie above 4 GB)
	using phys_t	= igros_quad_t;

	// Physical page size
	constexpr auto PAGE_SHIFT	{12_usize};	///< Page size bits
	constexpr auto PAGE_SIZE	{1_usize << PAGE_SHIFT};	///< Page size in bytes

	/// @brief Physical memory mapped by boot code at kernel virtual offset (see boot.s)
	constexpr auto LOW_MEMORY_LIMIT		{0x400000_u64};
	/// @brief Physical memory never given out (BIOS, video memory, ROMs)
	constexpr auto LOW_MEMORY_RESERVED	{0x100000_u64};


	/// @brief Physical page frame allocator
	/// (bitmap of physical pages: never touches the pages themselves,
	/// so free memory doesn't have to be mapped)
	class phys final {

		/// @brief Copy c-tor
		phys(const phys &other) = delete;
		/// @brief Copy assignment
		auto	operator=(const phys &other) -> phys& = delete;


	public:

		/// @brief D-tor
		~phys() noexcept = default;

		/// @brief Physical memory tracked by allocator (bitmap of 128 KB)
		constexpr static auto	MEMORY_LIMIT	{0x100000000_u64};

		/// @brief Initialize from bootloader memory map (reserves kernel and boot data)
		/// @param info Multiboot information (memory map, boot data to keep)
		static void	init(const multiboot::info_t &info) noexcept;

		/// @brief Allocate physical page below limit (0 if no memory left)
		/// @param limit Page must be below this address
		/// @return Page physical address, 0 if no memory
		[[nodiscard]]
		static auto	alloc(const phys_t limit = MEMORY_LIMIT) noexcept -> phys_t;
		/// @brief Free physical page
		/// @param page Page physical address
		static void	free(const phys_t page) noexcept;

		/// @brief Number of free pages
		/// @return Number of free pages
		[[nodiscard]]
		static auto	freePages() noexcept -> igros_usize_t;
		/// @brief End of highest memory ever marked free (top of usable RAM, clamped to MEMORY_LIMIT)
		/// @return End of highest available memory
		[[nodiscard]]
		static auto	top() noexcept -> phys_t;

		/// @brief Mark physical memory range [start, end) as free (only whole pages)
		/// @param start Range start
		/// @param end Range end (whole pages inside are freed)
		static void	markFree(const phys_t start, const phys_t end) noexcept;
		/// @brief Mark physical memory range [start, end) as used (every touched page)
		/// @param start Range start
		/// @param end Range end (pages touched by range are used)
		static void	markUsed(const phys_t start, const phys_t end) noexcept;


	};


}	// namespace igros::mem

