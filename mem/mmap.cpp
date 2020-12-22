////////////////////////////////////////////////////////////////
//
//	Memory map operations definition
//
//	File:	mmap.cpp
//	Date:	17 Jul 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#include <cstdint>

#include <platform.hpp>

#include <mem/mmap.hpp>


// Memory code zone
namespace igros::mem {


	// Free pages pointer
	pointer_t phys::freePageList {nullptr};


	// Initialize physical memory
	void phys::init(const multiboot::memoryMapEntry* map, const std::size_t size) noexcept {

		// Memory map entries iterator
		auto entry	= map;
		// Previous entry pointer
		auto prevEntry	= static_cast<pointer_t>(nullptr);
		// Loop through memory map
		while (reinterpret_cast<std::size_t>(entry) < size) {
			// Check if entry is available
			if (	(multiboot::MEMORY_MAP_TYPE::AVAILABLE == entry->type)
				&& ((reinterpret_cast<std::size_t>(entry) < reinterpret_cast<std::size_t>(platform::KERNEL_START()))
				|| (reinterpret_cast<std::size_t>(entry) > reinterpret_cast<std::size_t>(platform::KERNEL_END())))) {
				// Initial entry's memory offset
				auto entrySize = 0ULL;
				// Loop through entry's memory
				while ((entry->address + entrySize) < entry->length) {
					// Calculate new entry address
					const auto newEntry = pointer_t(entry->address + entrySize);
					// Place previous entry's pointer in current entry
					*reinterpret_cast<pointer_t*>(newEntry) = prevEntry;
					// Save new entry's address ass new oldEntry's address
					prevEntry = newEntry;
					// Increment size by one page (4KB)
					entrySize += DEFAULT_PAGE_SIZE;
				}
			}
			// Move to next memory map entry
			entry = reinterpret_cast<multiboot::memoryMapEntry*>(std::size_t(entry) + entry->size + sizeof(entry->size));
		}
		// Linked list of free pages 
		phys::freePageList = prevEntry;

	}


	// Allcoate physical page
   	[[nodiscard]] pointer_t phys::alloc() noexcept {
		// Check if we have free pages
		if (nullptr != phys::freePageList) {
			// Get page pointer
			const auto page		= phys::freePageList;
			// Remove page from list of free pages
			phys::freePageList	= reinterpret_cast<pointer_t>(*static_cast<std::size_t*>(phys::freePageList));
			// Return page pointer
			return page;
		}
		// No free pages left
		return nullptr;
	}

	// Free physical page
	void phys::free(pointer_t &page) noexcept {
		// Error check
		if (nullptr == page) {
			return;
		}
		// Return page to free pages list
		*static_cast<pointer_t*>(page)	= phys::freePageList;
		phys::freePageList		= reinterpret_cast<pointer_t>(*static_cast<std::size_t*>(phys::freePageList));
		page				= nullptr;
	}


}	// namespace igros::mem

