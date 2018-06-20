////////////////////////////////////////////////////////////////
//
//	IO low-level port operations
//
//	File:	port.hpp
//	Date:	21 Jun. 2018
//
//	Copyright (c) 2018, Igor Baklykov
//	All rights reserved.
//


#ifndef IGROS_ARCH_PORT_HPP
#define IGROS_ARCH_PORT_HPP


#include <include/types.hpp>


// Arch-dependent code zone
namespace arch {


	// Machine ports enumeration
	enum portAddress_t : t_u16 {

		PIC_MASTER_CONTROL	= 0x0020,
		PIC_MASTER_DATA		= 0x0021,
		KEYBOARD_CONTROL	= 0x0060,
		KEYBOARD_DATA		= 0x0064,
		PIC_SLAVE_CONTROL	= 0x00A0,
		PIC_SLAVE_DATA		= 0x00A1,
		VGA_CURSOR_CONTROL	= 0x03D4,
		VGA_CURSOR_DATA		= 0x03D5

	};


	extern "C" {


		// Read byte from port
		t_u8	inPort8(const portAddress_t);
		// Read word from port
		t_u16	inPort16(const portAddress_t);
		// Read long from port
		t_u32	inPort32(const portAddress_t);

		// Write byte to port
		void	outPort8(const portAddress_t, const t_u8);
		// Write word to port
		void	outPort16(const portAddress_t, const t_u16);
		// Write long to port
		void	outPort32(const portAddress_t, const t_u32);


	}	// extern "C"


}	// namespace arch


#endif	// IGROS_ARCH_PORT_HPP

