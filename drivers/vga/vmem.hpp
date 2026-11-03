////////////////////////////////////////////////////////////////
///
///	@file
///	@brief		VGA memory low-level operations
///	@date		03 Nov 2026
///
///	@copyright	Copyright (c) 2017 - 2022, Igor Baklykov
///			All rights reserved.
///	@author		Igor Baklykov
///


#pragma once


// C++
#include <bit>
#include <string_view>
// IgrOS-Kernel arch
#include <arch/types.hpp>


// Arch-dependent code zone
namespace igros::arch {


	/// @brief VGA console max width
	constexpr static auto VIDEO_MEM_WIDTH {80_usize};
	/// @brief VGA console max height
	constexpr static auto VIDEO_MEM_HEIGHT {25_usize};
	/// @brief VGA console size
	constexpr static auto VIDEO_MEM_SIZE {VIDEO_MEM_WIDTH * VIDEO_MEM_HEIGHT};

	/// @brief TAB size
	constexpr static auto VIDEO_MEM_TAB_SIZE {8_usize};

	/// @brief VGA cursor start register
	constexpr static auto VIDEO_MEM_CURSOR_START {0x0A_u8};


	/// @brief VGA memory colors enumeration
	enum class vmemColor : igros_byte_t {
		Black	     = 0x00_u8, ///< Black VGA color
		Blue	     = 0x01_u8, ///< Blue VGA color
		Green	     = 0x02_u8, ///< Green VGA color
		Cyan	     = 0x03_u8, ///< Cyan VGA color
		Red	     = 0x04_u8, ///< Red VGA color
		Magenta	     = 0x05_u8, ///< Magenta VGA color
		Brown	     = 0x06_u8, ///< Brown VGA color
		LightGray    = 0x07_u8, ///< Light gray VGA color
		DarkGray     = 0x08_u8, ///< Dark gray VGA color
		LightBlue    = 0x09_u8, ///< Light blue VGA color
		LightGreen   = 0x0A_u8, ///< Light green VGA color
		LightCyan    = 0x0B_u8, ///< Light cyan VGA color
		LightRed     = 0x0C_u8, ///< Light red VGA color
		LightMagenta = 0x0D_u8, ///< Light magenta VGA color
		Yellow	     = 0x0E_u8, ///< Yellow VGA color
		White	     = 0x0F_u8, ///< White VGA color
	};

	/// @brief VGA memory symbol representation
	struct vmemSymbol {
		igros_sbyte_t symbol; ///< Symbol
		igros_byte_t  color;  ///< Color of symbol and background
	};

	/// @brief VGA memory cursor struct
	struct vmemCursor {
		igros_byte_t x; ///< Cursor X coordinate
		igros_byte_t y; ///< Cursor Y coordinate
	};


	/// @brief Set VGA memory cursor position
	/// @param x Column
	/// @param y Row
	void vmemCursorSet(const igros_byte_t x, const igros_byte_t y) noexcept;
	/// @brief Set VGA memory cursor position
	/// @param cursor Position
	void vmemCursorSet(const vmemCursor cursor) noexcept;

	/// @brief Get VGA memory cursor position
	/// @return Cursor position
	[[nodiscard]]
	auto vmemCursorGet() noexcept -> vmemCursor;

	/// @brief Disable VGA memory cursor
	void vmemCursorDisable() noexcept;
	/// @brief Enable VGA memory cursor
	void vmemCursorEnable() noexcept;

	/// @brief Set VGA memory color
	/// @param background Background color
	/// @param foreground Text color
	void vmemSetColor(const igros_byte_t background, const igros_byte_t foreground) noexcept;

	/// @brief Write symbol to VGA memory
	/// @param symbol Character (control characters move cursor)
	void vmemWrite(const char symbol) noexcept;
	/// @brief Write string to VGA memory
	/// @param message Null-terminated string
	void vmemWrite(const char* const message) noexcept;
	/// @brief Write fixed-width string to VGA memory
	/// @param message Text
	void vmemWrite(const std::string_view message) noexcept;

	/// @brief Clear VGA console
	void vmemClear() noexcept;

	/// @brief Init VGA console
	/// @return true if console is set up
	[[nodiscard]]
	auto vmemInit() noexcept -> bool;


} // namespace igros::arch

