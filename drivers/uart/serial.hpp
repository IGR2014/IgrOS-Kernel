////////////////////////////////////////////////////////////////
///
///	@file
///	@brief		UART driver
///	@date		16 Dec 2022
///
///	@copyright	Copyright (c) 2017 - 2022, Igor Baklykov
///			All rights reserved.
///	@author		Igor Baklykov
///


#pragma once


// IgrOS-Kernel arch
// C++
#include <span>
#include <string_view>
// IgrOS-Kernel arch
#include <arch/types.hpp>


// Arch-dependent code zone
namespace igros::arch {


	/// @brief Serial ports (standard PC I/O bases)
	enum class SERIAL_PORT : igros_word_t {
		COM1		= 0x03F8_u16,
		COM2		= 0x02F8_u16,
		COM3		= 0x03E8_u16,
		COM4		= 0x02E8_u16,
	};

	/// @brief Baud rates
	enum class BAUD_RATE : igros_dword_t {
		BAUD_110	= 110_u32,
		BAUD_300	= 300_u32,
		BAUD_600	= 600_u32,
		BAUD_1200	= 1200_u32,
		BAUD_2400	= 2400_u32,
		BAUD_4800	= 4800_u32,
		BAUD_9600	= 9600_u32,
		BAUD_14400	= 14400_u32,
		BAUD_19200	= 19200_u32,
		BAUD_38400	= 38400_u32,
		BAUD_57600	= 57600_u32,
		BAUD_115200	= 115200_u32,
	};

	/// @brief Data size
	enum class DATA_SIZE : igros_byte_t {
		CHAR_5		= 0x00,
		CHAR_6		= 0x01,
		CHAR_7		= 0x02,
		CHAR_8		= 0x03,
	};

	/// @brief Stop bits
	enum class STOP_BITS : igros_byte_t {
		STOP_1		= 0x00_u8,
		STOP_1_5	= 0x01_u8,
		STOP_2		= 0x03_u8,
	};

	/// @brief Parity
	enum class PARITY : igros_byte_t {
		NONE		= 0x00_u8,
		ODD		= 0x01_u8,
		EVEN		= 0x02_u8,
		MARK		= 0x05_u8,
		SPACE		= 0x07_u8,
	};


	/// @brief Initialize serial port
	/// @param port Serial port
	/// @param baudRate Baud rate
	/// @param dataSize Data bits
	/// @param stopBits Stop bits
	/// @param parity Parity
	/// @return true if port passed loopback test
	[[nodiscard]]
	auto	serialInit(const SERIAL_PORT port, const BAUD_RATE baudRate, const DATA_SIZE dataSize, const STOP_BITS stopBits, const PARITY parity) noexcept -> bool;

	/// @brief Is write ready?
	/// @param port Serial port
	/// @return true if transmitter holding register is empty
	[[nodiscard]]
	auto	serialReadyWrite(const SERIAL_PORT port) noexcept -> bool;
	/// @brief Is read ready?
	/// @param port Serial port
	/// @return true if received byte is available
	[[nodiscard]]
	auto	serialReadyRead(const SERIAL_PORT port) noexcept -> bool;

	/// @brief Serial write
	/// @param port Serial port
	/// @param text Text
	/// @return Number of written characters
	[[maybe_unused]]
	auto	serialWrite(const SERIAL_PORT port, const std::string_view text) noexcept -> igros_usize_t;
	/// @brief Serial read
	/// @param port Serial port
	/// @param buffer Output buffer
	/// @return Number of read characters
	[[maybe_unused]]
	auto	serialRead(const SERIAL_PORT port, const std::span<char> buffer) noexcept -> igros_usize_t;

	/// @brief Serial write to COM1 (kernel console)
	/// @param text Text
	/// @return Number of written characters
	[[maybe_unused]]
	auto	serialWrite(const std::string_view text) noexcept -> igros_usize_t;
	/// @brief Serial write to COM1 (kernel console)
	/// @param src Null-terminated string
	/// @return Number of written characters
	[[maybe_unused]]
	auto	serialWrite(const char* const src) noexcept -> igros_usize_t;

	/// @brief Setup COM1 (115200 8N1) with receive interrupt
	/// @return true if COM1 is set up
	[[nodiscard]]
	auto	serialSetup() noexcept -> bool;


}	// namespace igros::arch

