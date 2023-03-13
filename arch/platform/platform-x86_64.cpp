////////////////////////////////////////////////////////////////
//
//	Platform description for x86_64
//
//	File:	platform-x86_64.cpp
//	Date:	14 Mar 2023
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


// C++
#include <source_location>
// IgrOS-Kernel arch
#include <arch/types.hpp>
// IgrOS-Kernel arch platform
#include <arch/platform/platform.hpp>
// IgrOS-Kernel arch x86_64
#include <arch/x86_64/idt.hpp>
#include <arch/x86_64/exceptions.hpp>
#include <arch/x86_64/gdt.hpp>
#include <arch/x86_64/paging.hpp>
#include <arch/x86_64/irq.hpp>
// IgrOS-Kernel drivers
#include <drivers/clock/pit.hpp>
#include <drivers/clock/rtc.hpp>
#include <drivers/input/keyboard.hpp>
#include <drivers/uart/serial.hpp>
#include <drivers/vga/vmem.hpp>
// IgrOS-Kernel library
#include <klib/kprint.hpp>


// x86_64 namespace
namespace igros::x86_64 {


	// Initialize x86_64
	void x86_64Init() noexcept {

		// Setup Interrupts Descriptor Table
		x86_64::idt::init();
		// Init exceptions
		x86_64::except::init();
		// Setup Global Descriptors Table
		x86_64::gdt::init();

		// Setup paging (And identity map first 4MB where kernel physically is)
		x86_64::paging::init();

		// Init interrupts
		x86_64::irq::init();
		// Enable interrupts
		x86_64::irq::enable();

		// Setup VGA
		arch::vmemInit();
		// Setup keyboard
		arch::keyboardSetup();
		// Setup UART (#1, 115200 8N1)
		arch::serialSetup();
		// Setup RTC
		arch::rtcSetup();
		// Setup PIT
		//arch::pitSetup();

		// Debug print
		klib::kprintf(
			"Platform x86_64 initialized...\n[%s]\n",
			std::source_location::current().function_name()
		);

	}

	// Finalize x86_64
	void x86_64Finalize() noexcept {
		klib::kprintf(
			"Not implemented yet!\n[%s]\n",
			std::source_location::current().function_name()
		);
	}

	// Shutdown x86_64
	void x86_64Shutdown() noexcept {
		klib::kprintf(
			"Not implemented yet!\n[%s]\n",
			std::source_location::current().function_name()
		);
	}

	// Reboot x86_64
	void x86_64Reboot() noexcept {
		klib::kprintf(
			"Not implemented yet!\n[%s]\n",
			std::source_location::current().function_name()
		);
	}

	// Suspend x86_64
	void x86_64Suspend() noexcept {
		klib::kprintf(
			"Not implemented yet!\n[%s]\n",
			std::source_location::current().function_name()
		);
	}

	// Wakeup x86_64
	void x86_64Wakeup() noexcept {
		klib::kprintf(
			"Not implemented yet!\n[%s]\n",
			std::source_location::current().function_name()
		);
	}


}	// namespace igros::x86_64


// OS platform
namespace igros::platform {


	// Platform description
	const description_t CURRENT_PLATFORM {
		"x86_64",
		x86_64::x86_64Init,
		x86_64::x86_64Finalize,
		x86_64::x86_64Shutdown,
		x86_64::x86_64Reboot,
		x86_64::x86_64Suspend,
		x86_64::x86_64Wakeup
	};


}	// namespace igros::platform

