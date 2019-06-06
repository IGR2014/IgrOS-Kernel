////////////////////////////////////////////////////////////////
//
//	VGA memory low-level operations
//
//	File:	vmem.cpp
//	Date:	06 Jun 2019
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
//	All rights reserved.
//
//


#include <arch/port.hpp>

#include <drivers/vmem.hpp>

#include <klib/memset.hpp>


// Arch-dependent code zone
namespace arch {


	// Set cursor position
	void vmemCursorSet(const byte_t &x, const byte_t &y) {

		// Calculate VGA console offset
		word_t position = (y * VIDEO_MEM_WIDTH) + x;

		// Choose cursor location high register
		outPort8(VGA_CURSOR_CONTROL, 0x0E);
		// Write cursor position high byte
		outPort8(VGA_CURSOR_DATA, ((position >> 8) & 0x00FF));
		// Choose cursor location low register
		outPort8(VGA_CURSOR_CONTROL, 0x0F);
		// Write cursor position low byte
		outPort8(VGA_CURSOR_DATA, (position & 0x00FF));

		// Save cursor data
		cursorPos.x	= x;
		cursorPos.y	= y;

	}

	// Set VGA memory cursor position
	void vmemCursorSet(const vmemCursor &cursor) {

		vmemCursorSet(cursor.x, cursor.y);

	}

	// Get VGA memory cursor position
	vmemCursor vmemCursorGet() {

		// Position holder
		word_t position;

		// Choose cursor location high register
		outPort8(VGA_CURSOR_CONTROL, 0x0E);
		// Write cursor position high byte
		position = inPort8(VGA_CURSOR_DATA);
		// Choose cursor location low register
		outPort8(VGA_CURSOR_CONTROL, 0x0F);
		// Write cursor position low byte
		(position <<= 8) |= inPort8(VGA_CURSOR_DATA);

		// Return cursor data
		return	vmemCursor {
				.x = position % VIDEO_MEM_WIDTH,
				.y = position / VIDEO_MEM_WIDTH
			};

	}


	// Disable VGA memory cursor
	void vmemCursorDisable() {

		// Choose cursor start register
		outPort8(VGA_CURSOR_CONTROL, 0x0A);
		// Send control word to disable cursor
		outPort8(VGA_CURSOR_DATA, 0x20);

	}

	// Enable VGA memory cursor
	void vmemCursorEnable() {

		// Choose cursor start register
		outPort8(VGA_CURSOR_CONTROL, 0x0A);
		// Get current register value
		byte_t cursorStartReg = inPort8(VGA_CURSOR_DATA);
		// Send control word to disable cursor
		outPort8(VGA_CURSOR_DATA, cursorStartReg & ~0x20);

	}

	// Set VGA memory color
	void vmemSetColor(const byte_t &background, const byte_t &foreground) {

		// Background is first 4 bits and foreground is next 4
		vmemBkgColor = (background << 4) | foreground;

	}

	// Write symbol to VGA memory
	void vmemWrite(const sbyte_t &symbol) {

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
			vmemBase[pos].symbol	= symbol;
			vmemBase[pos].color	= vmemBkgColor;

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

				vmemBase[i - VIDEO_MEM_WIDTH] = vmemBase[i];

			}

			// Calculate offset in VGA console
			word_t pos = cursorPos.y * VIDEO_MEM_WIDTH + cursorPos.x;
			// Clear bottom line
			kmemset16(&vmemBase[pos], VIDEO_MEM_WIDTH, (' ' | (vmemBkgColor << 8)));

		}

		// Set new cursor position
		vmemCursorSet(cursorPos.x, cursorPos.y);

	}

	// Write string to VGA memory
	void vmemWrite(const sbyte_t* message) {

		// Cast const pointer to pointer
		sbyte_t* data = const_cast<sbyte_t*>(&message[0]);

		// Loop through message while \0 not found
		while (*data != '\0') {

			// Write symbols one by one
			vmemWrite(*data++);

		}

	}

	// Write fixed-width string to VGA memory
	void vmemWrite(const sbyte_t* message, const dword_t &size) {

		// Loop through message
		for (dword_t i = 0; i < size; ++i) {

			// Write symbols one by one
			vmemWrite(message[i]);

		}

	}


	// Clear VGA memory
	void vmemClear() {

		// Set whole screen with whitespace with default background
		kmemset16(vmemBase, VIDEO_MEM_SIZE, (' ' | (vmemBkgColor << 8)));

	}


	// Init VGA memory
	void vmemInit() {

		// Clear screen
		vmemClear();
		// Place cursor at (0, 0)
		vmemCursorSet(0, 0);
		// Disable cursor
		vmemCursorDisable();

	}


}	// namespace arch

