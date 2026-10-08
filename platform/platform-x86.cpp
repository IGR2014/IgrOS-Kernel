////////////////////////////////////////////////////////////////
//
//	Platform description for x86 family (i386 and x86_64)
//
//	File:	platform-x86.cpp
//	Date:	22 Mar 2023
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


// C++
#include <source_location>
// IgrOS-Kernel arch x86
#include <arch/x86/exceptions.hpp>
#include <arch/x86/irq.hpp>
#include <arch/x86/native.hpp>

#if	defined (IGROS_ARCH_i386)

// IgrOS-Kernel arch i386
#include <arch/i386/gdt.hpp>
#include <arch/i386/idt.hpp>
#include <arch/i386/paging.hpp>

#elif	defined (IGROS_ARCH_x86_64)

// IgrOS-Kernel arch x86_64
#include <arch/x86_64/gdt.hpp>
#include <arch/x86_64/idt.hpp>
#include <arch/x86_64/paging.hpp>

#endif

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


// x86 namespace
namespace igros::x86 {


	// Initialize x86
	static void platformInit() noexcept {

		// Setup Interrupts Descriptor Table
		native::idt::init();
		// Init exceptions
		except::init();
		// Setup Global Descriptors Table
		native::gdt::init();

		// Init interrupts
		irq::init();
		// Enable interrupts
		irq::enable();

		// Setup paging (And identity map first 4MB where kernel physically is)
		//native::paging::init();

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

	// Finalize x86
	static void platformFinalize() noexcept {
		// Debug print
		klib::kprintf(
			"[%s]\n",
			std::source_location::current().function_name()
		);
	}

	// Shutdown x86
	static void platformShutdown() noexcept {
		// Debug print
		klib::kprintf(
			"[%s]\n",
			std::source_location::current().function_name()
		);
	}

	// Reboot x86
	static void platformReboot() noexcept {
		// Debug print
		klib::kprintf(
			"[%s]\n",
			std::source_location::current().function_name()
		);
	}

	// Suspend x86
	static void platformSuspend() noexcept {
		// Debug print
		klib::kprintf(
			"[%s]\n",
			std::source_location::current().function_name()
		);
	}

	// Wakeup x86
	static void platformWakeup() noexcept {
		// Debug print
		klib::kprintf(
			"[%s]\n",
			std::source_location::current().function_name()
		);
	}


}	// namespace igros::x86


// OS platform
namespace igros::platform {


	// Get kernel current platform
	[[nodiscard]]
	auto Platform::current() noexcept -> const Platform& {
		// Current x86 platform reference
		return klib::kSingleton<Platform>::get(
			x86::ARCH_NAME,
			x86::platformInit,
			x86::platformFinalize,
			x86::platformShutdown,
			x86::platformReboot,
			x86::platformSuspend,
			x86::platformWakeup
		);
	}


}	// namespace igros::platform

