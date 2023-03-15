////////////////////////////////////////////////////////////////
//
//	Platform description for x86
//
//	File:	platform-i386.cpp
//	Date:	15 Mar 2023
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


// c++
#include <source_location>
// IgrOS-Kernel arch
#include <arch/types.hpp>
// IgrOS-Kernel arch platform
#include <arch/platform/platform.hpp>
// IgrOS-Kernel arch i386
#include <arch/i386/idt.hpp>
#include <arch/i386/exceptions.hpp>
#include <arch/i386/gdt.hpp>
#include <arch/i386/paging.hpp>
#include <arch/i386/irq.hpp>
#include <arch/i386/fpu.hpp>
// IgrOS-Kernel drivers
#include <drivers/clock/pit.hpp>
#include <drivers/clock/rtc.hpp>
#include <drivers/input/keyboard.hpp>
#include <drivers/uart/serial.hpp>
#include <drivers/vga/vmem.hpp>
// IgrOS-Kernel library
#include <klib/kprint.hpp>


// i386 namespace
namespace igros::i386 {


	// Initialize i386
	static void i386Init() noexcept {

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

		// Setup VGA
		arch::vmemInit();
		// Setup UART (#1, 115200 8N1)
		arch::serialSetup();
		// Setup keyboard
		arch::keyboardSetup();
		// Setup RTC
		arch::rtcSetup();
		// Setup PIT
		//arch::pitSetup();

		// Debug print
		klib::kprintf(
			"Platform i386 initialized...\n[%s]\n",
			std::source_location::current().function_name()
		);

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

