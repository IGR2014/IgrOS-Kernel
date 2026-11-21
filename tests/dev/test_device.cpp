////////////////////////////////////////////////////////////////
//
//	Device registry tests
//
//	File:	test_device.cpp
//	Date:	03 Nov 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


// C++
#include <array>
#include <string>
// IgrOS-Kernel dev
#include <dev/device.hpp>
// Tests
#include <tests/test.hpp>


namespace sys = igros::sys;


// Instance context
static auto instance {1};


auto main() -> int {

	// Empty registry
	IGROS_CHECK(0 == sys::devicesCount());
	IGROS_CHECK(nullptr == sys::findDevice("COM1"));

	// Registered device is found by name, description is copied
	auto com1 {igros::drivers::driver_t {.name = "COM1", .context = &instance}};
	IGROS_CHECK(sys::registerDevice(com1));
	com1.context = nullptr;
	const auto* const found {sys::findDevice("COM1")};
	IGROS_CHECK((nullptr != found) && (&instance == found->context));
	IGROS_CHECK(nullptr == sys::findDevice("COM"));
	IGROS_CHECK(nullptr == sys::findDevice("COM12"));

	// Name taken, no name
	IGROS_CHECK(!sys::registerDevice(igros::drivers::driver_t {.name = "COM1"}));
	IGROS_CHECK(!sys::registerDevice(igros::drivers::driver_t {}));
	IGROS_CHECK(1 == sys::devicesCount());

	// Unregister frees name
	IGROS_CHECK(sys::unregisterDevice("COM1"));
	IGROS_CHECK(!sys::unregisterDevice("COM1"));
	IGROS_CHECK(nullptr == sys::findDevice("COM1"));
	IGROS_CHECK(0 == sys::devicesCount());

	// Registry is full at limit, freed slot is reused
	static auto names {std::array<std::string, sys::DEVICES_MAX + 1> {}};
	for (auto i {igros::igros_usize_t {0}}; i < names.size(); ++i) {
		names[i] = "dev" + std::to_string(i);
	}
	for (auto i {igros::igros_usize_t {0}}; i < sys::DEVICES_MAX; ++i) {
		IGROS_CHECK(sys::registerDevice(igros::drivers::driver_t {.name = names[i].c_str()}));
	}
	IGROS_CHECK(sys::DEVICES_MAX == sys::devicesCount());
	IGROS_CHECK(!sys::registerDevice(igros::drivers::driver_t {.name = names[sys::DEVICES_MAX].c_str()}));
	IGROS_CHECK(sys::unregisterDevice("dev3"));
	IGROS_CHECK(sys::registerDevice(igros::drivers::driver_t {.name = names[sys::DEVICES_MAX].c_str()}));
	IGROS_CHECK(nullptr != sys::findDevice(names[sys::DEVICES_MAX]));
	IGROS_CHECK(sys::DEVICES_MAX == sys::devicesCount());

	return igros::test::result();
}
