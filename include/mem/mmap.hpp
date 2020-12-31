////////////////////////////////////////////////////////////////
//
//	Memory map operations
//
//	File:	mmap.hpp
//	Date:	17 Jul 2020
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <cstdint>

#include <multiboot.hpp>


// Memory code zone
namespace igros::mem {


	// Default page size constant
	constexpr auto DEFAULT_PAGE_SIZE = 0x1000;


	// Phyical memory structure
	class phys final {

		// Free pages pointer
		static pointer_t freePageList;


	public:

		// Initialize physical memory
		static void init(const multiboot::memoryMapEntry* map, const std::size_t size) noexcept;

		// Allcoate physical page
   		[[nodiscard]] static pointer_t	alloc() noexcept;
		// Free physical page
		static void			free(pointer_t &page) noexcept;


	};


}	// namespace igros::mem

