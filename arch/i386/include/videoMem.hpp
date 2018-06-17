////////////////////////////////////////////////////////////////
//
//	Video memory low-level operations
//
//	File:	videoMem.hpp
//	Date:	18 Jun. 2018
//
//	Copyright (c) 2018, Igor Baklykov
//	All rights reserved.
//


#ifndef IGROS_ARCH_VIDEO_MEMORY_HPP
#define IGROS_ARCH_VIDEO_MEMORY_HPP


// Video memory max width
#define	VIDEO_MEM_WIDTH			80
// Video memory max height
#define	VIDEO_MEM_HEIGHT		25
// Video memory size
#define	VIDEO_MEM_SIZE			(VIDEO_MEM_WIDTH * VIDEO_MEM_HEIGHT)

// TAB size
#define	VIDEO_MEM_TAB_SIZE		8

// VGA CRTC index register
#define VIDEO_MEM_CRTC_INDEX		0x3D4
// VGA CRTC data register
#define	VIDEO_MEM_CRTC_DATA		0x3D5
// VGA CRTC cursor high byte command
#define	VIDEO_MEM_CRTC_CURSOR_HIGH	0x0E
// VGA CRTC cursor low byte command
#define	VIDEO_MEM_CRTC_CURSOR_LOW	0x0F

#include <include/types.hpp>


// Arch-dependent code zone
namespace arch {


	// Video memory colors enumeration
	enum videoMemColor: unsigned char {

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

	// Video memory symbol representation
	struct videoMemSymbol {

		t_i8	symbol;		// Symbol
		t_u8	color;		// Color of symbol and background

	};

	// Video memory base address
	static videoMemSymbol* const	videoMemBase		= reinterpret_cast<videoMemSymbol*>(0x000B8000);

	// Video memory background symbol
	static t_u8			videoMemBkgColor	= VGAGreen;

	// Video memory cursor struct
	struct videoMemCursor {

		t_u8	x;		// Cursor X coordinate
		t_u8	y;		// Cursor Y coordinate

	};

	// Current cursor coordinates
	static videoMemCursor		cursorPos;


	// Set video memory cursor position
	void videoMemSetCursor(const t_u8&, const t_u8&);

	// Set video memory color
	void videoMemSetColor(const t_u8&, const t_u8&);

	// Write symbol to video memory
	void videoMemWrite(const t_i8&);

	// Write string to video memory
	void videoMemWrite(const t_i8p);

	// Write fixed-width string to video memory
	void videoMemWrite(const t_i8p, const t_u32&);

	// Write string to video memory with \r \n
	void videoMemWriteLine(const t_i8p);

	// Write fixed-width string to video memory with \r \n
	void videoMemWriteLine(const t_i8p, const t_u32&);

	// Clear video memory
	void videoMemClear();

	// Init video memory
	void videoMemInit();

	// Write decimal value to video memory
	void videoMemWriteDec(t_i32);


}	// namespace arch


#endif	// IGROS_ARCH_VIDEO_MEMORY_HPP

