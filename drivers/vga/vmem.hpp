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
#include <cstddef>
#include <span>
#include <string_view>
// IgrOS-Kernel arch
#include <arch/types.hpp>
// IgrOS-Kernel drivers
#include <drivers/driver.hpp>


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


	/// @brief VGA console driver
	namespace vga {

		/// @brief VGA console colors (SET_COLOR argument)
		struct colors_t {
			vmemColor background; ///< Background color
			vmemColor foreground; ///< Text color
		};

		/// @brief VGA console control requests
		enum class IOCTL : igros_dword_t {
			CLEAR	    = 0x01_u32, ///< Clear console (no arg)
			SET_COLOR   = 0x02_u32, ///< Set text colors (arg: colors_t*)
			SET_CURSOR  = 0x03_u32, ///< Set cursor position (arg: vmemCursor*)
			GET_CURSOR  = 0x04_u32, ///< Get cursor position (arg: vmemCursor*)
			SHOW_CURSOR = 0x05_u32, ///< Enable cursor (no arg)
			HIDE_CURSOR = 0x06_u32, ///< Disable cursor (no arg)
		};

		/// @brief VGA text mode console
		class console_t final {

		public:

			/// @brief Init console (cleared, cursor hidden at origin)
			/// @return true if console is set up
			[[nodiscard]]
			auto init() noexcept -> bool;
			/// @brief Write text bytes (control characters move cursor)
			/// @param data Text bytes
			/// @return Number of written bytes
			[[maybe_unused]]
			auto write(const std::span<const std::byte> data) noexcept -> igros_usize_t;
			/// @brief Console control
			/// @param request Request (IOCTL)
			/// @param arg Request argument
			/// @return true if request succeeded
			[[nodiscard]]
			auto ioctl(const igros_dword_t request, const igros_pointer_t arg) noexcept -> bool;

			/// @brief Write symbol
			/// @param symbol Character (control characters move cursor)
			void write(const char symbol) noexcept;
			/// @brief Write string
			/// @param message Null-terminated string
			void write(const char* const message) noexcept;
			/// @brief Write fixed-width string
			/// @param message Text
			void write(const std::string_view message) noexcept;
			/// @brief Clear console with background color
			void clear() noexcept;

			/// @brief Set text colors
			/// @param background Background color
			/// @param foreground Text color
			void setColor(const vmemColor background, const vmemColor foreground) noexcept;

			/// @brief Set cursor position
			/// @param x Column
			/// @param y Row
			void cursorSet(const igros_byte_t x, const igros_byte_t y) noexcept;
			/// @brief Set cursor position
			/// @param cursor Position
			void cursorSet(const vmemCursor cursor) noexcept;
			/// @brief Get cursor position (from CRT controller)
			/// @return Cursor position
			[[nodiscard]]
			auto cursorGet() const noexcept -> vmemCursor;
			/// @brief Disable cursor
			void cursorDisable() noexcept;
			/// @brief Enable cursor
			void cursorEnable() noexcept;

		private:

			/// @brief Blank symbol (space with current background)
			/// @return Space in current colors
			[[nodiscard]]
			auto blank() const noexcept -> vmemSymbol;

			igros_byte_t mColor {static_cast<igros_byte_t>(vmemColor::Green)}; ///< Current colors (background << 4 | foreground)
			vmemCursor   mCursor {};					   ///< Current cursor position
		};

		/// @brief Kernel console
		inline constinit console_t console {};

		/// @brief Kernel console driver description
		inline constexpr drivers::driver_t DRIVER {drivers::make("VGA", console)};

	} // namespace vga


	/// @brief Write symbol to kernel console
	/// @param symbol Character (control characters move cursor)
	void vmemWrite(const char symbol) noexcept;
	/// @brief Write string to kernel console
	/// @param message Null-terminated string
	void vmemWrite(const char* const message) noexcept;
	/// @brief Write fixed-width string to kernel console
	/// @param message Text
	void vmemWrite(const std::string_view message) noexcept;


} // namespace igros::arch

