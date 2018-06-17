////////////////////////////////////////////////////////////////
//
//	IO low-level port operations
//
//	File:	port.hpp
//	Date:	18 Jun. 2018
//
//	Copyright (c) 2018, Igor Baklykov
//	All rights reserved.
//


#ifndef IGROS_ARCH_PORT_HPP
#define IGROS_ARCH_PORT_HPP


#include <include/types.hpp>


// Arch-dependent code zone
namespace arch {


	extern "C" {


		// Read byte from port
		t_u8	inPortB(t_u16);
		// Read word from port
		t_u16	inPortW(t_u16);
		// Read long from port
		t_u32	inPortL(t_u16);

		// Write byte to port
		void	outPortB(t_u16, t_u8);
		// Write word to port
		void	outPortW(t_u16, t_u16);
		// Write long to port
		void	outPortL(t_u16, t_u32);


	}	// extern "C"


}	// namespace arch


#endif	// IGROS_ARCH_PORT_HPP

