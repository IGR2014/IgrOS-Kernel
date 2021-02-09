////////////////////////////////////////////////////////////////
//
//	UART driver
//
//	File:	serial.hpp
//	Date:	02 Feb 2021
//
//	Copyright (c) 2017 - 2021, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


#include <arch/types.hpp>


// Arch-dependent code zone
namespace igros::arch {


	// Baud rates
	enum class BAUD_RATE : dword_t {
		BAUD_110	= 110,
		BAUD_300	= 300,
		BAUD_600	= 600,
		BAUD_1200	= 1200,
		BAUD_2400	= 2400,
		BAUD_4800	= 4800,
		BAUD_9600	= 9600,
		BAUD_14400	= 14400,
		BAUD_19200	= 19200,
		BAUD_38400	= 38400,
		BAUD_57600	= 57600,
		BAUD_115200	= 115200
	};

	// Data size
	enum class DATA_SIZE : byte_t {
		CHAR_5		= 0x00,
		CHAR_6		= 0x01,
		CHAR_7		= 0x02,
		CHAR_8		= 0x03
	};

	// Stop bits
	enum class STOP_BITS : byte_t {
		STOP_1		= 0x00,
		STOP_1_5	= 0x01,
		STOP_2		= 0x03
	};

	// Parity
	enum class PARITY : byte_t {
		NONE		= 0x00,
		ODD		= 0x01,
		EVEN		= 0x02,
		MARK		= 0x05,
		SPACE		= 0x07
	};


	// Initialize serial port
	[[nodiscard]]
	bool		serialInit(const BAUD_RATE baudRate, const DATA_SIZE dataSize, const STOP_BITS stopBits, const PARITY parity) noexcept;

	// Is write ready?
	[[nodiscard]]
	bool		serialReadyWrite() noexcept;
	// Is read ready?
	[[nodiscard]]
	bool		serialReadyRead() noexcept;

	// Serial write
	[[nodiscard]]
	std::size_t	serialWrite(const byte_t* const src, const std::size_t size) noexcept;
	// Serial write
	[[nodiscard]]
	std::size_t	serialWrite(const sbyte_t* const src, const std::size_t size) noexcept;
	// Serial write
	[[nodiscard]]
	std::size_t	serialWrite(const sbyte_t* const src) noexcept;
	// Serial read
	[[nodiscard]]
	std::size_t	serialRead(byte_t* const src, const std::size_t size) noexcept;

	// Setup serial port
	void		serialSetup(const BAUD_RATE baudRate = BAUD_RATE::BAUD_115200, const DATA_SIZE dataSize = DATA_SIZE::CHAR_8, const STOP_BITS stopBits = STOP_BITS::STOP_1, const PARITY parity = PARITY::NONE) noexcept;


}	// namespace igros::arch

