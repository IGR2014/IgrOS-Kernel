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


auto main() -> int {

	// Every driver initialized in order, failures counted and reported
	constexpr auto DRIVERS = std::to_array<igros::drivers::driver_t>({
		{.name = "first",	.init = []() noexcept -> bool { return 1 == ++initialized; }},
		{.name = "broken",	.init = []() noexcept -> bool { ++initialized; return false; }},
		{.name = "last",	.init = []() noexcept -> bool { return 3 == ++initialized; }},
	});
	hw::reset();
	IGROS_CHECK(1 == igros::drivers::init(DRIVERS));
	IGROS_CHECK(3 == initialized);
	IGROS_CHECK("Driver first:\tOK\nDriver broken:\tERROR\nDriver last:\tOK\n" == hw::console);

	return igros::test::result();

}

