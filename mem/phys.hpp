////////////////////////////////////////////////////////////////
//
//	Physical memory allocator
//
//	File:	phys.hpp
//	Date:	16 Dec 2022
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// IgrOS-Kernel arch
#include <arch/types.hpp>
// IgrOS-Kernel multiboot
#include <multiboot/multiboot.hpp>


// Memory code zone
namespace igros::mem {


	// Physical address (64-bit: physical memory may lie above 4 GB)
	using phys_t	= igros_quad_t;

	// Physical page size
	constexpr auto PAGE_SHIFT	{12_usize};
	constexpr auto PAGE_SIZE	{1_usize << PAGE_SHIFT};

	// Physical memory mapped by boot code at kernel virtual offset (see boot.s)
	constexpr auto LOW_MEMORY_LIMIT		{0x400000_u64};
	// Physical memory never given out (BIOS, video memory, ROMs)
	constexpr auto LOW_MEMORY_RESERVED	{0x100000_u64};


	// Physical page frame allocator
	// (bitmap of physical pages: never touches the pages themselves,
	// so free memory doesn't have to be mapped)
	class phys final {

		// Copy c-tor
		phys(const phys &other) = delete;
		// Copy assignment
		auto	operator=(const phys &other) -> phys& = delete;


	public:

		// Physical memory tracked by allocator (bitmap of 128 KB)
		constexpr static auto	MEMORY_LIMIT	{0x100000000_u64};

		// Initialize from bootloader memory map (reserves kernel and boot data)
		static void	init(const multiboot::info_t &info) noexcept;

		// Allocate physical page below limit (0 if no memory left)
		[[nodiscard]]
		static auto	alloc(const phys_t limit = MEMORY_LIMIT) noexcept -> phys_t;
		// Free physical page
		static void	free(const phys_t page) noexcept;

		// Number of free pages
		[[nodiscard]]
		static auto	freePages() noexcept -> igros_usize_t;

		// Mark physical memory range [start, end) as free (only whole pages)
		static void	markFree(const phys_t start, const phys_t end) noexcept;
		// Mark physical memory range [start, end) as used (every touched page)
		static void	markUsed(const phys_t start, const phys_t end) noexcept;


	};


}	// namespace igros::mem

