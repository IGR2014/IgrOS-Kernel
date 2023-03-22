////////////////////////////////////////////////////////////////
//
//	Platform description for x86_64
//
//	File:	platform-x86_64.cpp
//	Date:	22 Mar 2023
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


// C++
#include <source_location>
// IgrOS-Kernel arch x86_64
#include <arch/x86_64/exceptions.hpp>
#include <arch/x86_64/gdt.hpp>
#include <arch/x86_64/idt.hpp>
#include <arch/x86_64/irq.hpp>
#include <arch/x86_64/paging.hpp>
// IgrOS-Kernel drivers
#include <drivers/clock/pit.hpp>
#include <drivers/clock/rtc.hpp>
#include <drivers/input/keyboard.hpp>
#include <drivers/uart/serial.hpp>
#include <drivers/vga/vmem.hpp>
// IgrOS-Kernel library
#include <klib/kprint.hpp>
#include <klib/kSingleton.hpp>
// IgrOS-Kernel platform
#include <platform/platform.hpp>


// x86_64 namespace
namespace igros::x86_64 {


	// Initialize x86_64
	void platformInit() noexcept {

		// Setup Interrupts Descriptor Table
		x86_64::idt::init();
		// Init exceptions
		x86_64::except::init();
		// Setup Global Descriptors Table
		x86_64::gdt::init();

		// Init interrupts
		x86_64::irq::init();
		// Enable interrupts
		x86_64::irq::enable();

		// Setup paging (And identity map first 4MB where kernel physically is)
		//x86_64::paging::init();

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
			"[%s]\n",
			std::source_location::current().function_name()
		);

	}

	// Finalize x86_64
	void platformFinalize() noexcept {
		// Debug print
		klib::kprintf(
			"[%s]\n",
			std::source_location::current().function_name()
		);
	}

	// Shutdown x86_64
	void platformShutdown() noexcept {
		// Debug print
		klib::kprintf(
			"[%s]\n",
			std::source_location::current().function_name()
		);
	}

	// Reboot x86_64
	void platformReboot() noexcept {
		// Debug print
		klib::kprintf(
			"[%s]\n",
			std::source_location::current().function_name()
		);
	}

	// Suspend x86_64
	void platformSuspend() noexcept {
		// Debug print
		klib::kprintf(
			"[%s]\n",
			std::source_location::current().function_name()
		);
	}

	// Wakeup x86_64
	void platformWakeup() noexcept {
		// Debug print
		klib::kprintf(
			"[%s]\n",
			std::source_location::current().function_name()
		);
	}


}	// namespace igros::x86_64


// OS platform
namespace igros::platform {


	// Get kernel current platform
	[[nodiscard]]
	auto Platform::current() noexcept -> const Platform& {
		// Current x86_64 platform reference
		return klib::kSingleton<Platform>::get(
			"x86_64",
			x86_64::platformInit,
			x86_64::platformFinalize,
			x86_64::platformShutdown,
			x86_64::platformReboot,
			x86_64::platformSuspend,
			x86_64::platformWakeup
		);
	}


}	// namespace igros::platform

