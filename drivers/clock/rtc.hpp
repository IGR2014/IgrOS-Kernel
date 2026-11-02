////////////////////////////////////////////////////////////////
///
///	@file
///	@brief		RTC clock driver
///	@date		16 Dec 2022
///
///	@copyright	Copyright (c) 2017 - 2022, Igor Baklykov
///			All rights reserved.
///	@author		Igor Baklykov
///


#pragma once


// IgrOS-Kernel arch
#include <arch/types.hpp>


// Arch-dependent code zone
namespace igros::arch {


	/// @brief Date/time structure
	struct clockDateTime_t {
		igros_dword_t		year;	///< Year (full)
		igros_dword_t		month;	///< Month (1..12)
		igros_dword_t		day;	///< Day of month (1..31)
		igros_dword_t		weekday;	///< Day of week (1..7, Sunday is 1)
		igros_dword_t		hour;	///< Hour (0..23)
		igros_dword_t		minute;	///< Minute (0..59)
		igros_dword_t		second;	///< Second (0..59)
	};


	/// @brief RTC date
	struct rtcDate_t {
		igros_byte_t		year;	///< Year of century
		igros_byte_t		month;	///< Month
		igros_byte_t		day;	///< Day of month
		/// @brief Field-wise comparison
		/// @return true if all fields are equal
		constexpr auto operator==(const rtcDate_t&) const noexcept -> bool = default;
	};

	/// @brief RTC time
	struct rtcTime_t {
		igros_byte_t		hour;	///< Hour (bit 7 - PM in 12-hour format)
		igros_byte_t		minute;	///< Minute
		igros_byte_t		second;	///< Second
		/// @brief Field-wise comparison
		/// @return true if all fields are equal
		constexpr auto operator==(const rtcTime_t&) const noexcept -> bool = default;
	};

	/// @brief RTC date/time
	struct rtcDateTime_t {
		rtcDate_t		date;	///< Date
		igros_byte_t		weekday;	///< Day of week
		rtcTime_t		time;	///< Time
		/// @brief Field-wise comparison
		/// @return true if all fields are equal
		constexpr auto operator==(const rtcDateTime_t&) const noexcept -> bool = default;
	};


	/// @brief Clock from RTC conversion
	/// @param rtcDateTime RTC date/time (binary)
	/// @param century Century years (2000)
	/// @return Date/time with full year
	[[nodiscard]]
	auto clockFromRTC(const rtcDateTime_t &rtcDateTime, const igros_dword_t century) noexcept -> clockDateTime_t;

	/// @brief Get current date/time
	/// @return Current date/time
	[[nodiscard]]
	auto clockGetCurrentDateTime() noexcept -> clockDateTime_t;


	/// @brief Read CMOS register
	/// @param cmd CMOS register
	/// @return Register value
	[[nodiscard]]
	auto rtcRead(const igros_byte_t cmd) noexcept -> igros_byte_t;

	/// @brief Read CMOS date
	/// @return Date as stored by RTC
	[[nodiscard]]
	auto rtcReadDate() noexcept -> rtcDate_t;
	/// @brief Read CMOS time
	/// @return Time as stored by RTC
	[[nodiscard]]
	auto rtcReadTime() noexcept -> rtcTime_t;
	/// @brief Read CMOS date/time
	/// @return Date/time as stored by RTC
	[[nodiscard]]
	auto rtcReadDateTime() noexcept -> rtcDateTime_t;

	/// @brief Convert BCD to binary
	/// @param bcd BCD value
	/// @return Binary value
	[[nodiscard]]
	auto rtcFromBCD(const igros_byte_t bcd) noexcept -> igros_byte_t;

	/// @brief Convert BCD date to binary
	/// @param date Date (converted in place)
	void rtcDateFromBCD(rtcDate_t &date) noexcept;
	/// @brief Convert BCD time to binary
	/// @param time Time (converted in place)
	void rtcTimeFromBCD(rtcTime_t &time) noexcept;
	/// @brief Convert BCD date/time to binary
	/// @param dateTime Date/time (converted in place)
	void rtcDateTimeFromBCD(rtcDateTime_t &dateTime) noexcept;

	/// @brief Setup RTC function
	/// @return true if RTC is set up
	[[nodiscard]]
	auto rtcSetup() noexcept -> bool;


}	// namespace igros::arch

