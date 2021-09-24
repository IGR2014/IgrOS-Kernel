////////////////////////////////////////////////////////////////
//
//	Multiboot 1 functions
//
//	File:	multiboot.cpp
//	Date:	24 Sep 2021
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#include <array>

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
			klib::kprintf(
				"BAD MULTIBOOT MAGIC!!!\r\n"
				"\tMagic:\t\t0x%08x\r\n"
				"\tAddress:\t0x%p\r\n",
				magic,
				multiboot
			);
			// Hang CPU
			arch::cpu::get().halt();
		}
	}


        // Print multiboot flags
	void info_t::printFlags() const noexcept {
                // Print header
		klib::kprintf(
			"MULTIBOOT header:\r\n"
			"\tFlags:\t\t\t0x%08x\r\n"
			"\tBIOS memory map:\t[ %c ]\r\n"
			"\tBoot device:\t\t[ %c ]\r\n"
			"\tCommand line:\t\t[ %c ]\r\n"
			"\tAOUT:\t\t\t[ %c ]\r\n"
			"\tELF:\t\t\t[ %c ]\r\n"
			"\tMemory map:\t\t[ %c ]\r\n"
			"\tDrives:\t\t\t[ %c ]\r\n"
			"\tTable config:\t\t[ %c ]\r\n"
			"\tBootloader name:\t[ %c ]\r\n"
			"\tAPM:\t\t\t[ %c ]\r\n"
			"\tVBE:\t\t\t[ %c ]\r\n"
			"\tFB:\t\t\t[ %c ]\r\n",
			flags,
			hasInfoMemory()		? 'Y' : 'N',
			hasInfoBootDevice()	? 'Y' : 'N',
			hasInfoCommandLine()	? 'Y' : 'N',
			hasInfoAOUT()		? 'Y' : 'N',
			hasInfoELF()		? 'Y' : 'N',
			hasInfoMemoryMap()	? 'Y' : 'N',
			hasInfoDrives()		? 'Y' : 'N',
			hasInfoConfig()		? 'Y' : 'N',
			hasInfoBootloaderName()	? 'Y' : 'N',
			hasInfoAPM()		? 'Y' : 'N',
			hasInfoVBE()		? 'Y' : 'N',
			hasInfoFrameBuffer()	? 'Y' : 'N'
		);
        }

	// Print multiboot memory info
	void info_t::printMemInfo() const noexcept {
		// Print header
		klib::kprintf("MEMORY INFO:\r\n");
		// Check if memory info exists
		if (hasInfoMemory()) {
			klib::kprintf(
				"\tLow:\t%d Kb\r\n"
				"\tHigh:\t%d Kb.\r\n",
				memLow,
				memHigh
			);
		} else {
			klib::kprintf("\tNo memory info provided...\r\n");
		}
	}

	// Dump multiboot memory map
	void info_t::printMemMap() const noexcept {
		// Print header
		klib::kprintf("MEMORY MAP:\r\n");
		// Check if memory map exists
		if (hasInfoMemoryMap()) {
			klib::kprintf(
				"\tSize:\t%d bytes\r\n"
				"\tAddr:\t0x%p\r\n",
				mmapLength,
				mmapAddr
			);
			// Get pointer to memory map
			auto memoryMap = reinterpret_cast<multiboot::memoryMapEntry*>(mmapAddr);
			// Loop through memory map
			while (reinterpret_cast<quad_t>(memoryMap) < (mmapAddr + mmapLength)) {
				klib::kprintf(
					"\t[%d] 0x%p - 0x%p",
					memoryMap->type,
					reinterpret_cast<pointer_t>(memoryMap->address),
					reinterpret_cast<pointer_t>(memoryMap->address + memoryMap->length)
				);
				// Move to next memory map entry
				memoryMap = reinterpret_cast<multiboot::memoryMapEntry*>(reinterpret_cast<quad_t>(memoryMap) + memoryMap->size + sizeof(memoryMap->size));
			}
			klib::kprintf("\r\n");
		} else {
			klib::kprintf("\tNo memory map provided...\r\n");
		}
	}


	// Print multiboot VBE info
	void info_t::printVBEInfo() const noexcept {
		// Print header
		klib::kprintf("VBE:\r\n");
		// Test VBE
		if (hasInfoVBE()) {
			// Get VBE config info
			const auto config	= reinterpret_cast<multiboot::vbeConfig*>(vbeControlInfo);
			// Get current VBE mode
			const auto mode		= reinterpret_cast<multiboot::vbeMode*>(vbeModeInfo);
			// Get OEM string
			const auto oem		= reinterpret_cast<const char* const>(((config->oem & 0xFFFF0000) >> 12) + (config->oem & 0xFFFF));
			// Get vendor string
			const auto vendor	= reinterpret_cast<const char* const>(((config->vendor & 0xFFFF0000) >> 12) + (config->vendor & 0xFFFF));
			// Get product string
			const auto product	= reinterpret_cast<const char* const>(((config->productName & 0xFFFF0000) >> 12) + (config->productName & 0xFFFF));
			// Get revision string
			const auto revision	= reinterpret_cast<const char* const>(((config->productRev & 0xFFFF0000) >> 12) + (config->productRev & 0xFFFF));
			// Dump VBE
			klib::kprintf(
				"Signature:\t%c%c%c%c\r\n"
				"Version:\t%d.%d\r\n"
				"OEM:\t\t\"%s\"\r\n"
				"Vendor name:\t\"%s\"\r\n"
				"Card name:\t\"%s\"\r\n"
				"Card rev.:\t\"%s\"\r\n"
				"Current mode:\t#%d (%dx%d, %dbpp, 0x%p)\r\n"
				"Video memory:\t%d Kb.\r\n",
				config->signature[0],
				config->signature[1],
				config->signature[2],
				config->signature[3],
				(config->version >> 8) & 0x00FF,
				config->version & 0x00FF,
				(nullptr != oem)	? oem		: "Unknown",
				(nullptr != vendor)	? vendor	: "Unknown",
				(nullptr != product)	? product	: "Unknown",
				(nullptr != revision)	? revision	: "Unknown",
				vbeModeCurrent,
				mode->width,
				mode->height,
				mode->bpp,
				mode->physbase,
				static_cast<dword_t>(config->memory) * 64U
			);
		} else {
			klib::kprintf("\tNo VBE info provided...\r\n");
		}
	}

	// Print multiboot FB info
	void info_t::printFBInfo() const noexcept {
		// Print header
		klib::kprintf("FB:\r\n");
		// Check framebuffer
		if (hasInfoFrameBuffer()) {
			// Framebuffer type name
			const char* fbTypeName = "";
			// Get framebuffer type
			switch (fbType) {
				// Indexed
				case 0:
					fbTypeName = "Indexed";
					break;
				// RGB
				case 1:
					fbTypeName = "RGB";
					break;
				// Text
				default:
					fbTypeName = "Text";
					break;
			}
			// Dump FB
			klib::kprintf(
				"Current mode:\t(%dx%d, %dbpp, %d, %s)\r\n"
				"Address:\t0x%p\r\n"
				"Size:\t\t%z\r\n",
				fbWidth,
				fbHeight,
				fbBpp,
				fbPitch,
				fbTypeName,
				static_cast<std::size_t>(fbAddress),
				fbWidth * (fbBpp >> 3) * fbHeight * fbPitch
			);
		} else {
			klib::kprintf("\tNo framebuffer info provided...\r\n");
		}
	}


}	// namespace igros::multiboot

