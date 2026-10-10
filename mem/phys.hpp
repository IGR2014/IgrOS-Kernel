////////////////////////////////////////////////////////////////
//
//	Memory map operations
//
//	File:	mmap.hpp
//	Date:	16 Dec 2022
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// C++
#include <cstdint>
// IgrOS-Kernel multiboot
#include <multiboot/multiboot.hpp>


// Memory code zone
namespace igros::mem {


	// Default page size constant
	constexpr auto DEFAULT_PAGE_SIZE {0x1000_usize};


	// Phyical memory structure
	class phys final {

		// Free pages pointer
		static igros_pointer_t	freePageList;


	public:

		// Initialize physical memory
		static void	init(const multiboot::memoryMapEntry* map, const igros_usize_t size) noexcept;

		// Allcoate physical page
   		[[nodiscard]]
		static auto	alloc() noexcept -> igros_pointer_t;
		// Free physical page
		static void	free(igros_pointer_t &page) noexcept;


	};


}	// namespace igros::mem

