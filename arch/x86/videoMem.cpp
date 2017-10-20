////////////////////////////////////////////////////////////////
//
//	Video memory low-level operations
//
//	File:	videoMem.cpp
//	Date:	20 Nov. 2017
//
//	Copyright (c) 2017, Igor Baklykov
//	All rights reserved.
//


#include <include/videoMem.hpp>
#include <include/port.hpp>


// Arch-dependent code zone
namespace arch {


	// Set cursor position
	void videoMemSetCursor(const t_u8 &x, const t_u8 &y) {

		// Calculate video memory offset
		t_u16 position = (y * VIDEO_MEM_WIDTH) + x;

		// Send to controller low byte of offset
		outPortB(VIDEO_MEM_CRTC_INDEX, VIDEO_MEM_CRTC_CURSOR_LOW);
		outPortB(VIDEO_MEM_CRTC_DATA, (t_u8)position & 0xFF);

		// Send to controller high byte of offset
		outPortB(VIDEO_MEM_CRTC_INDEX, VIDEO_MEM_CRTC_CURSOR_HIGH);
		outPortB(VIDEO_MEM_CRTC_DATA, (t_u8)(position >> 8) & 0xFF);

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

		// Backspace symbol
		if (symbol == '\b') {

			// If we are not at start
			if (cursorPos.x != 0) {

				// Move 1 symbol backward
				--cursorPos.x;

			} else {

				cursorPos.x = VIDEO_MEM_WIDTH - 1;
				// Move 1 line up
				--cursorPos.y;

			}

		// Tabulation symbol
		} else if (symbol == '\t') {

			// calculate new tab offset
			cursorPos.x = (cursorPos.x + VIDEO_MEM_TAB_SIZE) & ~7;

		// Carret return
		} else if (symbol == '\r') {

			// Move to start of the row
			cursorPos.x = 0;

		// Carret new line
		} else if (symbol == '\n') {

			// Move to next row
			++cursorPos.y;

		// If non-control (printable) character
		} else if (symbol >= ' ') {

			// Calculate offset in video memory
			t_u16 pos = cursorPos.y * VIDEO_MEM_WIDTH + cursorPos.x;

			// Write symbol to video memory
			videoMemBase[pos].symbol	= symbol;
			videoMemBase[pos].color		= videoMemBkgColor;

			// Move cursor 1 symbol right
			++cursorPos.x;

		}

		// Check if we are not out of columns
		if (cursorPos.x >= VIDEO_MEM_WIDTH) {

			// Move to next line
			cursorPos.x = 0;
			++cursorPos.y;

		}

		// Chech if we are not out of rows
		if (cursorPos.y >= VIDEO_MEM_HEIGHT) {

			// Move cursor to the last line
			cursorPos.y = VIDEO_MEM_HEIGHT - 1;

			// Move screen 1 line up
			for (t_u16 i = VIDEO_MEM_WIDTH; i < VIDEO_MEM_SIZE; ++i) {

				videoMemBase[i - VIDEO_MEM_WIDTH] = videoMemBase[i];

			}

		}

		// Set new cursor position
		videoMemSetCursor(cursorPos.x, cursorPos.y);

	}

	// Write string to video memory
	void videoMemWriteMessage(const t_i8p message) {

		// Cast const pointer to pointer
		t_i8* data = const_cast<t_i8*>(message);

		// Loop through message while \0 not found
		while (*data != '\0') {

			// Write symbols one by one
			videoMemWriteSymbol(*data++);

		}

	}

	// Write fixed-width string to video memory
	void videoMemWriteMessage(const t_i8p message, const t_u32 &size) {

		// Loop through message
		for (t_u32 i = 0; i < size; ++i) {

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
	void videoMemWriteLine(const t_i8p message, const t_u32 &size) {

		// Write fixed-size message
		videoMemWriteMessage(message, size);
		// Add \r and \n to it
		videoMemWriteMessage("\r\n");

	}

	// Clear video memory
	void videoMemClear() {

		// Loop through all video memory
		for (t_u16 i = 0; i < VIDEO_MEM_SIZE; ++i) {

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
