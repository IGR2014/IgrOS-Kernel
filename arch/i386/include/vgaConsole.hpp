////////////////////////////////////////////////////////////////
//
//	VGA console low-level operations
//
//	File:	vgaConsole.hpp
//	Date:	23 Jun. 2018
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
//	All rights reserved.
//
//


#ifndef IGROS_ARCH_VIDEO_MEMORY_HPP
#define IGROS_ARCH_VIDEO_MEMORY_HPP


#include <include/types.hpp>


// Arch-dependent code zone
namespace arch {


	// VGA console max width
	static const word_t	VIDEO_MEM_WIDTH		= 80;
	// VGA console max height
	static const word_t	VIDEO_MEM_HEIGHT	= 25;
	// VGA console size
	static const dword_t	VIDEO_MEM_SIZE		= (VIDEO_MEM_WIDTH * VIDEO_MEM_HEIGHT);

	// TAB size
	static const word_t	VIDEO_MEM_TAB_SIZE	= 8;


	// VGA console colors enumeration
	enum vgaConsoleColor_t : byte_t {

	        Black   	= 0x00,			// Black VGA color
	        Blue		= 0x01,			// Blue VGA color
		Green		= 0x02,			// Green VGA color
		Cyan		= 0x03,			// Cyan VGA color
		Red		= 0x04,			// Red VGA color
		Magenta		= 0x05,			// Magenta VGA color
		Brown		= 0x06,			// Brown VGA color
		LightGray	= 0x07,			// Light gray VGA color
		DarkGray	= 0x08,			// Dark gray VGA color
		LightBlue	= 0x09,			// Light blue VGA color
		LightGreen	= 0x0A,			// Light green VGA color
		LightCyan	= 0x0B,			// Light cyan VGA color
		LightRed	= 0x0C,			// Light red VGA color
		LightMagenta	= 0x0D,			// Light magenta VGA color
		Yellow		= 0x0E,			// Yellow VGA color
		White		= 0x0F			// White VGA color

	};

	// VGA console symbol representation
	struct vgaConsoleSymbol_t {

		sbyte_t	symbol;		// Symbol
		byte_t	color;		// Color of symbol and background

	};

	// VGA console base address
	static vgaConsoleSymbol_t* const	vgaConsoleBase		= reinterpret_cast<vgaConsoleSymbol_t*>(0x000B8000);

	// VGA console background symbol
	static byte_t				vgaConsoleBkgColor	= vgaConsoleColor_t::Green;

	// VGA console cursor struct
	struct vgaConsoleCursor_t {

		byte_t	x;		// Cursor X coordinate
		byte_t	y;		// Cursor Y coordinate

	};

	// Current cursor coordinates
	static vgaConsoleCursor_t	cursorPos;


	// Set VGA console cursor position
	void vgaConsoleCursorSet(const byte_t&, const byte_t&);
	// Disable VGA console cursor
	void vgaConsoleCursorDisable();

	// Set VGA console color
	void vgaConsoleSetColor(const byte_t&, const byte_t&);

	// Write symbol to VGA console
	void vgaConsoleWrite(const sbyte_t&);

	// Write string to VGA console
	void vgaConsoleWrite(const sbyte_t*);

	// Write fixed-width string to VGA console
	void vgaConsoleWrite(const sbyte_t*, const dword_t&);

	// Write string to VGA console with \r \n
	void vgaConsoleWriteLine(const sbyte_t*);

	// Write fixed-width string to VGA console with \r \n
	void vgaConsoleWriteLine(const sbyte_t*, const dword_t&);

	// Clear VGA console
	void vgaConsoleClear();

	// Init VGA console
	void vgaConsoleInit();

	// Write decimal value to VGA console
	void vgaConsoleWriteDec(sdword_t);
	// Write hexidemical value to VGA console
	void vgaConsoleWriteHex(dword_t);


}	// namespace arch


#endif	// IGROS_ARCH_VIDEO_MEMORY_HPP

