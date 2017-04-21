#include "videoMem.hpp"

extern "C" void kernelFunc(void) {

	const char* message = "Hello world, Kernel! :)";

	// Clear screen
	videoMemClear();

	// Print message on screen
	videoMemWriteLine(message);

}
