////////////////////////////////////////////////////////////////
//
//	Memory map operations
//
//	File:	mmap.hpp
//	Date:	30 Jun 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <cstdint>

#include <types.hpp>

#include <multiboot.hpp>


// Memory code zone
namespace igros::mem {


	// Default page size constant
	constexpr static std::size_t DEFAULT_PAGE_SIZE = 0x1000;


	// Phyical memory structure
	class phys final {

		// Free pages pointer
		static pointer_t freePageList;


	public:

		// Initialize physical memory
		static void init(const multiboot::memoryMapEntry_t* map, const std::size_t size) noexcept;

		// Allcoate physical page
   		[[nodiscard]] static pointer_t	alloc() noexcept;
		// Free physical page
		static void			free(pointer_t &page) noexcept;


	};


}	// namespace igros::mem

