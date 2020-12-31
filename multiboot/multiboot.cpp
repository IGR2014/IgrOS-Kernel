////////////////////////////////////////////////////////////////
//
//	Multiboot 1 functions
//
//	File:	multiboot.cpp
//	Date:	17 Jul 2020
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#include <multiboot.hpp>

#include <arch/types.hpp>
#include <arch/cpu.hpp>

#include <klib/kprint.hpp>


// Multiboot code zone
namespace igros::multiboot {


	// Test multiboot
	void test(const info_t* const multiboot, const dword_t magic) noexcept {
		// Check multiboot magic
		if (!multiboot::check(magic)) {
			// Write Multiboot magic error message message
			klib::kprintf(	u8"BAD MULTIBOOT MAGIC!!!\r\n"
					u8"\tMagic:\t\t0x%08x\r\n"
					u8"\tAddress:\t0x%p\r\n",
					magic,
					multiboot);
			// Hang CPU
			arch::cpu::get().halt();
		}
	}


        // Print multiboot flags
	void info_t::printFlags() const noexcept {
                // Print header
		klib::kprintf(	u8"MULTIBOOT header:\r\n"
				u8"Flags:\t\t0x%x\r\n"
				u8"BIOS memory map:\t%s\r\n"
				u8"Boot device:\t%s\r\n"
				u8"Command line:\t%s\r\n"
				u8"AOUT:\t\t%s\r\n"
				u8"ELF:\t\t%s\r\n"
				u8"Memory map:\t%s\r\n"
				u8"Drives:\t\t%s\r\n"
				u8"Table config:\t%s\r\n"
				u8"Bootloader name:\t%s\r\n"
				u8"APM:\t\t%s\r\n"
				u8"VBE:\t\t%s\r\n"
				u8"FB:\t\t%s\r\n",
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

	// Print multiboot memory info
	void info_t::printMemInfo() const noexcept {
		// Print header
		klib::kprintf(u8"MEMORY INFO:\r\n");
		// Check if memory info exists
		if (hasInfoMemory()) {
			klib::kprintf(	u8"\tLow:\t%d Kb\r\n"
					u8"\tHigh:\t%d Kb.\r\n",
					memLow,
					memHigh);
		} else {
			klib::kprintf(u8"\tNo memory info provided...\r\n");
		}
	}

	// Dump multiboot memory map
	void info_t::printMemMap() const noexcept {
		// Print header
		klib::kprintf(u8"MEMORY MAP:\r\n");
		// Check if memory map exists
		if (hasInfoMemoryMap()) {
			klib::kprintf(	u8"\tSize:\t%d bytes\r\n"
					u8"\tAddr:\t0x%p\r\n",
					mmapLength,
					mmapAddr);
			// Get pointer to memory map
			auto memoryMap = reinterpret_cast<multiboot::memoryMapEntry*>(mmapAddr);
			// Loop through memory map
			while (reinterpret_cast<quad_t>(memoryMap) < (mmapAddr + mmapLength)) {
				klib::kprintf(	u8"\t[%d] 0x%p - 0x%p",
						memoryMap->type,
						reinterpret_cast<pointer_t>(memoryMap->address),
						reinterpret_cast<pointer_t>(memoryMap->address + memoryMap->length));
				// Move to next memory map entry
				memoryMap = reinterpret_cast<multiboot::memoryMapEntry*>(reinterpret_cast<quad_t>(memoryMap) + memoryMap->size + sizeof(memoryMap->size));
			}
			klib::kprintf(u8"\r\n");
		} else {
			klib::kprintf(u8"\tNo memory map provided...\r\n");
		}
	}


	// Print multiboot VBE info
	void info_t::printVBEInfo() const noexcept {
		// Print header
		klib::kprintf(u8"VBE:\r\n");
		// Test VBE
		if (hasInfoVBE()) {
			// Get VBE config info
			auto config	= reinterpret_cast<multiboot::vbeConfig*>(vbeControlInfo);
			// Get current VBE mode
			auto mode	= reinterpret_cast<multiboot::vbeMode*>(vbeModeInfo);
			// Get OEM string
			auto oem	= reinterpret_cast<const char* const>(((config->oem & 0xFFFF0000) >> 12) + (config->oem & 0xFFFF));
			// Get available modes
			//auto modes	= reinterpret_cast<multiboot::vbeMode*>(((config->modes & 0xFFFF0000) >> 12) + (config->modes & 0xFFFF));
			// Get vendor string
			auto vendor	= reinterpret_cast<const char* const>(((config->vendor & 0xFFFF0000) >> 12) + (config->vendor & 0xFFFF));
			// Get product string
			auto product	= reinterpret_cast<const char* const>(((config->productName & 0xFFFF0000) >> 12) + (config->productName & 0xFFFF));
			// Get revision string
			auto revision	= reinterpret_cast<const char* const>(((config->productRev & 0xFFFF0000) >> 12) + (config->productRev & 0xFFFF));
			// Dump VBE
			klib::kprintf(	u8"Signature:\t%c%c%c%c\r\n"
					u8"Version:\t%d.%d\r\n"
					u8"OEM:\t\t\"%s\"\r\n"
					u8"Vendor name:\t\"%s\"\r\n"
					u8"Card name:\t\"%s\"\r\n"
					u8"Card rev.:\t\"%s\"\r\n"
					u8"Current mode:\t#%d (%dx%d, %dbpp, 0x%p)\r\n"
					u8"Video memory:\t%d Kb.\r\n",
					config->signature[0],
					config->signature[1],
					config->signature[2],
					config->signature[3],
					(config->version >> 8) & 0xFF,
					config->version & 0xFF,
					(nullptr != oem) ? oem : "Unknown",
					(nullptr != vendor) ? vendor : "Unknown",
					(nullptr != product) ? product : "Unknown",
					(nullptr != revision) ? revision : "Unknown",
					vbeModeCurrent,
					mode->width,
					mode->height,
					mode->bpp,
					mode->physbase,
					static_cast<dword_t>(config->memory) * 64);
		} else {
			klib::kprintf(u8"\tNo VBE info provided...\r\n");
		}
	}

	// Print multiboot FB info
	void info_t::printFBInfo() const noexcept {
		// Print header
		klib::kprintf(u8"FB:\r\n");
		// Check framebuffer
		if (hasInfoFrameBuffer()) {
			// Dump FB
			klib::kprintf(	u8"Current mode:\t(%dx%d, %dbpp, %d, %s)\r\n"
					u8"Address:\t0x%p\r\n"
					u8"Size:\t\t%z\r\n",
					fbWidth,
					fbHeight,
					fbBpp,
					fbPitch,
					((0u == fbType) ? u8"Indexed" : ((1u == fbType) ? u8"RGB" : u8"Text")),
					static_cast<std::size_t>(fbAddress),
					fbWidth * (fbBpp >> 3) * fbHeight * fbPitch);
		} else {
			klib::kprintf(u8"\tNo framebuffer info provided...\r\n");
		}
	}


}	// namespace igros::multiboot

