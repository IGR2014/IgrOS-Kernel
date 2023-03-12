////////////////////////////////////////////////////////////////
//
//	Multiboot 1 functions
//
//	File:	multiboot.cpp
//	Date:	12 Mar 2023
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


// C++
#include <array>
#include <bit>
// IgrOS-Kernel arch
#include <arch/types.hpp>
#include <arch/cpu.hpp>
// IgrOS-Kernel klib
#include <klib/kprint.hpp>
// IgrOS-Kernel multiboot
#include <multiboot/multiboot.hpp>


// Multiboot code zone
namespace igros::multiboot {


	// Test multiboot
	void test(const info_t* const multiboot, const igros_dword_t magic) noexcept {
		// Check multiboot magic
		if (!multiboot::check(magic)) [[unlikely]] {
			// Write Multiboot magic error message message
			klib::kprintf(
R"multiboot(
BAD MULTIBOOT MAGIC!!!
	Magic:		0x%08x
	Address:	0x%p
)multiboot",
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
R"multiboot(
MULTIBOOT header:
	Flags:			0x%08x
	BIOS memory map:	[ %c ]
	Boot device:		[ %c ]
	Command line:		[ %c ]
	AOUT:			[ %c ]
	ELF:			[ %c ]
	Memory map:		[ %c ]
	Drives:			[ %c ]
	Table config:		[ %c ]
	Bootloader name:	[ %c ]
	APM:			[ %c ]
	VBE:			[ %c ]
	FB:			[ %c ]
)multiboot",
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
		// Check if memory info exists
		if (hasInfoMemory()) [[likely]] {
			klib::kprintf(
R"multiboot(
MEMORY INFO:
	Low:	%d Kb.
	High:	%d Kb.

)multiboot",
				memLow,
				memHigh
			);
		} else {
			klib::kprintf(
R"multiboot(
MEMORY INFO:
	No memory info provided...

)multiboot"
			);
		}
	}

	// Dump multiboot memory map
	void info_t::printMemMap() const noexcept {
		// Check if memory map exists
		if (hasInfoMemoryMap()) [[likely]] {
			klib::kprintf(
R"multiboot(
MEMORY MAP:
	Size:	%d bytes
	Addr:	0x%p

)multiboot",
				mmapLength,
				mmapAddr
			);
			// Get pointer to memory map
			auto memoryMap {std::bit_cast<multiboot::memoryMapEntry*>(static_cast<igros_usize_t>(mmapAddr))};
			// Loop through memory map
			while (std::bit_cast<igros_usize_t>(memoryMap) < (mmapAddr + mmapLength)) {
				klib::kprintf(
R"multiboot(
	[%d] 0x%p - 0x%p"
)multiboot",
					memoryMap->type,
					std::bit_cast<igros_pointer_t>(static_cast<igros_usize_t>(memoryMap->address)),
					std::bit_cast<igros_pointer_t>(static_cast<igros_usize_t>(memoryMap->address + memoryMap->length))
				);
				// Move to next memory map entry
				memoryMap = std::bit_cast<multiboot::memoryMapEntry*>(std::bit_cast<igros_usize_t>(memoryMap) + memoryMap->size + sizeof(memoryMap->size));
			}
			klib::kprintf("\n");
		} else {
			klib::kprintf(
R"multiboot(
MEMORY MAP:
	No memory map provided...

)multiboot"
			);
		}
	}


	// Print multiboot VBE info
	void info_t::printVBEInfo() const noexcept {
		// Test VBE
		if (hasInfoVBE()) [[likely]] {
			// Get VBE config info
			const auto config	{std::bit_cast<multiboot::vbeConfig*>(static_cast<igros_usize_t>(vbeControlInfo))};
			// Get current VBE mode
			const auto mode		{std::bit_cast<multiboot::vbeMode*>(static_cast<igros_usize_t>(vbeModeInfo))};
			// Get OEM string
			const auto oem		{std::bit_cast<const char* const>(static_cast<igros_usize_t>(((config->oem & 0xFFFF0000) >> 12) + (config->oem & 0xFFFF)))};
			// Get vendor string
			const auto vendor	{std::bit_cast<const char* const>(static_cast<igros_usize_t>(((config->vendor & 0xFFFF0000) >> 12) + (config->vendor & 0xFFFF)))};
			// Get product string
			const auto product	{std::bit_cast<const char* const>(static_cast<igros_usize_t>(((config->productName & 0xFFFF0000) >> 12) + (config->productName & 0xFFFF)))};
			// Get revision string
			const auto revision	{std::bit_cast<const char* const>(static_cast<igros_usize_t>(((config->productRev & 0xFFFF0000) >> 12) + (config->productRev & 0xFFFF)))};
			// Dump VBE
			klib::kprintf(
R"multiboot(
VBE:
	Signature:	%c%c%c%c
	Version:	%d.%d
	OEM:		"%s"
	Vendor name:	"%s"
	Card name:	"%s"
	Card rev.:	"%s"
	Current mode:	#%d (%dx%d, %dbpp, 0x%p)
	Video memory:	%d Kb.

)multiboot",
				config->signature[0],
				config->signature[1],
				config->signature[2],
				config->signature[3],
				(config->version >> 8) & 0x00FF_u16,
				(config->version & 0x00FF_u16),
				(nullptr != oem)	? oem		: "<none>",
				(nullptr != vendor)	? vendor	: "<none>",
				(nullptr != product)	? product	: "<none>",
				(nullptr != revision)	? revision	: "<none>",
				vbeModeCurrent,
				mode->width,
				mode->height,
				mode->bpp,
				mode->physbase,
				static_cast<igros_dword_t>(config->memory) * 64_u32
			);
		} else {
			klib::kprintf(
R"multiboot(
VBE:
	No VBE info provided...

)multiboot"
			);
		}
	}

	// Print multiboot FB info
	void info_t::printFBInfo() const noexcept {
		// Print header
		klib::kprintf("FB:\r\n");
		// Check framebuffer
		if (hasInfoFrameBuffer()) [[likely]] {
			// Framebuffer type name
			const auto* fbTypeName {""};
			// Get framebuffer type
			switch (fbType) {
				// Indexed
				case 0x00_u8:
					fbTypeName = "Indexed";
					break;
				// RGB
				case 0x01_u8:
					fbTypeName = "RGB";
					break;
				// Text
				default:
					fbTypeName = "Text";
					break;
			}
			// Dump FB
			klib::kprintf(
R"multiboot(
FrameBuffer:
	Current mode:	(%dx%d, %dbpp, %d, %s)
	Address:	0x%p
	Size:		%z

)multiboot",
				fbWidth,
				fbHeight,
				fbBpp,
				fbPitch,
				fbTypeName,
				std::bit_cast<igros_pointer_t>(static_cast<igros_usize_t>(fbAddress)),
				fbWidth * (fbBpp >> 3) * fbHeight * fbPitch
			);
		} else {
			klib::kprintf(
R"multiboot(
FrameBuffer:
	No FrameBuffer info provided...

)multiboot"
			);
		}
	}


}	// namespace igros::multiboot

