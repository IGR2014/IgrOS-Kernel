////////////////////////////////////////////////////////////////
//
//	Host stub for VGA text output: console capture (see hw.hpp)
//
//	File:	vmem.cpp
//	Date:	19 Oct 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


// IgrOS-Kernel drivers
#include <drivers/vga/vmem.hpp>
// Tests
#include <tests/hw.hpp>


namespace igros::arch {


	void vmemWrite(const char symbol) noexcept {
		test::hw::console += symbol;
	}

	void vmemWrite(const char* const message) noexcept {
		test::hw::console += message;
	}

	void vmemWrite(const char* const message, const igros_usize_t size) noexcept {
		test::hw::console.append(message, size);
	}


}	// namespace igros::arch

