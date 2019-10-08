////////////////////////////////////////////////////////////////
//
//	Multiboot 1 functions
//
//	File:	multiboot.cpp
//	Date:	08 Oct 2019
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
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

		klib::ksprint(text,	"MULTIBOOT header\r\n"
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


}	// namespace multiboot

