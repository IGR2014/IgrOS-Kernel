////////////////////////////////////////////////////////////////
//
//	Drivers list initialization tests
//
//	File:	test_driver.cpp
//	Date:	28 Oct 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


// C++
#include <array>
// IgrOS-Kernel drivers
#include <drivers/driver.hpp>
// Tests
#include <tests/hw.hpp>


namespace hw = igros::test::hw;


// Initialized drivers count
static auto initialized {0};


int main() {

	// Every driver initialized in order, failures counted and reported
	constexpr std::array<igros::drivers::driver_t, 3> DRIVERS {{
		{"first",	[]() noexcept { return 1 == ++initialized; }},
		{"broken",	[]() noexcept { ++initialized; return false; }},
		{"last",	[]() noexcept { return 3 == ++initialized; }},
	}};
	hw::reset();
	IGROS_CHECK(1 == igros::drivers::init(DRIVERS));
	IGROS_CHECK(3 == initialized);
	IGROS_CHECK("Driver first:\tOK\nDriver broken:\tERROR\nDriver last:\tOK\n" == hw::console);

	return igros::test::result();

}

