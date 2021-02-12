////////////////////////////////////////////////////////////////
//
//	Multiboot 1 functions
//
//	File:	multiboot.cpp
//	Date:	12 Feb 2021
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
				u8"BAD MULTIBOOT MAGIC!!!\r\n"
				u8"\tMagic:\t\t0x%08x\r\n"
				u8"\tAddress:\t0x%p\r\n",
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
			u8"MULTIBOOT header:\r\n"
			u8"\tFlags:\t\t\t0x%08x\r\n"
			u8"\tBIOS memory map:\t[ %c ]\r\n"
			u8"\tBoot device:\t\t[ %c ]\r\n"
			u8"\tCommand line:\t\t[ %c ]\r\n"
			u8"\tAOUT:\t\t\t[ %c ]\r\n"
			u8"\tELF:\t\t\t[ %c ]\r\n"
			u8"\tMemory map:\t\t[ %c ]\r\n"
			u8"\tDrives:\t\t\t[ %c ]\r\n"
			u8"\tTable config:\t\t[ %c ]\r\n"
			u8"\tBootloader name:\t[ %c ]\r\n"
			u8"\tAPM:\t\t\t[ %c ]\r\n"
			u8"\tVBE:\t\t\t[ %c ]\r\n"
			u8"\tFB:\t\t\t[ %c ]\r\n",
			flags,
			hasInfoMemory()		? u8'Y' : u8'N',
			hasInfoBootDevice()	? u8'Y' : u8'N',
			hasInfoCommandLine()	? u8'Y' : u8'N',
			hasInfoAOUT()		? u8'Y' : u8'N',
			hasInfoELF()		? u8'Y' : u8'N',
			hasInfoMemoryMap()	? u8'Y' : u8'N',
			hasInfoDrives()		? u8'Y' : u8'N',
			hasInfoConfig()		? u8'Y' : u8'N',
			hasInfoBootloaderName()	? u8'Y' : u8'N',
			hasInfoAPM()		? u8'Y' : u8'N',
			hasInfoVBE()		? u8'Y' : u8'N',
			hasInfoFrameBuffer()	? u8'Y' : u8'N'
		);
        }

	// Print multiboot memory info
	void info_t::printMemInfo() const noexcept {
		// Print header
		klib::kprintf(u8"MEMORY INFO:\r\n");
		// Check if memory info exists
		if (hasInfoMemory()) {
			klib::kprintf(
				u8"\tLow:\t%d Kb\r\n"
				u8"\tHigh:\t%d Kb.\r\n",
				memLow,
				memHigh
			);
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
			klib::kprintf(
				u8"\tSize:\t%d bytes\r\n"
				u8"\tAddr:\t0x%p\r\n",
				mmapLength,
				mmapAddr
			);
			// Get pointer to memory map
			auto memoryMap = reinterpret_cast<multiboot::memoryMapEntry*>(mmapAddr);
			// Loop through memory map
			while (reinterpret_cast<quad_t>(memoryMap) < (mmapAddr + mmapLength)) {
				klib::kprintf(
					u8"\t[%d] 0x%p - 0x%p",
					memoryMap->type,
					reinterpret_cast<pointer_t>(memoryMap->address),
					reinterpret_cast<pointer_t>(memoryMap->address + memoryMap->length)
				);
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
				u8"Signature:\t%c%c%c%c\r\n"
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
				(config->version >> 8) & 0x00FF,
				config->version & 0x00FF,
				(nullptr != oem)	? oem		: u8"Unknown",
				(nullptr != vendor)	? vendor	: u8"Unknown",
				(nullptr != product)	? product	: u8"Unknown",
				(nullptr != revision)	? revision	: u8"Unknown",
				vbeModeCurrent,
				mode->width,
				mode->height,
				mode->bpp,
				mode->physbase,
				static_cast<dword_t>(config->memory) * 64U
			);
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
			// Framebuffer type name
			const char* fbTypeName = u8"";
			// Get framebuffer type
			switch (fbType) {
				// Indexed
				case 0:
					fbTypeName = u8"Indexed";
					break;
				// RGB
				case 1:
					fbTypeName = u8"RGB";
					break;
				// Text
				default:
					fbTypeName = u8"Text";
					break;
			}
			// Dump FB
			klib::kprintf(
				u8"Current mode:\t(%dx%d, %dbpp, %d, %s)\r\n"
				u8"Address:\t0x%p\r\n"
				u8"Size:\t\t%z\r\n",
				fbWidth,
				fbHeight,
				fbBpp,
				fbPitch,
				fbTypeName,
				static_cast<std::size_t>(fbAddress),
				fbWidth * (fbBpp >> 3) * fbHeight * fbPitch
			);
		} else {
			klib::kprintf(u8"\tNo framebuffer info provided...\r\n");
		}
	}


}	// namespace igros::multiboot

