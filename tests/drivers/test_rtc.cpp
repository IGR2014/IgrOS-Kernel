////////////////////////////////////////////////////////////////
//
//	CMOS real time clock driver tests
//
//	File:	test_rtc.cpp
//	Date:	03 Nov 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


// C++
#include <algorithm>
#include <array>
#include <bit>
#include <cstddef>
#include <vector>
// IgrOS-Kernel drivers
#include <drivers/clock/rtc.hpp>
// Tests
#include <tests/hw.hpp>


using namespace igros::arch;
namespace hw = igros::test::hw;


// CMOS ports and registers
constexpr auto CMOS_COMMAND {std::uint16_t {0x70}};
constexpr auto CMOS_DATA {std::uint16_t {0x71}};
constexpr auto RTC_SECOND {0x00};
constexpr auto RTC_REGISTER_A {0x0A};
constexpr auto RTC_REGISTER_B {0x0B};
constexpr auto RTC_CENTURY {0x32};

// Register B flags
constexpr auto RTC_IS_TIME_24 {0x02u};
constexpr auto RTC_IS_BINARY {0x04u};


// CMOS model: registers selected by command port
static auto cmos {std::array<std::uint8_t, 128> {}};
// Register A reads reporting update in progress
static auto updating {0};
// Seconds register reads before seconds tick
static auto secondsTick {-1};

static auto cmosRead(const std::uint16_t port) -> std::uint32_t {
	if (CMOS_DATA != port) {
		return hw::portValue[port];
	}
	const auto index {hw::portValue[CMOS_COMMAND] & 0x7F};
	if ((RTC_REGISTER_A == index) && (updating > 0)) {
		--updating;
		return 0x80;
	}
	if ((RTC_SECOND == index) && (secondsTick >= 0)) {
		if (0 == secondsTick) {
			++cmos[RTC_SECOND];
		}
		--secondsTick;
	}
	return cmos[index];
}

// Set clock registers: year month day weekday hour minute second
static void setCMOS(const std::uint8_t flags, const std::uint8_t century, const std::array<std::uint8_t, 7> &values) { // NOLINT(bugprone-easily-swappable-parameters): test helper, arguments written in each call
	hw::reset();
	hw::portReadHook     = cmosRead;
	updating	     = 0;
	secondsTick	     = -1;
	cmos		     = {};
	cmos[0x09]	     = values[0];
	cmos[0x08]	     = values[1];
	cmos[0x07]	     = values[2];
	cmos[0x06]	     = values[3];
	cmos[0x04]	     = values[4];
	cmos[0x02]	     = values[5];
	cmos[0x00]	     = values[6];
	cmos[RTC_REGISTER_B] = flags;
	cmos[RTC_CENTURY]    = century;
}

// Compare date/time
[[nodiscard]]
static auto equals(const clockDateTime_t &value, const std::array<igros::igros_dword_t, 7> &expected) -> bool {
	return (expected[0] == value.year) && (expected[1] == value.month) && (expected[2] == value.day) &&
		(expected[3] == value.weekday) && (expected[4] == value.hour) &&
		(expected[5] == value.minute) && (expected[6] == value.second);
}


// BCD conversion
static void testBCD() {
	IGROS_CHECK(0 == rtcFromBCD(0x00));
	IGROS_CHECK(9 == rtcFromBCD(0x09));
	IGROS_CHECK(59 == rtcFromBCD(0x59));
	IGROS_CHECK(99 == rtcFromBCD(0x99));
}


// Raw register reads go through command port
static void testRead() {
	setCMOS(RTC_IS_TIME_24, 0x20, {0x26, 0x10, 0x19, 0x02, 0x23, 0x45, 0x07});
	IGROS_CHECK(0x45 == rtcRead(0x02));
	IGROS_CHECK((std::vector<std::uint32_t> {0x02}) == hw::writes(CMOS_COMMAND));
	IGROS_CHECK((rtcDate_t {0x26, 0x10, 0x19}) == rtcReadDate());
	IGROS_CHECK((rtcTime_t {0x23, 0x45, 0x07}) == rtcReadTime());
}


// Date/time comparison: every field counts
static void testCompare() {
	constexpr auto DATE {rtcDate_t {.year = 26, .month = 10, .day = 19}};
	constexpr auto TIME {rtcTime_t {.hour = 23, .minute = 45, .second = 7}};
	IGROS_CHECK(!(DATE == rtcDate_t {.year = 25, .month = 10, .day = 19}));
	IGROS_CHECK(!(DATE == rtcDate_t {.year = 26, .month = 11, .day = 19}));
	IGROS_CHECK(!(DATE == rtcDate_t {.year = 26, .month = 10, .day = 20}));
	IGROS_CHECK(!(TIME == rtcTime_t {.hour = 22, .minute = 45, .second = 7}));
	IGROS_CHECK(!(TIME == rtcTime_t {.hour = 23, .minute = 46, .second = 7}));
	IGROS_CHECK(!(TIME == rtcTime_t {.hour = 23, .minute = 45, .second = 8}));
	const auto dateTime {rtcDateTime_t {.date = DATE, .weekday = 2, .time = TIME}};
	IGROS_CHECK(dateTime == (rtcDateTime_t {.date = DATE, .weekday = 2, .time = TIME}));
	IGROS_CHECK(!(dateTime == rtcDateTime_t {.date = {.year = 25, .month = 10, .day = 19}, .weekday = 2, .time = TIME}));
	IGROS_CHECK(!(dateTime == rtcDateTime_t {.date = DATE, .weekday = 3, .time = TIME}));
	IGROS_CHECK(!(dateTime == rtcDateTime_t {.date = DATE, .weekday = 2, .time = {.hour = 22, .minute = 45, .second = 7}}));
}


