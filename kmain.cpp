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


// IgrOS-Kernel arch
#include <arch/cpu.hpp>
#include <arch/types.hpp>
// IgrOS-Kernel arch platform
#include <arch/platform/platform.hpp>
// IgrOS-Kernel drivers
#include <drivers/clock/pit.hpp>
#include <drivers/clock/rtc.hpp>
#include <drivers/input/keyboard.hpp>
#include <drivers/uart/serial.hpp>
#include <drivers/vga/vmem.hpp>
// IgrOS-Kernel library
#include <klib/kflags.hpp>
#include <klib/kprint.hpp>
#include <klib/kstring.hpp>
#include <klib/kversion.hpp>
// IgrOS-Kernel memory
#include <mem/mmap.hpp>
// IgrOS-Kernel multiboot
#include <multiboot/multiboot.hpp>


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

