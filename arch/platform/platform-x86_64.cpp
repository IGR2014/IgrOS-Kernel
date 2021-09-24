////////////////////////////////////////////////////////////////
//
//	Platform description for x86_64
//
//	File:	platform-x86_64.cpp
//	Date:	24 Sep 2021
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#include <platform.hpp>

#include <arch/x86_64/types.hpp>
#include <arch/x86_64/idt.hpp>
#include <arch/x86_64/exceptions.hpp>
#include <arch/x86_64/gdt.hpp>
#include <arch/x86_64/paging.hpp>
#include <arch/x86_64/irq.hpp>

#include <klib/kprint.hpp>


// x86_64 namespace
namespace igros::x86_64 {


	// Initialize x86_64
	void x86_64Init() noexcept {

		klib::kprintf("Initializing x86_64 platform...", __func__);

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

	}

	// Finalize x86_64
	void x86_64Finalize() noexcept {
		klib::kprintf("%s:\tNot implemented yet!", __func__);
	}

	// Shutdown x86_64
	void x86_64Shutdown() noexcept {
		klib::kprintf("%s:\tNot implemented yet!", __func__);
	}

	// Reboot x86_64
	void x86_64Reboot() noexcept {
		klib::kprintf("%s:\tNot implemented yet!", __func__);
	}

	// Suspend x86_64
	void x86_64Suspend() noexcept {
		klib::kprintf("%s:\tNot implemented yet!", __func__);
	}

	// Wakeup x86_64
	void x86_64Wakeup() noexcept {
		klib::kprintf("%s:\tNot implemented yet!", __func__);
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

