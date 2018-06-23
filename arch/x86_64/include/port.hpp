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


#include <types.hpp>


namespace arch {


	// Read byte from port
	extern "C" byte_t		portRByte(word_t);
	// Read word from port
	extern "C" word_t	portRWord(word_t);
	// Read long from port
	extern "C" dword_t	portRLong(word_t);

	// Write byte to port
	extern "C" void		portWByte(word_t, byte_t);
	// Write word to port
	extern "C" void		portWWord(word_t, word_t);
	// Write long to port
	extern "C" void		portWLong(word_t, dword_t);


}	// namespace arch


#endif	// IGROS_ARCH_PORT_HPP