// BCD 24-hour clock with century register
static void testBCD24() {
	setCMOS(RTC_IS_TIME_24, 0x20, {0x26, 0x10, 0x19, 0x02, 0x23, 0x45, 0x07});
	IGROS_CHECK(equals(clockGetCurrentDateTime(), {2026, 10, 19, 2, 23, 45, 7}));
}


// Binary 12-hour clock (PM flag in hour bit 7), no century register
static void testBinary12() {
	setCMOS(RTC_IS_BINARY, 0, {26, 10, 19, 2, 0x80 | 11, 5, 9});
	IGROS_CHECK(equals(clockGetCurrentDateTime(), {2026, 10, 19, 2, 23, 5, 9}));
	// 12 PM is noon, 12 AM is midnight
	setCMOS(RTC_IS_BINARY, 0, {26, 10, 19, 2, 0x80 | 12, 0, 0});
	IGROS_CHECK(12 == clockGetCurrentDateTime().hour);
	setCMOS(RTC_IS_BINARY, 0, {26, 10, 19, 2, 12, 0, 0});
	IGROS_CHECK(0 == clockGetCurrentDateTime().hour);
}


// BCD 12-hour clock keeps PM flag through conversion
static void testBCD12() {
	setCMOS(0, 0x19, {0x99, 0x12, 0x31, 0x05, 0x80 | 0x11, 0x59, 0x58});
	IGROS_CHECK(equals(clockGetCurrentDateTime(), {1999, 12, 31, 5, 23, 59, 58}));
}


// Waits for update to finish, rereads until two reads match
static void testStableRead() {
	setCMOS(RTC_IS_TIME_24 | RTC_IS_BINARY, 20, {26, 10, 19, 2, 10, 0, 30});
	updating    = 3;
	secondsTick = 0;
	IGROS_CHECK(equals(clockGetCurrentDateTime(), {2026, 10, 19, 2, 10, 0, 31}));
	IGROS_CHECK(0 == updating);
	// Seconds tick between first and second read: read again until reads match
	setCMOS(RTC_IS_TIME_24 | RTC_IS_BINARY, 20, {26, 10, 19, 2, 10, 0, 30});
	secondsTick = 1;
	IGROS_CHECK(equals(clockGetCurrentDateTime(), {2026, 10, 19, 2, 10, 0, 31}));
	IGROS_CHECK(secondsTick < 0);
}


// Setup prints current date/time
static void testSetup() {
	setCMOS(RTC_IS_TIME_24, 0x20, {0x26, 0x10, 0x19, 0x02, 0x09, 0x05, 0x07});
	IGROS_CHECK(rtc::cmos.init());
	IGROS_CHECK(hw::printed("RTC date/time:\t19.10.2026 09:05:07"));
}


// Read and control return current date/time
static void testDriver() {
	setCMOS(RTC_IS_TIME_24, 0x20, {0x26, 0x10, 0x19, 0x02, 0x09, 0x05, 0x07});
	auto bytes {std::array<std::byte, sizeof(clockDateTime_t) + 1> {}};
	IGROS_CHECK(sizeof(clockDateTime_t) == igros::drivers::read(rtc::DRIVER, bytes)); // flawfinder: ignore (device read, bounded by span size)
	auto raw {std::array<std::byte, sizeof(clockDateTime_t)> {}};
	std::copy_n(bytes.begin(), raw.size(), raw.begin());
	IGROS_CHECK(equals(std::bit_cast<clockDateTime_t>(raw), {2026, 10, 19, 2, 9, 5, 7}));
	// Too small buffer: nothing read
	IGROS_CHECK(0 == igros::drivers::read(rtc::DRIVER, std::span {bytes}.first(sizeof(clockDateTime_t) - 1))); // flawfinder: ignore (device read, bounded by span size)
	auto dateTime {clockDateTime_t {}};
	IGROS_CHECK(igros::drivers::ioctl(rtc::DRIVER, rtc::IOCTL::GET_DATETIME, &dateTime));
	IGROS_CHECK(equals(dateTime, {2026, 10, 19, 2, 9, 5, 7}));
	// No argument, unknown request, no write
	IGROS_CHECK(!igros::drivers::ioctl(rtc::DRIVER, rtc::IOCTL::GET_DATETIME));
	IGROS_CHECK(!rtc::DRIVER.ioctl(rtc::DRIVER.context, 0xFFu, &dateTime));
	IGROS_CHECK(0 == igros::drivers::write(rtc::DRIVER, "time"));
}


auto main() -> int {
	testBCD();
	testRead();
	testCompare();
	testBCD24();
	testBinary12();
	testBCD12();
	testStableRead();
	testSetup();
	testDriver();
	return igros::test::result();
}

