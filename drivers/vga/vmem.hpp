////////////////////////////////////////////////////////////////
//
//	VGA memory low-level operations
//
//	File:	vmem.hpp
//	Date:	16 Dec 2022
//
//	Copyright (c) 2017 - 2022, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// C++
#include <bit>
// IgrOS-Kernel arch
#include <arch/types.hpp>


// Arch-dependent code zone
namespace igros::arch {


	// VGA console max width
	constexpr static auto	VIDEO_MEM_WIDTH		{80_usize};
	// VGA console max height
	constexpr static auto	VIDEO_MEM_HEIGHT	{25_usize};
	// VGA console size
	constexpr static auto	VIDEO_MEM_SIZE		{VIDEO_MEM_WIDTH * VIDEO_MEM_HEIGHT};

	// TAB size
	constexpr static auto	VIDEO_MEM_TAB_SIZE	{8_usize};

	// VGA cursor start register
	constexpr static auto	VIDEO_MEM_CURSOR_START	{0x0A_u8};


	// VGA memory colors enumeration
	enum class vmemColor : igros_byte_t {
	        Black   	= 0x00_u8,			// Black VGA color
	        Blue		= 0x01_u8,			// Blue VGA color
		Green		= 0x02_u8,			// Green VGA color
		Cyan		= 0x03_u8,			// Cyan VGA color
		Red		= 0x04_u8,			// Red VGA color
		Magenta		= 0x05_u8,			// Magenta VGA color
		Brown		= 0x06_u8,			// Brown VGA color
		LightGray	= 0x07_u8,			// Light gray VGA color
		DarkGray	= 0x08_u8,			// Dark gray VGA color
		LightBlue	= 0x09_u8,			// Light blue VGA color
		LightGreen	= 0x0A_u8,			// Light green VGA color
		LightCyan	= 0x0B_u8,			// Light cyan VGA color
		LightRed	= 0x0C_u8,			// Light red VGA color
		LightMagenta	= 0x0D_u8,			// Light magenta VGA color
		Yellow		= 0x0E_u8,			// Yellow VGA color
		White		= 0x0F_u8			// White VGA color
	};

	// VGA memory symbol representation
	struct vmemSymbol {
		igros_sbyte_t	symbol;				// Symbol
		igros_byte_t	color;				// Color of symbol and background
	};

	// VGA memory base address
	static auto* const vmemBase	{std::bit_cast<vmemSymbol* const>(0x000B8000_usize)};

	// VGA memory background symbol
	static auto vmemBkgColor	{vmemColor::Green};

	// VGA memory cursor struct
	struct vmemCursor {
		igros_byte_t	x;				// Cursor X coordinate
		igros_byte_t	y;				// Cursor Y coordinate
	};

	// Current cursor coordinates
	static vmemCursor cursorPos;


	// Set VGA memory cursor position
	void	vmemCursorSet(const igros_byte_t x, const igros_byte_t y) noexcept;
	// Set VGA memory cursor position
	void	vmemCursorSet(const vmemCursor) noexcept;

	// Get VGA memory cursor position
	[[nodiscard]]
	auto	vmemCursorGet() noexcept -> vmemCursor;

	// Disable VGA memory cursor
	void	vmemCursorDisable() noexcept;
	// Enable VGA memory cursor
	void	vmemCursorEnable() noexcept;

	// Set VGA memory color
	void	vmemSetColor(const igros_byte_t background, const igros_byte_t foreground) noexcept;

	// Write symbol to VGA memory
	void	vmemWrite(const char symbol) noexcept;
	// Write string to VGA memory
	void	vmemWrite(const char* const message) noexcept;
	// Write fixed-width string to VGA memory
	void	vmemWrite(const char* const message, const igros_usize_t size) noexcept;

	// Clear VGA console
	void	vmemClear() noexcept;

	// Init VGA console
	void	vmemInit() noexcept;


}	// namespace igros::arch

