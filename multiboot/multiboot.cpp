////////////////////////////////////////////////////////////////
//
//	Multiboot 1 functions
//
//	File:	multiboot.cpp
//	Date:	12 Feb 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#include <multiboot.hpp>

#include <types.hpp>

#include <drivers/vmem.hpp>

#include <klib/kprint.hpp>


// Multiboot code zone
namespace multiboot {


        // Dump multiboot flags
	void info_t::dumpFlags() const noexcept {
                // Print header
		klib::kprintf(	u8"MULTIBOOT header:\r\n"
				u8"Flags:\t\t0x%x\r\n"
				u8"BIOS memory:\t%s\r\n"
				u8"Boot device:\t%s\r\n"
				u8"Command line:\t%s\r\n"
				u8"Sections AOUT:\t%s\r\n"
				u8"Sections ELF:\t%s\r\n"
				u8"Memory map:\t%s\r\n"
				u8"Drives:\t\t%s\r\n"
				u8"Table config:\t%s\r\n"
				u8"Loader name:\t%s\r\n"
				u8"Table APM:\t%s\r\n"
				u8"VBE:\t\t%s\r\n"
				u8"Frame buffer:\t%s\r\n",
				flags,
				hasInfoMemory()		? u8"true" : u8"false",
				hasInfoBootDevice()	? u8"true" : u8"false",
				hasInfoCommandLine()	? u8"true" : u8"false",
				hasInfoAOUT()		? u8"true" : u8"false",
				hasInfoELF()		? u8"true" : u8"false",
				hasInfoMemoryMap()	? u8"true" : u8"false",
				hasInfoDrives()		? u8"true" : u8"false",
				hasInfoConfig()		? u8"true" : u8"false",
				hasInfoBootloaderName()	? u8"true" : u8"false",
				hasInfoAPM()		? u8"true" : u8"false",
				hasInfoVBE()		? u8"true" : u8"false",
				hasInfoFrameBuffer()	? u8"true" : u8"false");
        }

	// Dump multiboot memory info
	void info_t::dumpMemInfo() const noexcept {
		// Check if memory info exists
		if (hasInfoMemory()) {
			klib::kprintf(	u8"MEMORY INFO:\r\n"
					u8"\tLow:\t%d Kb\r\n"
					u8"\tHigh:\t%d Kb.\r\n",
					memLow,
					memHigh);
		} else {
			klib::kprintf(	u8"MEMORY INFO:\r\n"
					u8"\tNo memory info provided...\r\n");
		}
	}

	// Dump multiboot memory map
	void info_t::dumpMemMap() const noexcept {
		// Check if memory map exists
		if (hasInfoMemoryMap()) {
			klib::kprintf(	"MEMORY MAP:\r\n"
					"\tSize:\t%d bytes\r\n"
					"\tAddr:\t0x%p\r\n",
					mmapLength,
					mmapAddr);
			// Get pointer to memory map
			auto memoryMap = reinterpret_cast<multiboot::memoryMapEntry_t*>(mmapAddr);
			// Loop through memory map
			while (reinterpret_cast<quad_t>(memoryMap) < (mmapAddr + mmapLength)) {
				klib::kprintf(	u8"\t[%d] 0x%p - 0x%p",
						memoryMap->type,
						reinterpret_cast<pointer_t>(memoryMap->address),
						reinterpret_cast<pointer_t>(memoryMap->address + memoryMap->length));
				// Move to next memory map entry
				memoryMap = reinterpret_cast<multiboot::memoryMapEntry_t*>(quad_t(memoryMap) + memoryMap->size + sizeof(memoryMap->size));
			}
			klib::kprintf("\r\n");
		} else {
			klib::kprintf(	u8"MEMORY MAP:\r\n"
					u8"\tNo memory map provided...\r\n");
		}
	}


}	// namespace multiboot

