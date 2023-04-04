////////////////////////////////////////////////////////////////
//
//	VGA memory low-level operations
//
//	File:	vmem.cpp
//	Date:	16 Dec 2022
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


// IgrOS-Kernel arch
#include <arch/io.hpp>
// IgrOS-Kernel drivers
#include <drivers/vga/vmem.hpp>
// IgrOS-Kernel library
#include <klib/kmemory.hpp>
#include <klib/kstring.hpp>


// Arch-dependent code zone
namespace igros::arch {


	// VGA cursor control ports
	constexpr auto VGA_CURSOR_CONTROL	{static_cast<io::port_t>(0x03D4_u16)};
	constexpr auto VGA_CURSOR_DATA		{static_cast<io::port_t>(VGA_CURSOR_CONTROL + 1_u16)};


	// Set cursor position
	void vmemCursorSet(const igros_byte_t x, const igros_byte_t y) noexcept {
		// Calculate VGA console offset
		const auto position {(y * VIDEO_MEM_WIDTH) + x};
		// Choose cursor location high register
		io::get().writePort8(VGA_CURSOR_CONTROL,	0x0E_u8);
		// Write cursor position high byte
		io::get().writePort8(VGA_CURSOR_DATA,		((position & 0xFF00_u16) >> 8));
		// Choose cursor location low register
		io::get().writePort8(VGA_CURSOR_CONTROL,	0x0F_u8);
		// Write cursor position low byte
		io::get().writePort8(VGA_CURSOR_DATA,		(position & 0x00FF_u16));
		// Save cursor data
		cursorPos.x = x;
		cursorPos.y = y;
	}

	// Set VGA memory cursor position
	void vmemCursorSet(const vmemCursor cursor) noexcept {
		vmemCursorSet(cursor.x, cursor.y);
	}

	// Get VGA memory cursor position
	[[nodiscard]]
	auto vmemCursorGet() noexcept -> vmemCursor {
		// Choose cursor location high register
		io::get().writePort8(VGA_CURSOR_CONTROL, 0x0E_u8);
		// Write cursor position high byte
		auto position {static_cast<igros_word_t>(io::get().readPort8(VGA_CURSOR_DATA))};
		// Choose cursor location low register
		io::get().writePort8(VGA_CURSOR_CONTROL, 0x0F_u8);
		// Write cursor position low byte
		(position <<= 8) |= io::get().readPort8(VGA_CURSOR_DATA);
		// Return cursor data
		return vmemCursor {
			.x = static_cast<igros_byte_t>(position % VIDEO_MEM_WIDTH),
			.y = static_cast<igros_byte_t>(position / VIDEO_MEM_WIDTH)
		};
	}


	// Disable VGA memory cursor
	void vmemCursorDisable() noexcept {
		// Choose cursor start register
		io::get().writePort8(VGA_CURSOR_CONTROL,	0x0A_u8);
		// Send control word to disable cursor
		io::get().writePort8(VGA_CURSOR_DATA,		0x20_u8);
	}

	// Enable VGA memory cursor
	void vmemCursorEnable() noexcept {
		// Choose cursor start register
		io::get().writePort8(VGA_CURSOR_CONTROL, 0x0A_u8);
		// Get current register value
		const auto cursorStartReg {io::get().readPort8(VGA_CURSOR_DATA)};
		// Send control word to disable cursor
		io::get().writePort8(VGA_CURSOR_DATA, cursorStartReg & ~0x20_u8);
	}

	// Set VGA memory color
	void vmemSetColor(const igros_byte_t background, const igros_byte_t foreground) noexcept {
		// Background is first 4 bits and foreground is next 4
		vmemBkgColor = static_cast<vmemColor>((background << 4) | foreground);
	}

	// Write symbol to VGA memory
	void vmemWrite(const char symbol) noexcept {
		// Backspace symbol
		if (symbol == '\b') {
			// If we are not at start
			if (0_u8 != cursorPos.x) {
				// Move 1 symbol backward
				--cursorPos.x;
			} else {
				cursorPos.x = VIDEO_MEM_WIDTH - 1_u16;
				// Move 1 line up
				--cursorPos.y;
			}
		// Tabulation symbol
		} else if (symbol == '\t') {
			// calculate new tab offset
			cursorPos.x = (cursorPos.x + VIDEO_MEM_TAB_SIZE) & ~0x07_u8;
		// Carret return
		} else if (symbol == '\r') {
			// Move to start of the row
			cursorPos.x = 0_u8;
		// Carret new line
		} else if (symbol == '\n') {
			// Move to start of the row
			cursorPos.x = 0_u8;
			// Move to next row
			++cursorPos.y;
		// If non-control (printable) character
		} else if (symbol >= ' ') {
			// Calculate offset in VGA console
			const auto pos {cursorPos.y * VIDEO_MEM_WIDTH + cursorPos.x};
			// Write symbol to VGA console
			vmemBase[pos].symbol	= symbol;
			vmemBase[pos].color	= static_cast<igros_byte_t>(vmemBkgColor);
			// Move cursor 1 symbol right
			++cursorPos.x;
		}
		// Check if we are not out of columns
		if (cursorPos.x >= VIDEO_MEM_WIDTH) {
			// Move to start of the row
			cursorPos.x = 0_u8;
			// Move to next row
			++cursorPos.y;
		}
		// Chech if we are not out of rows
		if (cursorPos.y >= VIDEO_MEM_HEIGHT) {
			// Move cursor to the last line
			cursorPos.y = VIDEO_MEM_HEIGHT - 1_u16;
			// Move screen 1 line up
			for (auto i = VIDEO_MEM_WIDTH; i < VIDEO_MEM_SIZE; ++i) {
				vmemBase[i - VIDEO_MEM_WIDTH] = vmemBase[i];
			}
			// Calculate offset in VGA console
			const auto pos {cursorPos.y * VIDEO_MEM_WIDTH + cursorPos.x};
			// Clear bottom line
			klib::kmemset(&vmemBase[pos], VIDEO_MEM_WIDTH, (' ' | (static_cast<igros_word_t>(vmemBkgColor) << 8)));
		}
		// Set new cursor position
		vmemCursorSet(cursorPos.x, cursorPos.y);
	}

	// Write string to VGA memory
	void vmemWrite(const char* const message) noexcept {
		// Write symbols one by one
		vmemWrite(message, klib::kstrlen(message));
	}

	// Write fixed-width string to VGA memory
	void vmemWrite(const char* const message, const igros_usize_t size) noexcept {
		// Loop through message
		for (auto i {0_usize}; i < size; ++i) {
			// Write symbols one by one
			vmemWrite(message[i]);
		}
	}


	// Clear VGA memory
	void vmemClear() noexcept {
		// Set whole screen with whitespace with default background
		klib::kmemset(vmemBase, VIDEO_MEM_SIZE, (' ' | (static_cast<igros_word_t>(vmemBkgColor) << 8)));
	}


	// Init VGA memory
	void vmemInit() noexcept {
		// Clear screen
		vmemClear();
		// Place cursor at (0, 0)
		vmemCursorSet(0_u8, 0_u8);
		// Disable cursor
		vmemCursorDisable();
	}


}	// namespace igros::arch

