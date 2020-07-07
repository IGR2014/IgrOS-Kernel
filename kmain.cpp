////////////////////////////////////////////////////////////////
//
//	Boot low-level main setup function
//
//	File:	boot.cpp
//	Date:	07 Jul 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#ifndef IGROS_ARCH
#define IGROS_ARCH "Unknown"
#endif	// IGROS_ARCH


// Common
#include <version.hpp>
#include <multiboot.hpp>
#include <platform.hpp>
#include <flags.hpp>

// Architecture dependent
#include <types.hpp>
#include <gdt.hpp>
#include <idt.hpp>
#include <irq.hpp>
#include <paging.hpp>
#include <cr.hpp>
#include <cpu.hpp>

// Kernel drivers
#include <drivers/vmem.hpp>
#include <drivers/keyboard.hpp>
#include <drivers/pit.hpp>
#include <drivers/rtc.hpp>

// Kernel library
#include <klib/kstring.hpp>
#include <klib/kprint.hpp>

// Kernel memory
#include <mem/mmap.hpp>


// Kernel start and end
extern const igros::byte_t _SECTION_KERNEL_START_;
extern const igros::byte_t _SECTION_KERNEL_END_;


// OS namesapce
namespace igros {


#ifdef	__cplusplus

	extern "C" {

#endif	// __cplusplus


		// Kernel main function
		void kmain(const multiboot::info_t* const multiboot, const dword_t magic) noexcept {

			// Init VGA memory
			arch::vmemInit();

			// Write Multiboot magic error message message
			klib::kprintf(u8"IgrOS kernel\r\n");

			// Check multiboot magic
			if (!multiboot::check(magic)) {
				// Write Multiboot magic error message message
				klib::kprintf(	u8"BAD MULTIBOOT MAGIC!!!\r\n"
						u8"\tMAGIC:\t\t0x%08x\r\n"
						u8"\tADDRESS:\t0x%p\r\n",
						magic,
						multiboot);
				// Hang CPU
				arch::cpuHalt();
			}

			// Write kernel info
			klib::kprintf(	u8"Kernel info:\r\n"
					u8"Arch:\t\t%s\r\n"
					u8"Start addr:\t0x%p\r\n"
					u8"End addr:\t0x%p\r\n"
					u8"Size:\t\t%d Kb.\r\n"
					u8"Build:\t\t" __DATE__ u8", " __TIME__ u8"\r\n"
					u8"Version:\tv%d.%d.%d [%s]\r\n"
					u8"Author:\t\tIgor Baklykov (c) %d - %d\r\n"
					u8"Command line:\t%s\r\n"
					u8"Loader:\t\t%s\r\n",
					(IGROS_ARCH),
					&_SECTION_KERNEL_START_,
					&_SECTION_KERNEL_END_,
					static_cast<dword_t>(&_SECTION_KERNEL_END_ - &_SECTION_KERNEL_START_) >> 10,
					IGROS_VERSION_MAJOR,
					IGROS_VERSION_MINOR,
					IGROS_VERSION_BUILD,
					IGROS_VERSION_NAME,
					2017,
					2020,
					multiboot->commandLine(),
					multiboot->loaderName());

			// Setup Interrupts Descriptor Table
			arch::idt::init();
			// Init exceptions
			arch::except::init();
			// Setup Global Descriptors Table
			arch::gdt::init();

			// Setup paging (And identity map first 4MB where kernel physically is)
			arch::paging::init();

			// Init interrupts
			arch::irq::init();
			// Enable interrupts
			arch::irq::enable();

			// Setup PIT
			//arch::pitSetup();
			// Setup keyboard
			//arch::keyboardSetup();
			// Setup RTC
			arch::rtcSetup();

/*
			// Test VBE
			if (multiboot->hasInfoVBE()) {
				// Get VBE config info
				auto config	= reinterpret_cast<multiboot::vbeConfig*>(multiboot->vbeControlInfo);
				// Get current VBE mode
				auto mode	= reinterpret_cast<multiboot::vbeMode*>(multiboot->vbeModeInfo);
				// Get OEM string
				auto oem	= reinterpret_cast<const char* const>(((config->oem & 0xFFFF0000) >> 12) + (config->oem & 0xFFFF));
				// Get available modes string
				auto modes	= reinterpret_cast<multiboot::vbeMode*>(((config->modes & 0xFFFF0000) >> 12) + (config->modes & 0xFFFF));
				// Get vendor string
				auto vendor	= reinterpret_cast<const char* const>(((config->vendor & 0xFFFF0000) >> 12) + (config->vendor & 0xFFFF));
				// Get product string
				auto product	= reinterpret_cast<const char* const>(((config->productName & 0xFFFF0000) >> 12) + (config->productName & 0xFFFF));
				// Get revision string
				auto revision	= reinterpret_cast<const char* const>(((config->productRev & 0xFFFF0000) >> 12) + (config->productRev & 0xFFFF));
				// Dump VBE
				klib::kprintf(	u8"VBE:\r\n"
						u8"Signature:\t%c%c%c%c\r\n"
						u8"Version:\t%d.%d\r\n"
						u8"OEM:\t\t%s\r\n"
						u8"Vendor name:\t%s\r\n"
						u8"Card name:\t%s\r\n"
						u8"Card rev.:\t%s\r\n"
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
						multiboot->vbeModeCurrent,
						mode->width,
						mode->height,
						mode->bpp,
						mode->physbase,
						static_cast<dword_t>(config->memory) * 64);

			}

			// Check if memory map exists
			if (multiboot->hasInfoMemoryMap()) {
				klib::kprintf(u8"Memory map available\r\n");
			}
*/

			// Check framebuffer
			if (multiboot->hasInfoFrameBuffer()) {
				// Dump FB
				klib::kprintf(	u8"FB:\r\n"
						u8"Current mode:\t(%dx%d, %dbpp, %d, %s)\r\n"
						u8"Address:\t0x%p\r\n",
						multiboot->fbWidth,
						multiboot->fbHeight,
						multiboot->fbBpp,
						multiboot->fbPitch,
						((0u == multiboot->fbType) ? u8"Indexed" : ((1u == multiboot->fbType) ? u8"RGB" : u8"Text")),
						static_cast<std::size_t>(multiboot->fbAddress));

/*
				// Get video memory
				auto pvMem = reinterpret_cast<byte_t*>(multiboot->fbAddress);
				auto vvMem = static_cast<byte_t*>(nullptr);
				//
				if (nullptr != pvMem) {
					//
					vvMem = reinterpret_cast<byte_t*>(0xFFFFFFFFE0000000);
					//
					arch::paging::map(reinterpret_cast<arch::page_t*>(pvMem), vvMem, arch::paging::flags_t::WRITABLE | arch::paging::flags_t::PRESENT);
					//
					for (auto i = 0; i < 10; i++) {
						auto j		= i * 3;
						vvMem[j + 0]	= 0xFF;
						vvMem[j + 1]	= 0xFF;
						vvMem[j + 2]	= 0xFF;
					}
				}
*/

			}

			// Write "Booted successfully" message
			klib::kprintf(u8"Booted successfully\r\n");

		}


#ifdef	__cplusplus

	}	// extern "C"

#endif	// __cplusplus


}	// namespace igros

