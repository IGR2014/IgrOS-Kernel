#ifndef VIDEO_MEMORY_H
#define VIDEO_MEMORY_H


#include <include/types.hpp>


// Arch-dependent code zone
namespace arch {

	// Video memory colors enumeration
	enum videoMemColor: unsigned char {

	        black   	= 0x00,
	        blue		= 0x01,
		green		= 0x02,
		cyan		= 0x03,
		red		= 0x04,
		magenta		= 0x05,
		brown		= 0x06,
		lightGray	= 0x07,
		darkGray	= 0x08,
		lightBlue	= 0x09,
		lightGreen	= 0x0A,
		lightCyan	= 0x0B,
		lightRed	= 0x0C,
		lightMagenta	= 0x0D,
		yellow		= 0x0E,
		white		= 0x0F

	};

	// Video memory symbol representation
	struct videoMemSymbol {

		t_i8	symbol;		// Symbol
		t_u8	color;		// Color of symbol and background

	};

	// Video memory base address
	static videoMemSymbol* const	videoMemBase		= (videoMemSymbol*)0xb8000;

	// Video memory max width
	static const t_u8		videoMemWidth		= 80;
	// Video memory max height
	static const t_u8		videoMemHeight		= 25;
	// Video memory size
	static const t_u16		videoMemSize		= videoMemWidth * videoMemHeight;

	// Video memory background symbol
	static t_u8			videoMemBkgColor	= 0x1F;

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
	void videoMemWriteSymbol(const t_i8&);

	// Write string to video memory
	void videoMemWriteMessage(const t_i8p);

	// Write fixed-width string to video memory
	void videoMemWriteMessage(const t_i8p, const t_u64&);

	// Write string to video memory with \r \n
	void videoMemWriteLine(const t_i8p);

	// Write fixed-width string to video memory with \r \n
	void videoMemWriteLine(const t_i8p, const t_u64&);

	// Clear video memory
	void videoMemClear();

	// Init video memory
	void videoMemInit();

}	// arch

#endif	// VIDEO_MEMORY_H
