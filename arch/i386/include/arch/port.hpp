////////////////////////////////////////////////////////////////
//
//	IO low-level port operations
//
//	File:	port.hpp
//	Date:	06 Jun 2019
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
//	All rights reserved.
//
//


#pragma once
#ifndef IGROS_ARCH_PORT_HPP
#define IGROS_ARCH_PORT_HPP


#include <arch/types.hpp>


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


#ifdef	__cplusplus

	extern "C" {

#endif	// __cplusplus


		// Read byte from port
		static inline byte_t volatile	inPort8(const portAddress_t);
		// Read word from port
		static inline word_t volatile	inPort16(const portAddress_t);
		// Read long from port
		static inline dword_t volatile	inPort32(const portAddress_t);

		// Write byte to port
		static inline void volatile	outPort8(const portAddress_t, const byte_t);
		// Write word to port
		static inline void volatile	outPort16(const portAddress_t, const word_t);
		// Write long to port
		static inline void volatile	outPort32(const portAddress_t, const dword_t);


#ifdef	__cplusplus

	}	// extern "C"

#endif	// __cplusplus


}	// namespace arch


#endif	// IGROS_ARCH_PORT_HPP

