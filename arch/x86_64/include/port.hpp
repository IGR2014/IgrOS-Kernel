#ifndef PORT_H
#define PORT_H


#include <types.hpp>


namespace arch {

	// Read byte from port
	extern "C" t_u8		portRByte(t_u16);
	// Read word from port
	extern "C" t_u16	portRWord(t_u16);
	// Read long from port
	extern "C" t_u32	portRLong(t_u16);

	// Write byte to port
	extern "C" void		portWByte(t_u16, t_u8);
	// Write word to port
	extern "C" void		portWWord(t_u16, t_u16);
	// Write long to port
	extern "C" void		portWLong(t_u16, t_u32);

}	// namespace arch

#endif	// PORT_H

