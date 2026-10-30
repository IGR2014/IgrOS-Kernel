////////////////////////////////////////////////////////////////
//
//	Boot low-level main setup function
//
//	File:	boot.cpp
//	Date:	30 Oct 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


// C++
#include <span>
#include <type_traits>
// IgrOS-Kernel arch
#include <arch/cpu.hpp>
// IgrOS-Kernel memory
#include <mem/direct.hpp>
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
	void kmain(const igros::igros_usize_t multibootAddress, const igros::igros_dword_t magic) noexcept {

		// Run global constructors (dynamic initialization of global objects)
		for (const auto ctor : std::span {_SECTION_INIT_ARRAY_START_, _SECTION_INIT_ARRAY_END_}) {
			ctor();
		}

		// Multiboot info at physical address (screen is reachable through boot page tables, before platform init)
		const auto* const multiboot {igros::mem::direct::pointer<const igros::multiboot::info_t>(multibootAddress)};
		if (nullptr == multiboot) [[unlikely]] {
			igros::klib::kprintf("Multiboot info at 0x%zx is not directly mapped", multibootAddress);
			igros::arch::cpu::halt();
		}

		// Test multiboot before anything reads multiboot info (hang on error)
		multiboot->test(magic);

		// Initialize physical memory allocator (needed by paging)
		igros::mem::phys::init(*multiboot);

		// Initialize platform
		igros::platform::Platform::current().initialize();

		// Write greetings message (screen is reachable through platform init)
		igros::klib::kprintf("IgrOS kernel");

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

		// Show free physical memory
		igros::klib::kprintf("Physical memory:\t%zu KB free", igros::mem::phys::freePages() << 2);

		// Write "Booted successfully" message
		igros::klib::kprintf("Booted successfully");

		// Halt CPU
		igros::arch::cpu::halt();

	}


#ifdef	__cplusplus

}	// extern "C"

#endif	// __cplusplus

