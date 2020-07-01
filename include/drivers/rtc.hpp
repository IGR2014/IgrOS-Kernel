////////////////////////////////////////////////////////////////
//
//	RTC clock driver
//
//	File:	rtc.hpp
//	Date:	30 Jun 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <types.hpp>


// Arch-dependent code zone
namespace igros::arch {


	// Date/time structure
	struct clockDateTime_t {
		dword_t		year;
		dword_t		month;
		dword_t		day;
		dword_t		weekday;
		dword_t		hour;
		dword_t		minute;
		dword_t		second;
	};


	// RTC date
	struct rtcDate_t {
		byte_t		year;
		byte_t		month;
		byte_t		day;
	};

	// RTC time
	struct rtcTime_t {
		byte_t		hour;
		byte_t		minute;
		byte_t		second;
	};

	// RTC date/time
	struct rtcDateTime_t {
		rtcDate_t	date;
		byte_t		weekday;
		rtcTime_t	time;
	};


	// Clock from RTC conversion
	void clockFromRTC(const rtcDateTime_t &rtcDateTime, const dword_t century, clockDateTime_t &dateTime) noexcept;

	// Get current date/time
	clockDateTime_t clockGetCurrentDateTime() noexcept;


	// Read CMOS register
	byte_t rtcRead(const byte_t cmd) noexcept;


	// Read CMOS date
	void rtcReadDate(rtcDate_t &date) noexcept;

	// Read CMOS time
	void rtcReadTime(rtcTime_t &time) noexcept;

	// Read CMOS date/time
	void rtcReadDateTime(rtcDateTime_t &dateTime) noexcept;


	// Convert BCD to binary
	byte_t rtcFromBCD(const byte_t bcd) noexcept;


	// Convert BCD date to binary
	void rtcDateFromBCD(rtcDate_t &date) noexcept;

	// Convert BCD time to binary
	void rtcTimeFromBCD(rtcTime_t &time) noexcept;

	// Convert BCD date/time to binary
	void rtcDateTimeFromBCD(rtcDateTime_t &dateTime) noexcept;


	// Setup RTC function
	void rtcSetup() noexcept;


}	// namespace igros::arch

