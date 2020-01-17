////////////////////////////////////////////////////////////////
//
//	Multiboot 1 functions
//
//	File:	multiboot.cpp
//	Date:	10 Oct 2019
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#include <multiboot.hpp>

#include <arch/types.hpp>

#include <drivers/vmem.hpp>

#include <klib/kprint.hpp>


// Multiboot code zone
namespace multiboot {


        // Dump multiboot flags
	void info_t::dumpFlags() const {

                // Print buffer
		sbyte_t text[1024];
		klib::ksprintf(	text,
				"MULTIBOOT header\r\n"
				"Flags:\t\t0x%x\r\n"
				"BIOS memory:\t%s\r\n"
				"Boot device:\t%s\r\n"
				"Command line:\t%s\r\n"
				"Sections AOUT:\t%s\r\n"
				"Sections ELF:\t%s\r\n"
				"Memory map:\t%s\r\n"
				"Drives:\t\t%s\r\n"
				"Table config:\t%s\r\n"
				"Loader name:\t%s\r\n"
				"Table APM:\t%s\r\n"
				"VBE:\t\t%s\r\n"
				"Frame buffer:\t%s\r\n"
				"\r\n",
				flags,
				hasInfoMemory() ? "true" : "false",
				hasInfoBootDevice() ? "true" : "false",
				hasInfoCommandLine() ? "true" : "false",
				hasInfoAOUT() ? "true" : "false",
				hasInfoELF() ? "true" : "false",
				hasInfoMemoryMap() ? "true" : "false",
				hasInfoDrives() ? "true" : "false",
				hasInfoConfig() ? "true" : "false",
				hasInfoBootloaderName() ? "true" : "false",
				hasInfoAPM() ? "true" : "false",
				hasInfoVBE() ? "true" : "false",
				hasInfoFrameBuffer() ? "true" : "false");
		arch::vmemWrite(text);

        }

	// Dump multiboot memory info
	void info_t::dumpMemInfo() const {

		// Print buffer
		sbyte_t text[1024];
		// Check if memory info exists
		if (hasInfoMemory()) {
			klib::ksprintf(	text,
					"MEMORY INFO:\r\n"
					"\tLow:\t%d Kb\r\n"
					"\tHigh:\t%d Kb.\r\n",
					memLow,
					memHigh);
			arch::vmemWrite(text);
		} else {
			klib::ksprintf(	text,
					"\tNo memory info provided...\r\n");
			arch::vmemWrite(text);
		}
		klib::ksprintf(text, "\r\n");
		arch::vmemWrite(text);

	}

	// Dump multiboot memory map
	void info_t::dumpMemMap() const {

		// Print buffer
		sbyte_t text[1024];
		// Check if memory map exists
		if (hasInfoMemoryMap()) {
			klib::ksprintf(	text,
					"MEMORY MAP:\r\n"
					"\tSize:\t%d bytes\r\n"
					"\tAddr:\t0x%p\r\n",
					mmapLength,
					mmapAddr);
			arch::vmemWrite(text);
			// Get pointer to memory map
			auto memoryMap = reinterpret_cast<multiboot::memoryMapEntry_t*>(mmapAddr);
			// Loop through memory map
			while (quad_t(memoryMap) < (mmapAddr + mmapLength)) {
				klib::ksprintf(	text,
						"\t[%d] 0x%llx - 0x%llx\r\n",
						memoryMap->type,
						memoryMap->address,
						memoryMap->address + memoryMap->length);
				arch::vmemWrite(text);
				// Move to next memory map entry
				memoryMap = reinterpret_cast<multiboot::memoryMapEntry_t*>(quad_t(memoryMap) + memoryMap->size + sizeof(memoryMap->size));
			}
		} else {
			klib::ksprintf(	text,
					"\tNo memory map provided...\r\n");
			arch::vmemWrite(text);
		}
		klib::ksprintf(text, "\r\n");
		arch::vmemWrite(text);

	}


}	// namespace multiboot

