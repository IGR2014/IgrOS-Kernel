////////////////////////////////////////////////////////////////
///
///	@file
///	@brief		RTC clock driver
///	@date		02 Nov 2026
///
///	@copyright	Copyright (c) 2017 - 2026, Igor Baklykov
///			All rights reserved.
///	@author		Igor Baklykov
///


// IgrOS-Kernel arch
#include <arch/io.hpp>
// IgrOS-Kernel drivers
#include <drivers/clock/rtc.hpp>
// IgrOS-Kernel library
#include <klib/kFlags.hpp>
#include <klib/kPrint.hpp>


// Arch-dependent code zone
namespace igros::arch {


	// CMOS ports
	constexpr auto CMOS_COMMAND	{static_cast<port_t>(0x0070_u16)};	///< Register select port
	constexpr auto CMOS_DATA	{static_cast<port_t>(CMOS_COMMAND + 1_u16)};	///< Data port


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
			.second		= rtcDateTime.time.second,
		};
	}


	// RTC date/time commands
	constexpr auto RTC_YEAR		{0x09_u8};	///< Year register
	constexpr auto RTC_MONTH	{0x08_u8};	///< Month register
	constexpr auto RTC_DAY		{0x07_u8};	///< Day of month register
	constexpr auto RTC_WEEKDAY	{0x06_u8};	///< Day of week register
	constexpr auto RTC_HOUR		{0x04_u8};	///< Hour register
	constexpr auto RTC_MINUTE	{0x02_u8};	///< Minute register
	constexpr auto RTC_SECOND	{0x00_u8};	///< Second register

	constexpr auto RTC_CENTURY	{0x32_u8};	///< Century register (ACPI FADT default)

	constexpr auto RTC_REGISTER_A	{0x0A_u8};	///< Status register A
	constexpr auto RTC_REGISTER_B	{0x0B_u8};	///< Status register B

	/// @brief Anonymous namespace
	namespace {

		/// @brief Register A bits
		enum class RTC_A : igros_byte_t {
			UPDATING	= 0x80_u8,	///< Date/time update in progress (values unstable)
		};

		/// @brief Register B bits
		enum class RTC_B : igros_byte_t {
			TIME_24		= 0x02_u8,	///< 24-hour format (12-hour otherwise, bit 7 of hour - PM)
			BINARY		= 0x04_u8,	///< Binary values (BCD otherwise)
		};

	}	// namespace


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
			.day	= rtcRead(RTC_DAY),
		};
	}

	// Read CMOS time
	[[nodiscard]]
	auto rtcReadTime() noexcept -> rtcTime_t {
		return rtcTime_t {
			.hour	= rtcRead(RTC_HOUR),
			.minute	= rtcRead(RTC_MINUTE),
			.second	= rtcRead(RTC_SECOND),
		};
	}

	// Read CMOS date/time
	[[nodiscard]]
	auto rtcReadDateTime() noexcept -> rtcDateTime_t {
		return rtcDateTime_t {
			.date		= rtcReadDate(),
			.weekday	= rtcRead(RTC_WEEKDAY),
			.time		= rtcReadTime(),
		};
	}


	// Convert BCD to binary
	[[nodiscard]]
	auto rtcFromBCD(const igros_byte_t bcd) noexcept -> igros_byte_t {
		return static_cast<igros_byte_t>((bcd & 0x0F_u8) + (((bcd >> 4) & 0x0F_u8) * 10_u8));
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
	auto clockGetCurrentDateTime() noexcept -> clockDateTime_t {
		// Read CMOS date/time (once no update is in progress)
		const auto readStable = []() noexcept {
			while (klib::kFlags<RTC_A>::from(rtcRead(RTC_REGISTER_A)).isSet(RTC_A::UPDATING)) {
				// Wait for update end
			}
			return rtcReadDateTime();
		};
		auto rtcDateTime	{readStable()};
		auto lastDateTime	{readStable()};
		// Re-read until two reads match (update could happen between registers reads)
		while (lastDateTime != rtcDateTime) {
			lastDateTime	= rtcDateTime;
			rtcDateTime	= readStable();
		}
		// Get RTC flags
		const auto flags	{klib::kFlags<RTC_B>::from(rtcRead(RTC_REGISTER_B))};
		// Get RTC century
		auto century		{rtcRead(RTC_CENTURY)};
		// Check data format
		if (!flags.isSet(RTC_B::BINARY)) {
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
		if (!flags.isSet(RTC_B::TIME_24)) {
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
		const auto dateTime {clockGetCurrentDateTime()};
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

