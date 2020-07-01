////////////////////////////////////////////////////////////////
//
//	IO low-level port operations
//
//	File:	port.hpp
//	Date:	30 Jun 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#pragma	once


#include <types.hpp>


// Arch-dependent code zone
namespace igros::arch {


	// Machine ports enumeration
	enum portAddress_t : word_t {

		PIC_MASTER_CONTROL	= 0x0020,
		PIC_MASTER_DATA		= 0x0021,
		PIT_CHANNEL_0		= 0x0040,
		PIT_CHANNEL_1		= 0x0041,
		PIT_CHANNEL_2		= 0x0042,
		PIT_CONTROL		= 0x0043,
		KEYBOARD_CONTROL	= 0x0064,
		KEYBOARD_DATA		= 0x0060,
		CMOS_COMMAND		= 0x0070,
		CMOS_DATA		= 0x0071,
		PIC_SLAVE_CONTROL	= 0x00A0,
		PIC_SLAVE_DATA		= 0x00A1,
		VGA_CURSOR_CONTROL	= 0x03D4,
		VGA_CURSOR_DATA		= 0x03D5

	};


#ifdef	__cplusplus

	extern "C" {

#endif	// __cplusplus


		// Read byte from port
		[[nodiscard]] inline byte_t volatile outPort8(const portAddress_t addr) noexcept;
		// Read word from port
		[[nodiscard]] inline word_t volatile outPort16(const portAddress_t addr) noexcept;
		// Read long from port
		[[nodiscard]] inline dword_t volatile outPort32(const portAddress_t addr) noexcept;

		// Write byte to port
		inline void volatile inPort8(const portAddress_t addr, const byte_t value) noexcept;
		// Write word to port
		inline void volatile inPort16(const portAddress_t addr, const word_t value) noexcept;
		// Write long to port
		inline void volatile inPort32(const portAddress_t addr, const dword_t value) noexcept;


#ifdef	__cplusplus

	}	// extern "C"

#endif	// __cplusplus


}	// namespace igros::arch

