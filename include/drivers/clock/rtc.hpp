////////////////////////////////////////////////////////////////
//
//	RTC clock driver
//
//	File:	rtc.hpp
//	Date:	08 Dec 2022
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <arch/types.hpp>


// Arch-dependent code zone
namespace igros::arch {


	// Date/time structure
	struct clockDateTime_t {
		igros_dword_t		year;
		igros_dword_t		month;
		igros_dword_t		day;
		igros_dword_t		weekday;
		igros_dword_t		hour;
		igros_dword_t		minute;
		igros_dword_t		second;
	};


	// RTC date
	struct rtcDate_t {
		igros_byte_t		year;
		igros_byte_t		month;
		igros_byte_t		day;
	};

	// RTC time
	struct rtcTime_t {
		igros_byte_t		hour;
		igros_byte_t		minute;
		igros_byte_t		second;
	};

	// RTC date/time
	struct rtcDateTime_t {
		rtcDate_t		date;
		igros_byte_t		weekday;
		rtcTime_t		time;
	};


	// Clock from RTC conversion
	[[nodiscard]]
	auto clockFromRTC(const rtcDateTime_t &rtcDateTime, const igros_dword_t century) noexcept -> clockDateTime_t;

	// Get current date/time
	[[nodiscard]]
	auto clockGetCurrentDateTime() noexcept -> clockDateTime_t;


	// Read CMOS register
	[[nodiscard]]
	auto rtcRead(const igros_byte_t cmd) noexcept -> igros_byte_t;

	// Read CMOS date
	[[nodiscard]]
	auto rtcReadDate() noexcept -> rtcDate_t;
	// Read CMOS time
	[[nodiscard]]
	auto rtcReadTime() noexcept -> rtcTime_t;
	// Read CMOS date/time
	[[nodiscard]]
	auto rtcReadDateTime() noexcept -> rtcDateTime_t;

	// Convert BCD to binary
	[[nodiscard]]
	auto rtcFromBCD(const igros_byte_t bcd) noexcept -> igros_byte_t;

	// Convert BCD date to binary
	void rtcDateFromBCD(rtcDate_t &date) noexcept;
	// Convert BCD time to binary
	void rtcTimeFromBCD(rtcTime_t &time) noexcept;
	// Convert BCD date/time to binary
	void rtcDateTimeFromBCD(rtcDateTime_t &dateTime) noexcept;

	// Setup RTC function
	void rtcSetup() noexcept;


}	// namespace igros::arch

