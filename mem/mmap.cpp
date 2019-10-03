////////////////////////////////////////////////////////////////
//
//	Memory map operations definition
//
//	File:	mmap.cpp
//	Date:	03 Oct 2019
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
//	All rights reserved.
//
//


#include <cstdint>

#include <mem/mmap.hpp>


// Memory code zone
namespace mem {


	// Free pages pointer
	static pointer_t FREE_PAGES = nullptr;


	// Initialize memory map
	void mmapInit(const multiboot::memoryMapEntry_t* memoryMap, const std::size_t memoryMapSize) {

		// Memory map entries iterator
		auto entry = memoryMap;
		// Previous entry pointer
		pointer_t prevEntry = nullptr;
		// Loop through memory map
		while (std::size_t(entry) < memoryMapSize) {
			// Check if entry is available
			if (multiboot::MEMORY_MAP_TYPE::AVAILABLE == entry->type) {
				// Initial entry's memory offset
				quad_t size = 0ULL;
				// Loop through entry's memory
				while ((entry->address + size) < entry->length) {
					// Calculate new entry address
					const pointer_t newEntry = pointer_t(entry->address + size);
					// Place previous entry's pointer in current entry
					*reinterpret_cast<std::size_t*>(newEntry) = std::size_t(prevEntry);
					// Save new entry's address ass new oldEntry's address
					prevEntry = newEntry;
					// Increment size by one page (4KB)
					size += DEFAULT_PAGE_SIZE;
				}
			}
			// Move to next memory map entry
			entry = reinterpret_cast<multiboot::memoryMapEntry_t*>(std::size_t(entry) + entry->size + sizeof(entry->size));
		}
		// Linked list of free pages 
		FREE_PAGES = prevEntry;

	}


	// Allcoate physical page
   	pointer_t mmapPageAlloc() {
		// Check if we have free pages
		if (nullptr != FREE_PAGES) {
			// Get page pointer
			auto page = FREE_PAGES;
			// Remove page from list of free pages
			FREE_PAGES = reinterpret_cast<std::size_t*>(FREE_PAGES);
			// Return page pointer
			return page;
		}
		// No free pages left
		return nullptr;
	}

	// Free physical page
	void mmapPageFree(pointer_t* page) {
		// Error check
		if (nullptr == page) {
			return;
		}
		//

	}


}	// namespace mem

