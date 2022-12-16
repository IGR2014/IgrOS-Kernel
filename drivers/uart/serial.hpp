////////////////////////////////////////////////////////////////
//
//	UART driver
//
//	File:	serial.hpp
//	Date:	16 Dec 2022
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// IgrOS-Kernel arch
#include <arch/types.hpp>


// Arch-dependent code zone
namespace igros::arch {


	// Baud rates
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
		BAUD_115200	= 115200_u32
	};

	// Data size
	enum class DATA_SIZE : igros_byte_t {
		CHAR_5		= 0x00,
		CHAR_6		= 0x01,
		CHAR_7		= 0x02,
		CHAR_8		= 0x03
	};

	// Stop bits
	enum class STOP_BITS : igros_byte_t {
		STOP_1		= 0x00_u8,
		STOP_1_5	= 0x01_u8,
		STOP_2		= 0x03_u8
	};

	// Parity
	enum class PARITY : igros_byte_t {
		NONE		= 0x00_u8,
		ODD		= 0x01_u8,
		EVEN		= 0x02_u8,
		MARK		= 0x05_u8,
		SPACE		= 0x07_u8
	};


	// Initialize serial port
	[[nodiscard]]
	auto	serialInit(const BAUD_RATE baudRate, const DATA_SIZE dataSize, const STOP_BITS stopBits, const PARITY parity) noexcept -> bool;

	// Is write ready?
	[[nodiscard]]
	auto	serialReadyWrite() noexcept -> bool;
	// Is read ready?
	[[nodiscard]]
	auto	serialReadyRead() noexcept -> bool;

	// Serial write
	[[maybe_unused]]
	auto	serialWrite(const char* const src, const igros_usize_t size) noexcept -> igros_usize_t;
	// Serial write
	[[maybe_unused]]
	auto	serialWrite(const char* const src) noexcept -> igros_usize_t;
	// Serial read
	[[maybe_unused]]
	auto	serialRead(char* const src, const igros_usize_t size) noexcept -> igros_usize_t;

	// Setup serial port
	void	serialSetup(const BAUD_RATE baudRate = BAUD_RATE::BAUD_115200, const DATA_SIZE dataSize = DATA_SIZE::CHAR_8, const STOP_BITS stopBits = STOP_BITS::STOP_1, const PARITY parity = PARITY::NONE) noexcept;


}	// namespace igros::arch

