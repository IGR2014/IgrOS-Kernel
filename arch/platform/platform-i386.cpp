////////////////////////////////////////////////////////////////
//
//	Platform description for x86
//
//	File:	platform-i386.cpp
//	Date:	12 Dec 2022
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


#include <source_location>

#include <platform.hpp>

#include <arch/types.hpp>

#include <arch/i386/idt.hpp>
#include <arch/i386/exceptions.hpp>
#include <arch/i386/gdt.hpp>
#include <arch/i386/paging.hpp>
#include <arch/i386/irq.hpp>
#include <arch/i386/fpu.hpp>

#include <klib/kprint.hpp>


// i386 namespace
namespace igros::i386 {


	// Initialize i386
	static void i386Init() noexcept {

		// Print
		klib::kprintf(
			"Initializing i386 platform...\n[%s]\n",
			std::source_location::current().function_name()
		);

		// Setup Interrupts Descriptor Table
		i386::idt::init();
		// Init exceptions
		i386::except::init();
		// Setup Global Descriptors Table
		i386::gdt::init();

		// Setup paging (And identity map first 4MB where kernel physically is)
		i386::paging::init();

		// Init interrupts
		i386::irq::init();
		// Enable interrupts
		i386::irq::enable();

		// Check FPU
		i386::fpu::check();

	}

	// Finalize i386
	static void i386Finalize() noexcept {
		klib::kprintf(
			"Not implemented yet!\n[%s]\n",
			std::source_location::current().function_name()
		);
	}

	// Shutdown i386
	static void i386Shutdown() noexcept {
		klib::kprintf(
			"Not implemented yet!\n[%s]\n",
			std::source_location::current().function_name()
		);
	}

	// Reboot i386
	static void i386Reboot() noexcept {
		klib::kprintf(
			"Not implemented yet!\n[%s]\n",
			std::source_location::current().function_name()
		);
	}

	// Suspend i386
	static void i386Suspend() noexcept {
		klib::kprintf(
			"Not implemented yet!\n[%s]\n",
			std::source_location::current().function_name()
		);
	}

	// Wakeup i386
	static void i386Wakeup() noexcept {
		klib::kprintf(
			"Not implemented yet!\n[%s]\n",
			std::source_location::current().function_name()
		);
	}


}	// namespace igros::i386


// OS platform
namespace igros::platform {


	// Platform description
	const description_t CURRENT_PLATFORM {
		"i386",
		i386::i386Init,
		i386::i386Finalize,
		i386::i386Shutdown,
		i386::i386Reboot,
		i386::i386Suspend,
		i386::i386Wakeup
	};


}	// namespace igros::platform

