////////////////////////////////////////////////////////////////
//
//	Multiboot 1 functions
//
//	File:	multiboot.cpp
//	Date:	21 Jun 2019
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
	void info::dumpFlags() const {

                // Print buffer
		sbyte_t text[64];

                // Print multiboot header flags
		arch::vmemWrite("\tFlags:\t\t0x");
		arch::vmemWrite(klib::kitoa(text, 20, flags, klib::base::HEX));
		arch::vmemWrite("\r\n");

		// Check if multiboot BIOS memory info present
		arch::vmemWrite("\tBIOS memory:\t");
		arch::vmemWrite(hasInfoMemory() ? "true" : "false");
		arch::vmemWrite("\r\n");

		// Check if multiboot boot device info present
		arch::vmemWrite("\tBoot device:\t");
		arch::vmemWrite(hasInfoBootDevice() ? "true" : "false");
		arch::vmemWrite("\r\n");

		// Check if multiboot kernel command line present
		arch::vmemWrite("\tCommand line:\t");
		arch::vmemWrite(hasInfoBootDevice() ? "true" : "false");
		arch::vmemWrite("\r\n");

		// Check if multiboot kernel AOUT sections info present
		arch::vmemWrite("\tSections AOUT:\t");
		arch::vmemWrite(hasInfoAOUT() ? "true" : "false");
		arch::vmemWrite("\r\n");

		// Check if multiboot kernel ELF sections info present
		arch::vmemWrite("\tSections ELF:\t");
		arch::vmemWrite(hasInfoELF() ? "true" : "false");
		arch::vmemWrite("\r\n");

		// Check if multiboot memory map present
		arch::vmemWrite("\tMemory map:\t");
		arch::vmemWrite(hasInfoMemoryMap() ? "true" : "false");
		arch::vmemWrite("\r\n");

		// Check if multiboot drives info present
		arch::vmemWrite("\tDrives:\t\t");
		arch::vmemWrite(hasInfoDrives() ? "true" : "false");
		arch::vmemWrite("\r\n");

		// Check if multiboot config table present
		arch::vmemWrite("\tTable config:\t");
		arch::vmemWrite(hasInfoConfig() ? "true" : "false");
		arch::vmemWrite("\r\n");

		// Check if multiboot bootloader name present
		arch::vmemWrite("\tLoader name:\t");
		arch::vmemWrite(hasInfoBootloaderName() ? "true" : "false");
		arch::vmemWrite("\r\n");

		// Check if multiboot APM table present
		arch::vmemWrite("\tTable APM:\t");
		arch::vmemWrite(hasInfoAPM() ? "true" : "false");
		arch::vmemWrite("\r\n");

		// Check if multiboot VBE info present
		arch::vmemWrite("\tVBE:\t\t");
		arch::vmemWrite(hasInfoVBE() ? "true" : "false");
		arch::vmemWrite("\r\n");

		// Check if multiboot FrameBuffer info present
		arch::vmemWrite("\tFrame buffer:\t");
		arch::vmemWrite(hasInfoFrameBuffer() ? "true" : "false");
		arch::vmemWrite("\r\n\r\n");

        }


}	// namespace multiboot

