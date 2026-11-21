////////////////////////////////////////////////////////////////
//
//	Drivers list initialization tests
//
//	File:	test_driver.cpp
//	Date:	03 Nov 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


// C++
#include <array>
#include <cstddef>
#include <span>
#include <string>
// IgrOS-Kernel drivers
#include <drivers/driver.hpp>
// Tests
#include <tests/hw.hpp>


namespace hw = igros::test::hw;


using context_t = igros::drivers::driverContext_t;

namespace {

	// Device class: lifecycle and writes only
	class counter_t final {
	public:
		auto init() noexcept -> bool {
			mReady = true;
			return true;
		}
		void deinit() noexcept {
			mReady = false;
		}
		auto write(const std::span<const std::byte> data) noexcept -> igros::igros_usize_t {
			mWritten += data.size();
			return mReady ? data.size() : 0;
		}
		[[nodiscard]]
		auto written() const noexcept -> igros::igros_usize_t {
			return mWritten;
		}

	private:
		bool		     mReady {false};
		igros::igros_usize_t mWritten {0};
	};

} // namespace

// Initialized drivers count
static auto initialized {0};

// Instance context seen by every operation
static auto instance {42};
[[nodiscard]]
static auto isInstance(const context_t context) noexcept -> bool {
	return &instance == context;
}


auto main() -> int {

	// Every driver initialized in order, failures counted and reported
	constexpr auto DRIVERS = std::to_array<igros::drivers::driver_t>({
		{.name = "first", .init = [](context_t) noexcept { return 1 == ++initialized; }},
		{.name = "broken", .init = [](context_t) noexcept { ++initialized; return false; }},
		{.name = "last", .init = [](context_t) noexcept { return 3 == ++initialized; }},
	});
	hw::reset();
	IGROS_CHECK(1 == igros::drivers::init(DRIVERS));
	IGROS_CHECK(3 == initialized);
	IGROS_CHECK("Driver first:\tOK\nDriver broken:\tERROR\nDriver last:\tOK\n" == hw::console);

	// Ready callback gets only drivers that are set up
	initialized = 0;
	auto ready {std::string {}};
	IGROS_CHECK(1 == igros::drivers::init(DRIVERS, [&ready](const igros::drivers::driver_t &driver) noexcept { ready += driver.name; }));
	IGROS_CHECK("firstlast" == ready);

	// Driver without init has nothing to set up
	hw::reset();
	IGROS_CHECK(0 == igros::drivers::init(std::to_array<igros::drivers::driver_t>({{.name = "passive"}})));
	IGROS_CHECK("Driver passive:\tOK\n" == hw::console);

	// Deinit in reverse order, drivers without deinit skipped
	static auto    released {std::string {}};
	constexpr auto RELEASED = std::to_array<igros::drivers::driver_t>({
		{.name = "a", .deinit = [](context_t) noexcept { released += 'a'; }},
		{.name = "none"},
		{.name = "b", .deinit = [](context_t) noexcept { released += 'b'; }},
	});
	igros::drivers::deinit(RELEASED);
	IGROS_CHECK("ba" == released);

	// Operations go to driver with instance context, unsupported ones do nothing
	enum class REQUEST : igros::igros_dword_t { PING = 7 };
	static auto    received {std::string {}};
	constexpr auto DEVICE = igros::drivers::driver_t {
		.name	 = "device",
		.context = &instance,
		.init	 = [](const context_t context) noexcept { return isInstance(context); },
		.deinit	 = [](const context_t context) noexcept { released = isInstance(context) ? "device" : "other"; },
		.read	 = [](const context_t context, const std::span<std::byte> buffer) noexcept -> igros::igros_usize_t {
			buffer[0] = std::byte {'r'};
			return isInstance(context) ? 1 : 0;
		},
		.write = [](const context_t context, const std::span<const std::byte> data) noexcept -> igros::igros_usize_t {
			for (const auto symbol : data) {
				received += static_cast<char>(symbol);
			}
			return isInstance(context) ? data.size() : 0;
		},
		.ioctl = [](const context_t context, const igros::igros_dword_t request, const igros::igros_pointer_t arg) noexcept -> bool {
			return isInstance(context) && (7 == request) && (nullptr != arg);
		},
	};
	IGROS_CHECK(igros::drivers::init(DEVICE));
	auto buffer {std::array<std::byte, 2> {}};
	IGROS_CHECK(1 == igros::drivers::read(DEVICE, buffer)); // flawfinder: ignore (device read, bounded by span size)
	IGROS_CHECK(std::byte {'r'} == buffer[0]);
	IGROS_CHECK(5 == igros::drivers::write(DEVICE, "hello"));
	IGROS_CHECK("hello" == received);
	auto arg {0};
	IGROS_CHECK(igros::drivers::ioctl(DEVICE, REQUEST::PING, &arg));
	IGROS_CHECK(!igros::drivers::ioctl(DEVICE, REQUEST::PING));
	igros::drivers::deinit(DEVICE);
	IGROS_CHECK("device" == released);
	constexpr auto NONE {igros::drivers::driver_t {.name = "none"}};
	IGROS_CHECK(igros::drivers::init(NONE));
	igros::drivers::deinit(NONE);
	IGROS_CHECK(0 == igros::drivers::read(NONE, buffer)); // flawfinder: ignore (device read, bounded by span size)
	IGROS_CHECK(0 == igros::drivers::write(NONE, "hello"));
	IGROS_CHECK(!igros::drivers::ioctl(NONE, REQUEST::PING, &arg));

	// Driver made from device class: methods it has, others missing
	static auto    counter {counter_t {}};
	constexpr auto COUNTER {igros::drivers::make("counter", counter)};
	IGROS_CHECK((&counter == COUNTER.context) && (std::string {"counter"} == COUNTER.name));
	IGROS_CHECK((nullptr == COUNTER.read) && (nullptr == COUNTER.ioctl));
	IGROS_CHECK(0 == igros::drivers::write(COUNTER, "early"));
	IGROS_CHECK(igros::drivers::init(COUNTER));
	IGROS_CHECK(3 == igros::drivers::write(COUNTER, "abc"));
	IGROS_CHECK(8 == counter.written());
	igros::drivers::deinit(COUNTER);
	IGROS_CHECK(0 == igros::drivers::write(COUNTER, "late"));
	IGROS_CHECK(!igros::drivers::ioctl(COUNTER, REQUEST::PING, &arg));

	return igros::test::result();
}

