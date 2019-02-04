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


// VGA console max width
#define	VIDEO_MEM_WIDTH			80
// VGA console max height
#define	VIDEO_MEM_HEIGHT		25
// VGA console size
#define	VIDEO_MEM_SIZE			(VIDEO_MEM_WIDTH * VIDEO_MEM_HEIGHT)

// TAB size
#define	VIDEO_MEM_TAB_SIZE		8


#include <include/types.hpp>


// Arch-dependent code zone
namespace arch {


	// VGA console colors enumeration
	enum vgaConsoleColor_t : byte_t {

	        VGABlack   	= 0x00,			// Black VGA color
	        VGABlue		= 0x01,			// Blue VGA color
		VGAGreen	= 0x02,			// Green VGA color
		VGACyan		= 0x03,			// Cyan VGA color
		VGARed		= 0x04,			// Red VGA color
		VGAMagenta	= 0x05,			// Magenta VGA color
		VGABrown	= 0x06,			// Brown VGA color
		VGALightGray	= 0x07,			// Light gray VGA color
		VGADarkGray	= 0x08,			// Dark gray VGA color
		VGALightBlue	= 0x09,			// Light blue VGA color
		VGALightGreen	= 0x0A,			// Light green VGA color
		VGALightCyan	= 0x0B,			// Light cyan VGA color
		VGALightRed	= 0x0C,			// Light red VGA color
		VGALightMagenta	= 0x0D,			// Light magenta VGA color
		VGAYellow	= 0x0E,			// Yellow VGA color
		VGAWhite	= 0x0F			// White VGA color

	};

	// VGA console symbol representation
	struct vgaConsoleSymbol_t {

		sbyte_t	symbol;		// Symbol
		byte_t	color;		// Color of symbol and background

	};

	// VGA console base address
	static vgaConsoleSymbol_t* const	vgaConsoleBase		= reinterpret_cast<vgaConsoleSymbol_t*>(0x000B8000);

	// VGA console background symbol
	static byte_t				vgaConsoleBkgColor	= VGAGreen;

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

