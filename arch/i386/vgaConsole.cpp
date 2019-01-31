////////////////////////////////////////////////////////////////
//
//	VGA console low-level operations
//
//	File:	vgaConsole.cpp
//	Date:	23 Jun. 2018
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
//	All rights reserved.
//
//


#include <include/vgaConsole.hpp>
#include <include/port.hpp>
#include <include/memset.hpp>


// Arch-dependent code zone
namespace arch {


	// Set cursor position
	void vgaConsoleCursorSet(const byte_t &x, const byte_t &y) {

		// Calculate VGA console offset
		word_t position = (y * VIDEO_MEM_WIDTH) + x;

		// Send to controller low byte of offset
		outPort16(VGA_CURSOR_CONTROL, ((position & 0x00FF) << 8) | 0x0F);
		// Send to controller high byte of offset
		outPort16(VGA_CURSOR_CONTROL, (position & 0xFF00) | 0x0E);

		// Save cursor data
		cursorPos.x	= x;
		cursorPos.y	= y;

	}

	// Disable VGA console cursor
	void vgaConsoleCursorDisable() {

		// Send control word to disable cursor
		outPort16(VGA_CURSOR_CONTROL, 0x200A);
	
	}

	// Set VGA console color
	void vgaConsoleSetColor(const byte_t &background, const byte_t &foreground) {

		// Background is first 4 bits and foreground is next 4
		vgaConsoleBkgColor = (background << 4) | foreground;

	}

	// Write symbol to VGA console
	void vgaConsoleWrite(const sbyte_t &symbol) {

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

			// Calculate offset in VGA console
			word_t pos = cursorPos.y * VIDEO_MEM_WIDTH + cursorPos.x;

			// Write symbol to VGA console
			vgaConsoleBase[pos].symbol	= symbol;
			vgaConsoleBase[pos].color	= vgaConsoleBkgColor;

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
			for (word_t i = VIDEO_MEM_WIDTH; i < VIDEO_MEM_SIZE; ++i) {

				vgaConsoleBase[i - VIDEO_MEM_WIDTH] = vgaConsoleBase[i];

			}

			// Calculate offset in VGA console
			word_t pos = cursorPos.y * VIDEO_MEM_WIDTH + cursorPos.x;
			// Clear bootom line
			kmemset16(&vgaConsoleBase[pos], VIDEO_MEM_WIDTH, (' ' | (vgaConsoleBkgColor << 8)));

		}

		// Set new cursor position
		vgaConsoleCursorSet(cursorPos.x, cursorPos.y);

	}

	// Write string to VGA console
	void vgaConsoleWrite(const sbyte_t* message) {

		// Cast const pointer to pointer
		sbyte_t* data = const_cast<sbyte_t*>(&message[0]);

		// Loop through message while \0 not found
		while (*data != '\0') {

			// Write symbols one by one
			vgaConsoleWrite(*data++);

		}

	}

	// Write fixed-width string to VGA console
	void vgaConsoleWrite(const sbyte_t* message, const dword_t &size) {

		// Loop through message
		for (dword_t i = 0; i < size; ++i) {

			// Write symbols one by one
			vgaConsoleWrite(message[i]);

		}

	}

	// Write string to VGA console with \r \n
	void vgaConsoleWriteLine(const sbyte_t* message) {

		// Write message
		vgaConsoleWrite(message);
		// Add \r and \n to it
		vgaConsoleWrite("\r\n");

	}

	// Write fixed-width string to VGA console with \r \n
	void vgaConsoleWriteLine(const sbyte_t* message, const dword_t &size) {

		// Write fixed-size message
		vgaConsoleWrite(message, size);
		// Add \r and \n to it
		vgaConsoleWrite("\r\n");

	}

	// Clear VGA console
	void vgaConsoleClear() {

		// Set whole screen with whitespace with default background
		kmemset16(vgaConsoleBase, VIDEO_MEM_SIZE, (' ' | (vgaConsoleBkgColor << 8)));

	}

	// Init VGA console
	void vgaConsoleInit() {

		// Clear screen
		vgaConsoleClear();
		// Place cursor at (0, 0)
		vgaConsoleCursorSet(0, 0);
		// Disable cursor
		vgaConsoleCursorDisable();

	}

	// Write decimal value to VGA console
	void vgaConsoleWriteDec(sdword_t number) {

		// Temporary buffer for number
		// Note that number should be 32-bit
		sbyte_t temp[11] = {0};

		// Check sign
		if (number < 0) {

			// Write
			vgaConsoleWrite('-');
			// Convert number to positive
			number = -number;

		}

		// Setup counter to 10-th position in temp
		sdword_t i = 9;

		// Loop through all digits while number is greater than 10
		// Digits are stored backwards from the end of the temp
		do {

			// Calculate reminder
			sdword_t reminder	= number % 10;
			// Devide value by base to remove current digit
			number			/= 10;
			// Save current digit in the temp
			temp[i--]		= '0' + reminder;

		} while (number != 0);

		// Output digit in temp from i + 1 position to the end
		vgaConsoleWrite(&temp[++i]);

	}

	// Write hexidemical value to VGA console
	void vgaConsoleWriteHex(dword_t number) {

		// Temporary buffer for number
		// Note that number should be 32-bit
		sbyte_t temp[9] = {0};

		// Write hex appendix
		vgaConsoleWrite("0x");

		// Setup counter to 10-th position in temp
		sdword_t i = 7;

		// Loop through all digits while number is greater than 10
		// Digits are stored backwards from the end of the temp
		do {

			// Calculate reminder
			sdword_t reminder	= number % 16;
			// Devide value by base to remove current digit
			number			/= 16;
			// Save current digit in the temp
			temp[i--]		= (((reminder < 10) ? '0' : ('A' - 10)) + reminder);

		} while (number != 0);

		while (i >= 0) {

			temp[i--]	= '0';

		};

		// Output digit in temp from i + 1 position to the end
		vgaConsoleWrite(temp);

	}


}	// namespace arch

