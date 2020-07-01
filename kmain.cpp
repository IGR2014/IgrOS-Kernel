////////////////////////////////////////////////////////////////
//
//	Boot low-level main setup function
//
//	File:	boot.cpp
//	Date:	28 Jun 2020
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

			// Test VBE
			if (multiboot->hasInfoVBE()) {
				// Get VBE config info
				auto config	= reinterpret_cast<multiboot::vbeConfig*>(multiboot->vbeControlInfo);
				// Get current VBE mode
				auto mode	= reinterpret_cast<multiboot::vbeMode*>(multiboot->vbeModeInfo);
				// Get OEM string
				auto oem	= reinterpret_cast<multiboot::vbeMode*>(((config->oem & 0xFFFF0000) >> 12) + (config->oem & 0xFFFF));
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
						u8"Current mode:\t#%d (%dx%d, %dbpp, 0x%p)\r\n",
						config->signature[0],
						config->signature[1],
						config->signature[2],
						config->signature[3],
						(config->version >> 8) & 0xFF,
						config->version & 0xFF,
						oem,
						vendor,
						product,
						revision,
						multiboot->vbeModeCurrent,
						mode->resX,
						mode->resY,
						mode->bpp,
						mode->physbase);

				// Get video memory
				auto vMem	= reinterpret_cast<byte_t*>(mode->physbase);//reinterpret_cast<byte_t*>(((mode->physbase & 0xFFFF0000) >> 12) + (mode->physbase & 0xFFFF));
				//
				for (int i = 0; i < 100000; i++) {
					auto j		= i * 3;
					vMem[j + 0]	= 0x00;
					vMem[j + 1]	= 0xFF;
					vMem[j + 2]	= 0x00;
				}

			}
			// Check framebuffer
			if (multiboot->hasInfoFrameBuffer()) {
				klib::kprintf(u8"Framebuffer available\r\n");
			}

	/*
			// Check if memory map exists
			if (multiboot->hasInfoMemoryMap()) {

				// Write pre-alloc message
				klib::kprintf(u8"Memory page allocation...");
				// Setup physical memory map
				mem::phys::init(reinterpret_cast<multiboot::memoryMapEntry_t*>(multiboot->mmapAddr), multiboot->mmapAddr + multiboot->mmapLength);
				// Allocate one page
				auto pg = mem::phys::alloc();
				// Write "Booted successfully" message
				klib::kprintf(u8"Phys. page at:\t0x%p", pg);

				reinterpret_cast<dword_t*>(pg)[0] = 45u;

				// Free page
				mem::phys::free(pg);
				// Write "Booted successfully" message
				klib::kprintf(u8"Phys. page at:\t0x%p\r\n", pg);

			}
	*/

			// Write "Booted successfully" message
			klib::kprintf(u8"Booted successfully\r\n");

		}


#ifdef	__cplusplus

	}	// extern "C"

#endif	// __cplusplus


}	// namespace igros

