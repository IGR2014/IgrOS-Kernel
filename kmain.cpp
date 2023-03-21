////////////////////////////////////////////////////////////////
//
//	Boot low-level main setup function
//
//	File:	boot.cpp
//	Date:	21 Mar 2023
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


// IgrOS-Kernel arch
#include <arch/cpu.hpp>
// IgrOS-Kernel multiboot
#include <multiboot/multiboot.hpp>
// IgrOS-Kernel platform
#include <platform/platform.hpp>


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus


	// Kernel main function
	[[noreturn]]
	void kmain(const igros::multiboot::info_t* const multiboot, const igros::igros_dword_t magic) noexcept {

		// Initialize platform
		igros::platform::Platform::current().initialize();

		// Write Multiboot magic error message message
		igros::klib::kprintf("IgrOS kernel");

		// Test multiboot (Hang on error)
		multiboot->test(magic);
		// Print kernel header
		multiboot->printHeader();
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

