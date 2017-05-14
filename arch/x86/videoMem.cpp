#include <include/videoMem.hpp>
#include <include/port.hpp>


// Arch-dependent code zone
namespace arch {

	// Set cursor position
	void videoMemSetCursor(const t_u8 &x, const t_u8 &y) {

		// Calculate video memory offset
		t_u16 position = (y * videoMemWidth) + x;

		// Send to controller low byte of offset
		outPortB(0x3D4, 0x0F);
		outPortB(0x3D5, (t_u8)position & 0xFF);

		// Send to controller high byte of offset
		outPortB(0x3D4, 0x0E);
		outPortB(0x3D5, (t_u8)(position >> 8) & 0xFF);

		// Save cursor data
		cursorPos.x	= x;
		cursorPos.y	= y;

	}

	// Set video memory color
	void videoMemSetColor(const t_u8 &background, const t_u8 &foreground) {

		// Background is first 4 bits and foreground is next 4
		videoMemBkgColor = (background << 4) | foreground;

	}

	// Write symbol to video memory
	void videoMemWriteSymbol(const t_i8 &symbol) {

		if (symbol == '\b') {

			// If we are not at start
			if (cursorPos.x != 0) {

				// Move 1 symbol backward
				--cursorPos.x;

			} else {

				cursorPos.x = videoMemWidth - 1;
				// Move 1 line up
				--cursorPos.y;

			}

		} else if (symbol == '\t') {

			// calculate new tab offset
			cursorPos.x = (cursorPos.x + 8) & ~7;

		} else if (symbol == '\r') {

			// Move to start of the row
			cursorPos.x = 0;

		} else if (symbol == '\n') {

			// Move to next row
			++cursorPos.y;

		} else if (symbol >= ' ') {

			// Calculate offset in video memory
			t_u16 pos = cursorPos.y * videoMemWidth + cursorPos.x;

			// Write symbol to video memory
			videoMemBase[pos].symbol	= symbol;
			videoMemBase[pos].color		= videoMemBkgColor;

			// Move cursor 1 symbol right
			++cursorPos.x;

		}

		// Check if we are not out of columns
		if (cursorPos.x >= videoMemWidth) {

			// Move to next line
			cursorPos.x = 0;
			++cursorPos.y;

		}

		// Chech if we are not out of rows
		if (cursorPos.y >= videoMemHeight) {

			// Move cursor to the last line
			cursorPos.y = videoMemHeight - 1;

			// Move screen 1 line up
			for (t_u16 i = videoMemWidth; i < videoMemSize; ++i) {

				videoMemBase[i - videoMemWidth] = videoMemBase[i];

			}

		}

		// Set new cursor position
		videoMemSetCursor(cursorPos.x, cursorPos.y);

	}

	// Write string to video memory
	void videoMemWriteMessage(const t_i8p message) {

		t_i8* data = const_cast<t_i8*>(message);

		// Loop through message while \0 not found
		while (*data != '\0') {

			// Write symbols one by one
			videoMemWriteSymbol(*data++);

		}

	}

	// Write fixed-width string to video memory
	void videoMemWriteMessage(const t_i8p message, const t_u64 &size) {

		// Loop through message
		for (int i = 0; i < size; ++i) {

			// Write symbols one by one
			videoMemWriteSymbol(message[i]);

		}

	}

	// Write string to video memory with \r \n
	void videoMemWriteLine(const t_i8p message) {

		// Write message
		videoMemWriteMessage(message);
		// Add \r and \n to it
		videoMemWriteMessage("\r\n");

	}

	// Write fixed-width string to video memory with \r \n
	void videoMemWriteLine(const t_i8p message, const t_u64 &size) {

		// Write fixed-size message
		videoMemWriteMessage(message, size);
		// Add \r and \n to it
		videoMemWriteMessage("\r\n");

	}

	// Clear video memory
	void videoMemClear() {

		// Loop through all video memory
		for (t_u16 i = 0; i < videoMemSize; ++i) {

			// Fill it with blank space
			videoMemBase[i].symbol	= ' ';
			videoMemBase[i].color	= videoMemBkgColor;

		}

	}

	// Init video memory
	void videoMemInit() {

		// Clear screen
		videoMemClear();
		// Place cursor at (0, 0)
		videoMemSetCursor(0, 0);

	}

}	// arch
