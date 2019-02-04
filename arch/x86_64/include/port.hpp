////////////////////////////////////////////////////////////////
//
//	IO low-level port operations
//
//	File:	port.hpp
//	Date:	08 Aug. 2018
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
//	All rights reserved.
//
//


#ifndef IGROS_ARCH_PORT_HPP
#define IGROS_ARCH_PORT_HPP


#include <include/types.hpp>


// Arch-dependent code zone
namespace arch {


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
		PIC_SLAVE_CONTROL	= 0x00A0,
		PIC_SLAVE_DATA		= 0x00A1,
		VGA_CURSOR_CONTROL	= 0x03D4,
		VGA_CURSOR_DATA		= 0x03D5

	};


	extern "C" {


		// Read byte from port
		byte_t	inPort8(const portAddress_t);
		// Read word from port
		word_t	inPort16(const portAddress_t);
		// Read long from port
		dword_t	inPort32(const portAddress_t);

		// Write byte to port
		void	outPort8(const portAddress_t, const byte_t);
		// Write word to port
		void	outPort16(const portAddress_t, const word_t);
		// Write long to port
		void	outPort32(const portAddress_t, const dword_t);


	}	// extern "C"


}	// namespace arch


#endif	// IGROS_ARCH_PORT_HPP

