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
// IgrOS-Kernel library
#include <klib/kflags.hpp>
#include <klib/kprint.hpp>
#include <klib/kstring.hpp>
#include <klib/kversion.hpp>
// IgrOS-Kernel memory
#include <mem/mmap.hpp>
// IgrOS-Kernel multiboot
#include <multiboot/multiboot.hpp>


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus


	// Kernel main function
	[[noreturn]]
	void kmain(const igros::multiboot::info_t* const multiboot, const igros::igros_dword_t magic) noexcept {

		// Initialize platform
		igros::platform::CURRENT_PLATFORM.initialize();

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

