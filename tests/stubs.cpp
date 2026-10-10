////////////////////////////////////////////////////////////////
//
//	Host stubs for kernel functions not under test
//
//	File:	stubs.cpp
//	Date:	11 Oct 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


// IgrOS-Kernel drivers
#include <drivers/uart/serial.hpp>
#include <drivers/vga/vmem.hpp>


// Console output (kprintf) goes nowhere on host
namespace igros::arch {


	void vmemWrite([[maybe_unused]] const char symbol) noexcept {}
	void vmemWrite([[maybe_unused]] const char* const message) noexcept {}
	void vmemWrite([[maybe_unused]] const char* const message, [[maybe_unused]] const igros_usize_t size) noexcept {}

	auto serialWrite([[maybe_unused]] const char* const src, const igros_usize_t size) noexcept -> igros_usize_t {
		return size;
	}

	auto serialWrite([[maybe_unused]] const char* const src) noexcept -> igros_usize_t {
		return 0;
	}


}	// namespace igros::arch


// Interrupts state (irq::guard) - nothing to save on host
extern "C" {


	auto irqSave() noexcept -> igros::igros_usize_t {
		return 0;
	}

	void irqRestore([[maybe_unused]] const igros::igros_usize_t flags) noexcept {}


}	// extern "C"

