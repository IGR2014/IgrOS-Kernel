////////////////////////////////////////////////////////////////
//
//	Platform description for x86_64
//
//	File:	platform-x86_64.cpp
//	Date:	14 Jul 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
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


// Kernel start and end
extern const igros::byte_t _SECTION_KERNEL_START_;
extern const igros::byte_t _SECTION_KERNEL_END_;


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

	}

	// Finalize x86_64
	void x86_64Finalize() noexcept {
		klib::kprintf(u8"%s:\tNot implemented yet!", __func__);
	}

	// Shutdown x86_64
	void x86_64Shutdown() noexcept {
		klib::kprintf(u8"%s:\tNot implemented yet!", __func__);
	}

	// Reboot x86_64
	void x86_64Reboot() noexcept {
		klib::kprintf(u8"%s:\tNot implemented yet!", __func__);
	}

	// Suspend x86_64
	void x86_64Suspend() noexcept {
		klib::kprintf(u8"%s:\tNot implemented yet!", __func__);
	}

	// Wakeup x86_64
	void x86_64Wakeup() noexcept {
		klib::kprintf(u8"%s:\tNot implemented yet!", __func__);
	}


}	// namespace igros::x86_64


// OS platform
namespace igros::platform {


	// Platform description
	const description_t CURRENT_PLATFORM = {
		u8"x86_64",
		x86_64::x86_64Init,
		x86_64::x86_64Finalize,
		x86_64::x86_64Shutdown,
		x86_64::x86_64Reboot,
		x86_64::x86_64Suspend,
		x86_64::x86_64Wakeup
	};


}	// namespace igros::platform

