////////////////////////////////////////////////////////////////
//
//	VGA memory low-level operations
//
//	File:	vmem.cpp
//	Date:	24 Jan 2020
//
//	Copyright (c) 2017 - 2020, Igor Baklykov
//	All rights reserved.
//
//


#include <arch/port.hpp>

#include <drivers/vmem.hpp>

#include <klib/kmemset.hpp>


// Arch-dependent code zone
namespace arch {


	// Set cursor position
	void vmemCursorSet(const byte_t &x, const byte_t &y) noexcept {
		// Calculate VGA console offset
		auto position = (y * VIDEO_MEM_WIDTH) + x;
		// Choose cursor location high register
		inPort8(VGA_CURSOR_CONTROL, 0x0E);
		// Write cursor position high byte
		inPort8(VGA_CURSOR_DATA, ((position & 0xFF00) >> 8));
		// Choose cursor location low register
		inPort8(VGA_CURSOR_CONTROL, 0x0F);
		// Write cursor position low byte
		inPort8(VGA_CURSOR_DATA, (position & 0x00FF));
		// Save cursor data
		cursorPos.x = x;
		cursorPos.y = y;
	}

	// Set VGA memory cursor position
	void vmemCursorSet(const vmemCursor &cursor) noexcept {
		vmemCursorSet(cursor.x, cursor.y);
	}

	// Get VGA memory cursor position
	vmemCursor vmemCursorGet() noexcept {
		// Choose cursor location high register
		inPort8(VGA_CURSOR_CONTROL, 0x0E);
		// Write cursor position high byte
		auto position = static_cast<word_t>(outPort8(VGA_CURSOR_DATA));
		// Choose cursor location low register
		inPort8(VGA_CURSOR_CONTROL, 0x0F);
		// Write cursor position low byte
		(position <<= 8) |= outPort8(VGA_CURSOR_DATA);
		// Return cursor data
		return {
			.x = static_cast<byte_t>(position % VIDEO_MEM_WIDTH),
			.y = static_cast<byte_t>(position / VIDEO_MEM_WIDTH)
		};
	}


	// Disable VGA memory cursor
	void vmemCursorDisable() noexcept {
		// Choose cursor start register
		inPort8(VGA_CURSOR_CONTROL, 0x0A);
		// Send control word to disable cursor
		inPort8(VGA_CURSOR_DATA, 0x20);
	}

	// Enable VGA memory cursor
	void vmemCursorEnable() noexcept {
		// Choose cursor start register
		inPort8(VGA_CURSOR_CONTROL, 0x0A);
		// Get current register value
		auto cursorStartReg = outPort8(VGA_CURSOR_DATA);
		// Send control word to disable cursor
		inPort8(VGA_CURSOR_DATA, cursorStartReg & ~0x20);
	}

	// Set VGA memory color
	void vmemSetColor(const byte_t &background, const byte_t &foreground) noexcept {
		// Background is first 4 bits and foreground is next 4
		vmemBkgColor = (background << 4) | foreground;
	}

	// Write symbol to VGA memory
	void vmemWrite(const sbyte_t &symbol) noexcept {

		// Backspace symbol
		if (symbol == '\b') {
			// If we are not at start
			if (cursorPos.x != 0u) {
				// Move 1 symbol backward
				--cursorPos.x;
			} else {
				cursorPos.x = VIDEO_MEM_WIDTH - 1u;
				// Move 1 line up
				--cursorPos.y;
			}
		// Tabulation symbol
		} else if (symbol == '\t') {
			// calculate new tab offset
			cursorPos.x = (cursorPos.x + VIDEO_MEM_TAB_SIZE) & ~7u;
		// Carret return
		} else if (symbol == '\r') {
			// Move to start of the row
			cursorPos.x = 0u;
		// Carret new line
		} else if (symbol == '\n') {
			// Move to next row
			++cursorPos.y;
		// If non-control (printable) character
		} else if (symbol >= ' ') {
			// Calculate offset in VGA console
			auto pos = cursorPos.y * VIDEO_MEM_WIDTH + cursorPos.x;
			// Write symbol to VGA console
			vmemBase[pos].symbol	= symbol;
			vmemBase[pos].color	= vmemBkgColor;
			// Move cursor 1 symbol right
			++cursorPos.x;
		}

		// Check if we are not out of columns
		if (cursorPos.x >= VIDEO_MEM_WIDTH) {
			// Move to next line
			cursorPos.x = 0u;
			++cursorPos.y;
		}

		// Chech if we are not out of rows
		if (cursorPos.y >= VIDEO_MEM_HEIGHT) {
			// Move cursor to the last line
			cursorPos.y = VIDEO_MEM_HEIGHT - 1u;
			// Move screen 1 line up
			for (auto i = VIDEO_MEM_WIDTH; i < VIDEO_MEM_SIZE; ++i) {
				vmemBase[i - VIDEO_MEM_WIDTH] = vmemBase[i];
			}
			// Calculate offset in VGA console
			auto pos = cursorPos.y * VIDEO_MEM_WIDTH + cursorPos.x;
			// Clear bootom line
			klib::kmemset16(&vmemBase[pos], VIDEO_MEM_WIDTH, static_cast<word_t>(' ' | (vmemBkgColor << 8)));
		}

		// Set new cursor position
		vmemCursorSet(cursorPos.x, cursorPos.y);

	}

	// Write string to VGA memory
	void vmemWrite(const sbyte_t* message) noexcept {
		// Cast const pointer to pointer
		auto data = const_cast<sbyte_t*>(&message[0]);
		// Loop through message while \0 not found
		while (*data != '\0') {
			// Write symbols one by one
			vmemWrite(*data++);
		}
	}

	// Write fixed-width string to VGA memory
	void vmemWrite(const sbyte_t* message, const dword_t &size) noexcept {
		// Loop through message
		for (auto i = 0u; i < size; ++i) {
			// Write symbols one by one
			vmemWrite(message[i]);
		}
	}


	// Clear VGA memory
	void vmemClear() noexcept {
		// Set whole screen with whitespace with default background
		klib::kmemset16(vmemBase, VIDEO_MEM_SIZE, static_cast<word_t>(' ' | (vmemBkgColor << 8)));
	}


	// Init VGA memory
	void vmemInit() noexcept {
		// Clear screen
		vmemClear();
		// Place cursor at (0, 0)
		vmemCursorSet(0u, 0u);
		// Disable cursor
		vmemCursorDisable();
	}


}	// namespace arch

