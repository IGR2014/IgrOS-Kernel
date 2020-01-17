////////////////////////////////////////////////////////////////
//
//	Memory map operations
//
//	File:	mmap.hpp
//	Date:	02 Oct 2019
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#pragma once
#ifndef IGROS_ARCH_MEMORY_MMAP_HPP
#define IGROS_ARCH_MEMORY_MMAP_HPP


#include <cstdint>

#include <arch/types.hpp>

#include <multiboot.hpp>


// Memory code zone
namespace mem {


	// Default page size constant
	static const std::size_t DEFAULT_PAGE_SIZE = 0x1000;


	// Initialize memory map
	void		mmapInit(const multiboot::memoryMapEntry_t* memoryMap, const std::size_t memoryMapSize);

	// Allcoate physical page
   	pointer_t	mmapPageAlloc();
	// Free physical page
	void		mmapPageFree(pointer_t* page);


}	// namespace mem


#endif	// IGROS_ARCH_MEMORY_MMAP_HPP

