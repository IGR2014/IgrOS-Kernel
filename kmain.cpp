////////////////////////////////////////////////////////////////
//
//	Boot low-level main setup function
//
//	File:	boot.cpp
//	Date:	10 Feb 2021
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


// Common
#include <version.hpp>
#include <multiboot.hpp>
#include <platform.hpp>
#include <flags.hpp>

// Architecture dependent
#include <arch/types.hpp>
#include <arch/cpu.hpp>

// Kernel drivers
#include <drivers/vga/vmem.hpp>
#include <drivers/input/keyboard.hpp>
#include <drivers/clock/pit.hpp>
#include <drivers/clock/rtc.hpp>
#include <drivers/uart/serial.hpp>

// Kernel library
#include <klib/kstring.hpp>
#include <klib/kprint.hpp>

// Kernel memory
#include <mem/mmap.hpp>


// OS namesapce
namespace igros {


	// Print kernel header
	void printHeader(const multiboot::info_t* const multiboot) noexcept {
		// Write kernel info
		klib::kprintf(
			u8"Kernel info:\r\n"
			u8"Arch:\t\t%s\r\n"
			u8"Start addr:\t0x%p\r\n"
			u8"End addr:\t0x%p\r\n"
			u8"Size:\t\t%d Kb.\r\n"
			u8"Build:\t\t" __DATE__ u8", " __TIME__ u8"\r\n"
			u8"Version:\t%s\r\n"
			u8"Author:\t\tIgor Baklykov (c) %d - %d\r\n"
			u8"Command line:\t%s\r\n"
			u8"Loader:\t\t%s\r\n",
			platform::CURRENT_PLATFORM.name(),
			platform::KERNEL_START(),
			platform::KERNEL_END(),
			platform::KERNEL_SIZE() >> 10,
			KERNEL_VERSION_STRING(),
			2017,
			2021,
			multiboot->commandLine(),
			multiboot->loaderName()
		);
	}


}	// namespace igros


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus


	// Kernel main function
	void kmain(const igros::multiboot::info_t* const multiboot, const igros::dword_t magic) noexcept {

		// Init VGA memory
		igros::arch::vmemInit();

		// Write Multiboot magic error message message
		igros::klib::kprintf(u8"IgrOS kernel\r\n");

		// Test multiboot (Hang on error)
		igros::multiboot::test(multiboot, magic);

		// Print kernel header
		igros::printHeader(multiboot);

		// Initialize platform
		igros::platform::CURRENT_PLATFORM.initialize();

		// Setup PIT
		//igros::arch::pitSetup();
		// Setup keyboard
		igros::arch::keyboardSetup();
		// Setup UART (#1, 115200 8N1)
		igros::arch::serialSetup();
		// Setup RTC
		igros::arch::rtcSetup();

		// Show multiboot flags
		multiboot->printFlags();
		// Show VBE info
		multiboot->printVBEInfo();
		// Show framebuffer info
		multiboot->printFBInfo();
		// Show memory map
		multiboot->printMemMap();

		// Write "Booted successfully" message
		igros::klib::kprintf(u8"Booted successfully\r\n");

		// Halt CPU
		igros::arch::cpu::get().halt();

	}


#ifdef	__cplusplus

}	// extern "C"

#endif	// __cplusplus

