////////////////////////////////////////////////////////////////
//
//	Host stub for serial output (console is captured by VGA stub)
//
//	File:	serial.cpp
//	Date:	19 Oct 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


// IgrOS-Kernel drivers
#include <drivers/uart/serial.hpp>


namespace igros::arch {


	auto serialWrite([[maybe_unused]] const char* const src, const igros_usize_t size) noexcept -> igros_usize_t {
		return size;
	}

	auto serialWrite([[maybe_unused]] const char* const src) noexcept -> igros_usize_t {
		return 0;
	}


}	// namespace igros::arch

