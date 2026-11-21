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


// C++
#include <algorithm>
#include <bit>
#include <span>
#include <string_view>
// IgrOS-Kernel arch
#include <arch/io.hpp>
// IgrOS-Kernel drivers
#include <drivers/vga/vmem.hpp>
// IgrOS-Kernel library
#include <klib/kString.hpp>
// IgrOS-Kernel platform
#include <platform/platform.hpp>


// Arch-dependent code zone
namespace igros::arch {


	// VGA cursor control ports
	constexpr auto VGA_CURSOR_CONTROL {static_cast<port_t>(0x03D4_u16)};		  ///< CRT controller register select port
	constexpr auto VGA_CURSOR_DATA {static_cast<port_t>(VGA_CURSOR_CONTROL + 1_u16)}; ///< CRT controller data port


	/// @brief VGA text buffer physical address
	constexpr auto VIDEO_MEM_PHYS {0x000B8000_usize};

	/// @brief VGA text buffer (low memory is mapped at kernel virtual offset)
	/// @return Text mode cells
	[[nodiscard]]
	static auto vmemScreen() noexcept -> std::span<vmemSymbol, VIDEO_MEM_SIZE> {
		return std::span<vmemSymbol, VIDEO_MEM_SIZE> {
			std::bit_cast<vmemSymbol*>(VIDEO_MEM_PHYS + platform::Platform::kernelOffset()),
			VIDEO_MEM_SIZE
		};
	}

	// Blank symbol
	[[nodiscard]]
	auto vga::console_t::blank() const noexcept -> vmemSymbol {
		return vmemSymbol {
			.symbol = ' ',
			.color	= mColor,
		};
	}


	// Set cursor position
	void vga::console_t::cursorSet(const igros_byte_t x, const igros_byte_t y) noexcept {
		// Calculate VGA console offset
		const auto position {(y * VIDEO_MEM_WIDTH) + x};
		// Choose cursor location high register
		io::writePort8(VGA_CURSOR_CONTROL, 0x0E_u8);
		// Write cursor position high byte
		io::writePort8(VGA_CURSOR_DATA, ((position & 0xFF00_u16) >> 8));
		// Choose cursor location low register
		io::writePort8(VGA_CURSOR_CONTROL, 0x0F_u8);
		// Write cursor position low byte
		io::writePort8(VGA_CURSOR_DATA, (position & 0x00FF_u16));
		// Save cursor data
		mCursor.x = x;
		mCursor.y = y;
	}

	// Set cursor position
	void vga::console_t::cursorSet(const vmemCursor cursor) noexcept {
		cursorSet(cursor.x, cursor.y);
	}

	// Get cursor position
	[[nodiscard]]
	// cppcheck-suppress functionStatic ; console method (one console API)
	auto vga::console_t::cursorGet() const noexcept -> vmemCursor { // NOLINT(readability-convert-member-functions-to-static): console method (one console API)
		// Choose cursor location high register
		io::writePort8(VGA_CURSOR_CONTROL, 0x0E_u8);
		// Read cursor position high byte
		auto position {static_cast<igros_word_t>(io::readPort8(VGA_CURSOR_DATA))};
		// Choose cursor location low register
		io::writePort8(VGA_CURSOR_CONTROL, 0x0F_u8);
		// Read cursor position low byte
		position = static_cast<igros_word_t>((static_cast<igros_dword_t>(position) << 8) | io::readPort8(VGA_CURSOR_DATA));
		// Return cursor data
		return vmemCursor {
			.x = static_cast<igros_byte_t>(position % VIDEO_MEM_WIDTH),
			.y = static_cast<igros_byte_t>(position / VIDEO_MEM_WIDTH),
		};
	}


	// Disable cursor
	// cppcheck-suppress functionStatic ; console method (one console API)
	void vga::console_t::cursorDisable() noexcept { // NOLINT(readability-convert-member-functions-to-static): console method (one console API)
		// Choose cursor start register
		io::writePort8(VGA_CURSOR_CONTROL, VIDEO_MEM_CURSOR_START);
		// Send control word to disable cursor
		io::writePort8(VGA_CURSOR_DATA, 0x20_u8);
	}

	// Enable cursor
	// cppcheck-suppress functionStatic ; console method (one console API)
	void vga::console_t::cursorEnable() noexcept { // NOLINT(readability-convert-member-functions-to-static): console method (one console API)
		// Choose cursor start register
		io::writePort8(VGA_CURSOR_CONTROL, VIDEO_MEM_CURSOR_START);
		// Get current register value
		const auto cursorStartReg {io::readPort8(VGA_CURSOR_DATA)};
		// Clear cursor disable bit
		io::writePort8(VGA_CURSOR_DATA, static_cast<igros_byte_t>(static_cast<igros_dword_t>(cursorStartReg) & ~0x20_u32));
	}

	// Set text colors
	void vga::console_t::setColor(const vmemColor background, const vmemColor foreground) noexcept {
		// Background is first 4 bits and foreground is next 4
		mColor = static_cast<igros_byte_t>((static_cast<igros_dword_t>(background) << 4) | static_cast<igros_dword_t>(foreground));
	}


