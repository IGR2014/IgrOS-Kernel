////////////////////////////////////////////////////////////////
//
//	VGA memory low-level operations
//
//	File:	vmem.hpp
//	Date:	01 Oct 2019
//
//	Copyright (c) 2017 - 2019, Igor Baklykov
//	All rights reserved.
//
//


#pragma once
#ifndef IGROS_ARCH_VIDEO_MEMORY_HPP
#define IGROS_ARCH_VIDEO_MEMORY_HPP


#include <arch/types.hpp>


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

	// VGA cursor start register
	static const byte_t	VIDEO_MEM_CURSOR_START	= 0x0A;


	// VGA memory colors enumeration
	enum vmemColor : byte_t {

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

	// VGA memory symbol representation
	struct vmemSymbol {

		sbyte_t	symbol;		// Symbol
		byte_t	color;		// Color of symbol and background

	};

	// VGA memory base address
	static vmemSymbol* const	vmemBase	= reinterpret_cast<vmemSymbol*>(0x000B8000);

	// VGA memory background symbol
	static byte_t			vmemBkgColor	= vmemColor::Green;

	// VGA memory cursor struct
	struct vmemCursor {

		byte_t	x;		// Cursor X coordinate
		byte_t	y;		// Cursor Y coordinate

	};

	// Current cursor coordinates
	static vmemCursor cursorPos;


	// Set VGA memory cursor position
	void		vmemCursorSet(const byte_t&, const byte_t&);
	// Set VGA memory cursor position
	void		vmemCursorSet(const vmemCursor&);

	// Get VGA memory cursor position
	vmemCursor	vmemCursorGet();

	// Disable VGA memory cursor
	void		vmemCursorDisable();
	// Enable VGA memory cursor
	void		vmemCursorEnable();

	// Set VGA memory color
	void		vmemSetColor(const byte_t&, const byte_t&);

	// Write symbol to VGA memory
	void		vmemWrite(const sbyte_t&);
	// Write string to VGA memory
	void		vmemWrite(const sbyte_t*);
	// Write fixed-width string to VGA memory
	void		vmemWrite(const sbyte_t*, const dword_t&);

	// Clear VGA console
	void		vmemClear();

	// Init VGA console
	void		vmemInit();


}	// namespace arch


#endif	// IGROS_ARCH_VIDEO_MEMORY_HPP

