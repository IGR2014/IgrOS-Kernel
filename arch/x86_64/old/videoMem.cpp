#include <videoMem.hpp>
#include <include/io.hpp>


// Clear wideo memory
void videoMemClear() {

	unsigned char color = videoMemColor::black << 4 | videoMemColor::lightGreen;

        for (int i = 0; i < videoMemSize; ++i) {

                videoMemPtr[i].symbol	= ' ';
		videoMemPtr[i].color	= color;

        }

};

// Set color
void videoMemSetColor(unsigned char &color) {

	videoMemSymbolColor = color;

};

// Set cursor position to x, y
void videoMemSetCursor(const unsigned int &x, const unsigned int &y) {

	videoMemX = x;
	videoMemY = y;

	unsigned char position = y * videoMemWidth + x;

	portWByte(0x03D4, 0x0F);
	portWByte(0x03D5, position & 0xFF);
	portWByte(0x03D4, 0x0E);
	portWByte(0x03D5, (position >> 8) & 0xFF);

};

// Write symbol to video memory
void videoMemWriteSymbol(const char &symbol) {

	if (symbol == '\b') {

		if (videoMemX != 0) {

			--videoMemX;

		}

	} else if (symbol == '\t') {

		videoMemX = (videoMemX + 8) & ~7;

	} else if (symbol == '\r') {

		videoMemX = 0;

	} else if (symbol == '\n') {

		++videoMemY;

	} else if (symbol >= ' ') {

		unsigned short pos = videoMemY * videoMemWidth + videoMemX;
		videoMemPtr[pos].symbol	= symbol;
		videoMemPtr[pos].color	= videoMemSymbolColor;
		++videoMemX;

	}

	if (videoMemX >= videoMemWidth) {

		videoMemX = 0;
		++videoMemY;

	}

	videoMemSetCursor(videoMemX, videoMemY);

};

// Write message to video memory
void videoMemWriteMessage(const char* message, const unsigned int &size) {

	for (int i = 0; i < size; ++i) {

		videoMemWriteSymbol(message[i]);

	}

};

// Write message to video memory (overloaded)
void videoMemWriteMessage(const char* message) {

	char* data = const_cast<char*>(message);

	while (*data != '\0') {

		videoMemWriteSymbol(*data++);

	}

};

// Write line to video memory
void videoMemWriteLine(const char* message, const unsigned int &size) {

	for (int i = 0; i < size; ++i) {

		videoMemWriteSymbol(message[i]);

	}

	videoMemWriteSymbol('\r');
	videoMemWriteSymbol('\n');

};

// Write line to video memory (overloaded)
void videoMemWriteLine(const char* message) {

	char* data = const_cast<char*>(message);

	while (*data != '\0') {

		videoMemWriteSymbol(*data++);

	}

	videoMemWriteSymbol('\r');
	videoMemWriteSymbol('\n');

};

