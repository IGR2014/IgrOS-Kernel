////////////////////////////////////////////////////////////////
//
//	IO low-level port operations
//
//	File:	port.hpp
//	Date:	20 Nov. 2017
//
//	Copyright (c) 2017, Igor Baklykov
//	All rights reserved.
//


#ifndef IGROS_ARCH_PORT_HPP
#define IGROS_ARCH_PORT_HPP


#include <include/types.hpp>


namespace arch {


	// Read byte from port
	extern "C" t_u8		inPortB(t_u16);
	// Read word from port
	extern "C" t_u16	inPortW(t_u16);
	// Read long from port
	extern "C" t_u32	inPortL(t_u16);

	// Write byte to port
	extern "C" void		outPortB(t_u16, t_u8);
	// Write word to port
	extern "C" void		outPortW(t_u16, t_u16);
	// Write long to port
	extern "C" void		outPortL(t_u16, t_u32);


}	// namespace arch


#endif	// IGROS_ARCH_PORT_HPP
