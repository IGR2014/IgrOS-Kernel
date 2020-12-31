////////////////////////////////////////////////////////////////
//
//	Platform description for x86
//
//	File:	platform-i386.cpp
//	Date:	17 Jul 2020
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#include <platform.hpp>

#include <arch/i386/types.hpp>
#include <arch/i386/idt.hpp>
#include <arch/i386/exceptions.hpp>
#include <arch/i386/gdt.hpp>
#include <arch/i386/paging.hpp>
#include <arch/i386/irq.hpp>

#include <klib/kprint.hpp>


// i386 namespace
namespace igros::i386 {


	// Initialize i386
	void i386Init() noexcept {

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

	}

	// Finalize i386
	void i386Finalize() noexcept {
		klib::kprintf(u8"%s:\tNot implemented yet!", __func__);
	}

	// Shutdown i386
	void i386Shutdown() noexcept {
		klib::kprintf(u8"%s:\tNot implemented yet!", __func__);
	}

	// Reboot i386
	void i386Reboot() noexcept {
		klib::kprintf(u8"%s:\tNot implemented yet!", __func__);
	}

	// Suspend i386
	void i386Suspend() noexcept {
		klib::kprintf(u8"%s:\tNot implemented yet!", __func__);
	}

	// Wakeup i386
	void i386Wakeup() noexcept {
		klib::kprintf(u8"%s:\tNot implemented yet!", __func__);
	}


}	// namespace igros::i386


// OS platform
namespace igros::platform {


	// Platform description
	const description_t CURRENT_PLATFORM = {
		u8"i386",
		i386::i386Init,
		i386::i386Finalize,
		i386::i386Shutdown,
		i386::i386Reboot,
		i386::i386Suspend,
		i386::i386Wakeup
	};


}	// namespace igros::platform

