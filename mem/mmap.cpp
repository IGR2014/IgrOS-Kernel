////////////////////////////////////////////////////////////////
//
//	Memory map operations definition
//
//	File:	mmap.cpp
//	Date:	09 Dec 2022
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


#include <cstdint>

#include <platform.hpp>

#include <mem/mmap.hpp>


// Memory code zone
namespace igros::mem {


	// Free pages pointer
	igros_pointer_t phys::freePageList {nullptr};


	// Initialize physical memory
	void phys::init(const multiboot::memoryMapEntry* map, const igros_usize_t size) noexcept {
		// Memory map entries iterator
		auto entry	{map};
		// Previous entry pointer
		auto prevEntry	{static_cast<igros_pointer_t>(nullptr)};
		// Loop through memory map
		while (std::bit_cast<igros_usize_t>(entry) < size) {
			// Check if entry is available
			if (
				(multiboot::MEMORY_MAP_TYPE::AVAILABLE	== entry->type)		&&
				(
					(std::bit_cast<igros_usize_t>(entry)	< std::bit_cast<igros_usize_t>(platform::KERNEL_START()))	||
					(std::bit_cast<igros_usize_t>(entry)	> std::bit_cast<igros_usize_t>(platform::KERNEL_END()))
				)
			) {
				// Initial entry's memory offset
				auto entrySize {0_usize};
				// Loop through entry's memory
				while ((entry->address + entrySize) < entry->length) {
					// Calculate new entry address
					const auto newEntry {std::bit_cast<igros_pointer_t>(static_cast<igros_usize_t>(entry->address + entrySize))};
					// Place previous entry's pointer in current entry
					*std::bit_cast<igros_pointer_t*>(newEntry) = prevEntry;
					// Save new entry's address ass new oldEntry's address
					prevEntry = newEntry;
					// Increment size by one page (4KB)
					entrySize += DEFAULT_PAGE_SIZE;
				}
			}
			// Move to next memory map entry
			entry = std::bit_cast<multiboot::memoryMapEntry*>(std::bit_cast<igros_usize_t>(entry) + entry->size + sizeof(entry->size));
		}
		// Linked list of free pages 
		phys::freePageList = prevEntry;
	}


	// Allcoate physical page
   	[[nodiscard]]
	auto phys::alloc() noexcept -> igros_pointer_t {
		// Check if we have free pages
		if (nullptr != phys::freePageList) {
			// Get page pointer
			const auto page		{phys::freePageList};
			// Remove page from list of free pages
			phys::freePageList	= std::bit_cast<igros_pointer_t>(*static_cast<igros_usize_t*>(phys::freePageList));
			// Return page pointer
			return page;
		}
		// No free pages left
		return nullptr;
	}

	// Free physical page
	void phys::free(igros_pointer_t &page) noexcept {
		// Error check
		if (nullptr == page) {
			return;
		}
		// Return page to free pages list
		*static_cast<igros_pointer_t*>(page)	= phys::freePageList;
		phys::freePageList			= std::bit_cast<igros_pointer_t>(*static_cast<igros_usize_t*>(phys::freePageList));
		page					= nullptr;
	}


}	// namespace igros::mem

