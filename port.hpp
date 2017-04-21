#ifndef PORT_H
#define PORT_H


// Read data from port
extern "C" unsigned char portRead(unsigned short);

// Write data to port
extern "C" void portWrite(unsigned short, unsigned char);

#endif	// PORT_H
