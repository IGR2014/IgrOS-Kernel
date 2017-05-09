#ifndef PORT_H
#define PORT_H


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

}	// namespacearch

#endif	// PORT_H

