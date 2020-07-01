////////////////////////////////////////////////////////////////
//
//	RTC clock driver
//
//	File:	rtc.cpp
//	Date:	30 Jun 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#include <port.hpp>
#include <irq.hpp>

#include <drivers/vmem.hpp>
#include <drivers/rtc.hpp>

#include <klib/kprint.hpp>


// Arch-dependent code zone
namespace igros::arch {


	// Clock from RTC conversion
	void clockFromRTC(const rtcDateTime_t &rtcDateTime, const dword_t century, clockDateTime_t &dateTime) noexcept {
		dateTime.year		= rtcDateTime.date.year + century;
		dateTime.month		= rtcDateTime.date.month;
		dateTime.day		= rtcDateTime.date.day;
		dateTime.weekday	= rtcDateTime.weekday;
		dateTime.hour		= rtcDateTime.time.hour;
		dateTime.minute		= rtcDateTime.time.minute;
		dateTime.second		= rtcDateTime.time.second;
	}


	// RTC date/time commands
	constexpr auto RTC_YEAR		= 0x09;
	constexpr auto RTC_MONTH	= 0x08;
	constexpr auto RTC_DAY		= 0x07;
	constexpr auto RTC_WEEKDAY	= 0x06;
	constexpr auto RTC_HOUR		= 0x04;
	constexpr auto RTC_MINUTE	= 0x02;
	constexpr auto RTC_SECOND	= 0x00;

	constexpr auto RTC_CENTURY	= 0x32;

	constexpr auto RTC_REGISTER_A	= 0x0A;
	constexpr auto RTC_REGISTER_B	= 0x0B;

	constexpr auto RTC_IS_TIME_24	= 0x02;
	constexpr auto RTC_IS_BINARY	= 0x04;


	// Read CMOS register
	byte_t rtcRead(const byte_t cmd) noexcept {
		// Write command
		inPort8(CMOS_COMMAND, cmd);
		// Read data
		return outPort8(CMOS_DATA);
	}

	// Read CMOS date
	void rtcReadDate(rtcDate_t &date) noexcept {
		date.year	= rtcRead(RTC_YEAR);
		date.month	= rtcRead(RTC_MONTH);
		date.day	= rtcRead(RTC_DAY);
	}

	// Read CMOS time
	void rtcReadTime(rtcTime_t &time) noexcept {
		time.hour	= rtcRead(RTC_HOUR);
		time.minute	= rtcRead(RTC_MINUTE);
		time.second	= rtcRead(RTC_SECOND);
	}

	// Read CMOS date/time
	void rtcReadDateTime(rtcDateTime_t &dateTime) noexcept {
		rtcReadDate(dateTime.date);
		rtcReadTime(dateTime.time);
		dateTime.weekday = rtcRead(RTC_WEEKDAY);
	}


	// Convert BCD to binary
	byte_t rtcFromBCD(const byte_t bcd) noexcept {
		return (bcd & 0x0F) + ((bcd >> 4) & 0x0F) * 10;
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
		// Wait for update if any
		while (0x00 != (0x80 & rtcRead(RTC_REGISTER_A))) {};
		// Read CMOS date/time
		rtcDateTime_t rtcDateTime;
		rtcReadDateTime(rtcDateTime);
		// Get RTC flags
		auto flags	= rtcRead(RTC_REGISTER_B);
		// Get RTC century
		auto century	= rtcRead(RTC_CENTURY);
		// Check data format
		if (0x00 == (flags & RTC_IS_BINARY)) {
			// Chech hours format
			auto hourModeBit = static_cast<byte_t>(rtcDateTime.time.hour & 0x80);
			// Drop highest bit from hours
			rtcDateTime.time.hour &= 0x7F;
			// Convert from BCD
			rtcDateTimeFromBCD(rtcDateTime);
			// Return highest bit to hours
			rtcDateTime.time.hour |= hourModeBit;
			// Convert century to BCD
			century = rtcFromBCD(century);
		}
		// Check 24-hours format
		if (	(0x00 == (flags & RTC_IS_TIME_24))
			&& (0x00 != (0x80 & rtcDateTime.time.hour))) {
			// Adjust hours value
			rtcDateTime.time.hour = static_cast<dword_t>((rtcDateTime.time.hour & 0x7F) + 12u) % 24u;
		}
		// Convert RTC to clock
		clockDateTime_t dateTime;
		clockFromRTC(rtcDateTime, ((0x00 != century) ? (century * 100u) : 2000u), dateTime);
		// Return current
		return dateTime;
	}


	// Setup RTC function
	void rtcSetup() noexcept {
		// Get current date/time
		auto dateTime = clockGetCurrentDateTime();
		// Print result
		klib::kprintf(	u8"RTC date/time:\t%02d.%02d.%04d %02d:%02d:%02d\r\n",
				dateTime.day,
				dateTime.month,
				dateTime.year,
				dateTime.hour,
				dateTime.minute,
				dateTime.second);
	}


}	// namespace igros::arch

