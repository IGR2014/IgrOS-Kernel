#include "videoMem.hpp"

extern "C" void kernelFunc(void) {

	const char* message = "Hello world, Kernel! :)";

	// Clear screen
	videoMemClear();

	// Print message on screen
	videoMemWriteLine(message);

	unsigned char i = 0;
	do {

		videoMemSetColor(i);
		videoMemWriteSymbol('0');

		if ((i++ % 16) == 15) {

			videoMemWriteSymbol('\r');
			videoMemWriteSymbol('\n');

		}

	} while (i != 0);

}
