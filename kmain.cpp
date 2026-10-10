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


// C++
#include <type_traits>
// IgrOS-Kernel arch
#include <arch/cpu.hpp>
// IgrOS-Kernel memory
#include <mem/phys.hpp>
// IgrOS-Kernel multiboot
#include <multiboot/multiboot.hpp>
// IgrOS-Kernel platform
#include <platform/platform.hpp>


#ifdef	__cplusplus

extern "C" {

#endif	// __cplusplus


	// Global constructors (from linker script)
	extern const std::add_pointer_t<void ()>	_SECTION_INIT_ARRAY_START_[];
	extern const std::add_pointer_t<void ()>	_SECTION_INIT_ARRAY_END_[];


	// Kernel main function
	[[noreturn]]
	void kmain(const igros::multiboot::info_t* const multiboot, const igros::igros_dword_t magic) noexcept {

		// Run global constructors (dynamic initialization of global objects)
		for (auto ctor {_SECTION_INIT_ARRAY_START_}; ctor != _SECTION_INIT_ARRAY_END_; ++ctor) {
			(*ctor)();
		}

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

		// Initialize physical memory allocator
		igros::mem::phys::init(*multiboot);
		igros::klib::kprintf("Physical memory:\t%zu KB free", igros::mem::phys::freePages() << 2);

		// Write "Booted successfully" message
		igros::klib::kprintf("Booted successfully");

		// Halt CPU
		igros::arch::cpu::halt();

	}


#ifdef	__cplusplus

}	// extern "C"

#endif	// __cplusplus