	// Write symbol
	void vga::console_t::write(const char symbol) noexcept {
		// Backspace symbol
		if (symbol == '\b') {
			// If we are not at start
			if (0_u8 != mCursor.x) {
				// Move 1 symbol backward
				--mCursor.x;
			} else {
				mCursor.x = VIDEO_MEM_WIDTH - 1_u16;
				// Move 1 line up
				--mCursor.y;
			}
			// Tabulation symbol
		} else if (symbol == '\t') {
			// calculate new tab offset
			mCursor.x = static_cast<igros_byte_t>((mCursor.x + VIDEO_MEM_TAB_SIZE) & ~(VIDEO_MEM_TAB_SIZE - 1_usize));
			// Carret return
		} else if (symbol == '\r') {
			// Move to start of the row
			mCursor.x = 0_u8;
			// Carret new line
		} else if (symbol == '\n') {
			// Move to start of the row
			mCursor.x = 0_u8;
			// Move to next row
			++mCursor.y;
			// If non-control (printable) character
		} else if (symbol >= ' ') {
			// Write symbol to VGA console
			vmemScreen()[(mCursor.y * VIDEO_MEM_WIDTH) + mCursor.x] = vmemSymbol {
				.symbol = symbol,
				.color	= mColor,
			};
			// Move cursor 1 symbol right
			++mCursor.x;
		}
		// Check if we are not out of columns
		if (mCursor.x >= VIDEO_MEM_WIDTH) {
			// Move to start of the row
			mCursor.x = 0_u8;
			// Move to next row
			++mCursor.y;
		}
		// Chech if we are not out of rows
		if (mCursor.y >= VIDEO_MEM_HEIGHT) {
			// Move cursor to the last line
			mCursor.y = VIDEO_MEM_HEIGHT - 1_u16;
			// Move screen 1 line up
			const auto screen {vmemScreen()};
			std::shift_left(screen.begin(), screen.end(), VIDEO_MEM_WIDTH); // NOLINT(modernize-use-ranges): std::ranges::shift_left is C++23
			// Clear bottom line
			std::ranges::fill(screen.last<VIDEO_MEM_WIDTH>(), blank());
		}
		// Set new cursor position
		cursorSet(mCursor.x, mCursor.y);
	}

	// Write string
	void vga::console_t::write(const char* const message) noexcept {
		write(klib::kstrview(message));
	}

	// Write fixed-width string
	void vga::console_t::write(const std::string_view message) noexcept {
		// Write symbols one by one
		for (const auto symbol : message) {
			write(symbol);
		}
	}

	// Clear console
	// cppcheck-suppress functionConst ; changes device (hardware) state
	void vga::console_t::clear() noexcept {
		// Set whole screen with whitespace with current background
		std::ranges::fill(vmemScreen(), blank());
	}


	// Init console
	[[nodiscard]]
	auto vga::console_t::init() noexcept -> bool {
		// Clear screen
		clear();
		// Place cursor at (0, 0)
		cursorSet(0_u8, 0_u8);
		// Disable cursor
		cursorDisable();
		// Success
		return true;
	}

	// Write text bytes
	[[maybe_unused]]
	auto vga::console_t::write(const std::span<const std::byte> data) noexcept -> igros_usize_t {
		// Write symbols one by one (console always accepts output)
		for (const auto symbol : data) {
			write(static_cast<char>(symbol));
		}
		return data.size();
	}

	// Console control
	[[nodiscard]]
	auto vga::console_t::ioctl(const igros_dword_t request, const igros_pointer_t arg) noexcept -> bool {
		switch (static_cast<IOCTL>(request)) {
			case IOCTL::CLEAR:
				clear();
				return true;
			case IOCTL::SET_COLOR:
				if (nullptr == arg) [[unlikely]] {
					return false;
				}
				setColor(static_cast<const colors_t*>(arg)->background, static_cast<const colors_t*>(arg)->foreground);
				return true;
			case IOCTL::SET_CURSOR:
				if (nullptr == arg) [[unlikely]] {
					return false;
				}
				cursorSet(*static_cast<const vmemCursor*>(arg));
				return true;
			case IOCTL::GET_CURSOR:
				if (nullptr == arg) [[unlikely]] {
					return false;
				}
				*static_cast<vmemCursor*>(arg) = cursorGet();
				return true;
			case IOCTL::SHOW_CURSOR:
				cursorEnable();
				return true;
			case IOCTL::HIDE_CURSOR:
				cursorDisable();
				return true;
			default:
				// Unknown request
				return false;
		}
	}


	// Write symbol to kernel console
	void vmemWrite(const char symbol) noexcept {
		vga::console.write(symbol);
	}

	// Write string to kernel console
	void vmemWrite(const char* const message) noexcept {
		vga::console.write(message);
	}

	// Write fixed-width string to kernel console
	void vmemWrite(const std::string_view message) noexcept {
		vga::console.write(message);
	}


} // namespace igros::arch

