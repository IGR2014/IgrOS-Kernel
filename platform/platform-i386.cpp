////////////////////////////////////////////////////////////////
//
//	Platform description for x86
//
//	File:	platform-i386.cpp
//	Date:	21 Mar 2023
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


// c++
#include <source_location>
// IgrOS-Kernel arch i386
#include <arch/i386/exceptions.hpp>
#include <arch/i386/fpu.hpp>
#include <arch/i386/gdt.hpp>
#include <arch/i386/idt.hpp>
#include <arch/i386/irq.hpp>
#include <arch/i386/paging.hpp>
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


// i386 namespace
namespace igros::i386 {


	// Initialize i386
	static void platformInit() noexcept {

		// Setup Interrupts Descriptor Table
		i386::idt::init();
		// Init exceptions
		i386::except::init();
		// Setup Global Descriptors Table
		i386::gdt::init();

		// Init interrupts
		i386::irq::init();
		// Enable interrupts
		i386::irq::enable();

		// Setup paging (And identity map first 4MB where kernel physically is)
		i386::paging::init();

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

	// Finalize i386
	static void platformFinalize() noexcept {
		// Debug print
		klib::kprintf(
			"[%s]\n",
			std::source_location::current().function_name()
		);
	}

	// Shutdown i386
	static void platformShutdown() noexcept {
		// Debug print
		klib::kprintf(
			"[%s]\n",
			std::source_location::current().function_name()
		);
	}

	// Reboot i386
	static void platformReboot() noexcept {
		// Debug print
		klib::kprintf(
			"[%s]\n",
			std::source_location::current().function_name()
		);
	}

	// Suspend i386
	static void platformSuspend() noexcept {
		// Debug print
		klib::kprintf(
			"[%s]\n",
			std::source_location::current().function_name()
		);
	}

	// Wakeup i386
	static void platformWakeup() noexcept {
		// Debug print
		klib::kprintf(
			"[%s]\n",
			std::source_location::current().function_name()
		);
	}


}	// namespace igros::i386


// OS platform
namespace igros::platform {


	// Get kernel current platform
	[[nodiscard]]
	auto Platform::current() noexcept -> const Platform& {
		// Current i386 platform reference
		return klib::kSingleton<Platform>::get(
			"i386",
			i386::platformInit,
			i386::platformFinalize,
			i386::platformShutdown,
			i386::platformReboot,
			i386::platformSuspend,
			i386::platformWakeup
		);
	}


}	// namespace igros::platform

