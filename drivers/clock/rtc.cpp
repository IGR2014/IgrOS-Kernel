////////////////////////////////////////////////////////////////
//
//	RTC clock driver
//
//	File:	rtc.cpp
//	Date:	16 Dec 2022
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


// IgrOS-Kernel arch
#include <arch/io.hpp>
// IgrOS-Kernel drivers
#include <drivers/clock/rtc.hpp>
// IgrOS-Kernel library
#include <klib/kprint.hpp>


// Arch-dependent code zone
namespace igros::arch {


	// CMOS ports
	constexpr auto CMOS_COMMAND	{static_cast<port_t>(0x0070_u16)};
	constexpr auto CMOS_DATA	{static_cast<port_t>(CMOS_COMMAND + 1_u16)};


	// Clock from RTC conversion
	[[nodiscard]]
	auto clockFromRTC(const rtcDateTime_t &rtcDateTime, const igros_dword_t century) noexcept -> clockDateTime_t {
		return clockDateTime_t {
			.year		= rtcDateTime.date.year + century,
			.month		= rtcDateTime.date.month,
			.day		= rtcDateTime.date.day,
			.weekday	= rtcDateTime.weekday,
			.hour		= rtcDateTime.time.hour,
			.minute		= rtcDateTime.time.minute,
			.second		= rtcDateTime.time.second
		};
	}


	// RTC date/time commands
	constexpr auto RTC_YEAR		{0x09_u8};
	constexpr auto RTC_MONTH	{0x08_u8};
	constexpr auto RTC_DAY		{0x07_u8};
	constexpr auto RTC_WEEKDAY	{0x06_u8};
	constexpr auto RTC_HOUR		{0x04_u8};
	constexpr auto RTC_MINUTE	{0x02_u8};
	constexpr auto RTC_SECOND	{0x00_u8};

	constexpr auto RTC_CENTURY	{0x32_u8};

	constexpr auto RTC_REGISTER_A	{0x0A_u8};
	constexpr auto RTC_REGISTER_B	{0x0B_u8};

	constexpr auto RTC_IS_TIME_24	{0x02_u8};
	constexpr auto RTC_IS_BINARY	{0x04_u8};


	// Read CMOS register
	[[nodiscard]]
	auto rtcRead(const igros_byte_t cmd) noexcept -> igros_byte_t {
		// Write command
		io::writePort8(CMOS_COMMAND, cmd);
		// Read data
		return io::readPort8(CMOS_DATA);
	}

	// Read CMOS date
	[[nodiscard]]
	auto rtcReadDate() noexcept -> rtcDate_t {
		return rtcDate_t {
			.year	= rtcRead(RTC_YEAR),
			.month	= rtcRead(RTC_MONTH),
			.day	= rtcRead(RTC_DAY)
		};
	}

	// Read CMOS time
	[[nodiscard]]
	auto rtcReadTime() noexcept -> rtcTime_t {
		return rtcTime_t {
			.hour	= rtcRead(RTC_HOUR),
			.minute	= rtcRead(RTC_MINUTE),
			.second	= rtcRead(RTC_SECOND)
		};
	}

	// Read CMOS date/time
	[[nodiscard]]
	auto rtcReadDateTime() noexcept -> rtcDateTime_t {
		return rtcDateTime_t {
			.date		= rtcReadDate(),
			.weekday	= rtcRead(RTC_WEEKDAY),
			.time		= rtcReadTime()
		};
	}


	// Convert BCD to binary
	[[nodiscard]]
	auto rtcFromBCD(const igros_byte_t bcd) noexcept -> igros_byte_t {
		return (bcd & 0x0F_u8) + ((bcd >> 4) & 0x0F_u8) * 10_u8;
	}

	// Convert BCD date to binary
	void rtcDateFromBCD(rtcDate_t &date) noexcept {
		date.year	= rtcFromBCD(date.year);
		date.month	= rtcFromBCD(date.month);
		date.day	= rtcFromBCD(date.day);
	}

	// Convert BCD time to binary
	void rtcTimeFromBCD(rtcTime_t &time) noexcept {
		time.hour	= rtcFromBCD(time.hour);
		time.minute	= rtcFromBCD(time.minute);
		time.second	= rtcFromBCD(time.second);
	}

	// Convert BCD date/time to binary
	void rtcDateTimeFromBCD(rtcDateTime_t &dateTime) noexcept {
		rtcDateFromBCD(dateTime.date);
		rtcTimeFromBCD(dateTime.time);
	}

	// Get current date/time
	clockDateTime_t clockGetCurrentDateTime() noexcept {
		// Read CMOS date/time (once no update is in progress)
		const auto readStable = []() noexcept {
			while (0x00_u8 != (0x80_u8 & rtcRead(RTC_REGISTER_A)));
			return rtcReadDateTime();
		};
		auto rtcDateTime	{readStable()};
		// Re-read until two reads match (update could happen between registers reads)
		for (auto last {rtcDateTime}; last != (rtcDateTime = readStable()); last = rtcDateTime);
		// Get RTC flags
		const auto flags	{rtcRead(RTC_REGISTER_B)};
		// Get RTC century
		auto century		{rtcRead(RTC_CENTURY)};
		// Check data format
		if (0_u8 == (flags & RTC_IS_BINARY)) {
			// Chech hours format
			const auto hourModeBit {static_cast<igros_byte_t>(rtcDateTime.time.hour & 0x80_u8)};
			// Drop highest bit from hours
			rtcDateTime.time.hour &= 0x7F_u8;
			// Convert from BCD
			rtcDateTimeFromBCD(rtcDateTime);
			// Return highest bit to hours
			rtcDateTime.time.hour |= hourModeBit;
			// Convert century to BCD
			century = rtcFromBCD(century);
		}
		// Check 12-hours format (1..12, bit 7 - PM)
		if (0x00_u8 == (flags & RTC_IS_TIME_24)) {
			const auto isPM {0x00_u8 != (0x80_u8 & rtcDateTime.time.hour)};
			// 12 AM -> 0, 12 PM -> 12
			rtcDateTime.time.hour = static_cast<igros_byte_t>(((rtcDateTime.time.hour & 0x7F_u8) % 12_u8) + (isPM ? 12_u8 : 0_u8));
		}
		// Convert RTC to clock
		return clockFromRTC(rtcDateTime, ((0x00_u8 != century) ? (century * 100_u32) : 2000_u32));
	}


	// Setup RTC function
	[[nodiscard]]
	auto rtcSetup() noexcept -> bool {
		// Get current date/time
		auto dateTime {clockGetCurrentDateTime()};
		// Print result
		klib::kprintf(
			"RTC date/time:\t%02d.%02d.%04d %02d:%02d:%02d",
			dateTime.day,
			dateTime.month,
			dateTime.year,
			dateTime.hour,
			dateTime.minute,
			dateTime.second
		);
		// Success
		return true;
	}


}	// namespace igros::arch

