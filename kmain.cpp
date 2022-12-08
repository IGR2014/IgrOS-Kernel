////////////////////////////////////////////////////////////////
//
//	Boot low-level main setup function
//
//	File:	boot.cpp
//	Date:	05 Dec 2022
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


// IgrOS-Kernel Common
#include <version.hpp>
#include <multiboot.hpp>
#include <platform.hpp>
#include <flags.hpp>
// IgrOS-Kernel arch
#include <arch/types.hpp>
#include <arch/cpu.hpp>
// IgrOS-Kernel drivers
#include <drivers/vga/vmem.hpp>
#include <drivers/input/keyboard.hpp>
#include <drivers/clock/pit.hpp>
#include <drivers/clock/rtc.hpp>
#include <drivers/uart/serial.hpp>
// IgrOS-Kernel library
#include <klib/kstring.hpp>
#include <klib/kprint.hpp>
// IgrOS-Kernel memory
#include <mem/mmap.hpp>


// OS namesapce
namespace igros {


	// Print kernel header
	void printHeader(const multiboot::info_t* const multiboot) noexcept {
		// Write kernel info
		klib::kprintf(
R"info(
Kernel info:
Arch:		%s
Start addr:	0x%p
End addr:	0x%p
Size:		%d Kb.
Build:		%s, %s
Version:	%s
Author:		Igor Baklykov (c) %d - %d
Loader:		"%s"
Command line:	"%s"
)info",
			platform::CURRENT_PLATFORM.name(),
			platform::KERNEL_START(),
			platform::KERNEL_END(),
			platform::KERNEL_SIZE() >> 10,
			__DATE__,
			__TIME__,
			KERNEL_VERSION_STRING(),
			2017,
			2022,
			multiboot->loaderName(),
			multiboot->commandLine()
		);
	}


}	// namespace igros


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus


	// Kernel main function
	void kmain(const igros::multiboot::info_t* const multiboot, const igros::igros_dword_t magic) noexcept {

		// Init VGA memory
		igros::arch::vmemInit();

		// Write Multiboot magic error message message
		igros::klib::kprintf("IgrOS kernel");

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
		igros::klib::kprintf("Booted successfully");

		// Halt CPU
		igros::arch::cpu::get().halt();

	}


#ifdef	__cplusplus

}	// extern "C"

#endif	// __cplusplus

