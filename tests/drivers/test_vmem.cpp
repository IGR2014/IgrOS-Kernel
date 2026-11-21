////////////////////////////////////////////////////////////////
//
//	VGA text mode driver tests
//
//	File:	test_vmem.cpp
//	Date:	03 Nov 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


// C++
#include <array>
#include <cstddef>
#include <vector>
// IgrOS-Kernel drivers
#include <drivers/vga/vmem.hpp>
// Tests
#include <tests/hw.hpp>


using namespace igros::arch;
namespace hw = igros::test::hw;


// Host video memory: kernel offset placed so that physical 0xB8000 is this buffer
extern "C" {
	alignas(16) vmemSymbol vgaMemory[VIDEO_MEM_SIZE];
}

// clang-format off
asm(
	".globl " IGROS_TEST_SYMBOL(KERNEL_OFFSET_VIRT) "; "
	".set " IGROS_TEST_SYMBOL(KERNEL_OFFSET_VIRT) ", " IGROS_TEST_SYMBOL(vgaMemory) " - 0xB8000"
);
// clang-format on


// Cursor ports
constexpr auto VGA_CURSOR_CONTROL {std::uint16_t {0x3D4}};
constexpr auto VGA_CURSOR_DATA {std::uint16_t {0x3D5}};


// Symbol at screen position
[[nodiscard]]
static auto at(const igros::igros_usize_t x, const igros::igros_usize_t y) -> const vmemSymbol & {
	return vgaMemory[(y * VIDEO_MEM_WIDTH) + x];
}

// Screen row starts with text
[[nodiscard]]
static auto row(const igros::igros_usize_t y, const char* const text) -> bool {
	for (auto x {igros::igros_usize_t {0}}; '\0' != text[x]; ++x) {
		if (text[x] != at(x, y).symbol) {
			return false;
		}
	}
	return true;
}

// Cursor position written to CRT controller (registers 0x0E high, 0x0F low)
[[nodiscard]]
static auto cursorWritten() -> igros::igros_usize_t {
	auto position {igros::igros_usize_t {0}};
	auto index {0u};
	for (const auto &access : hw::portLog) {
		if (!access.write) {
			continue;
		}
		if (VGA_CURSOR_CONTROL == access.port) {
			index = access.value;
		} else if ((VGA_CURSOR_DATA == access.port) && (0x0E == index)) {
			position = (position & 0x00FF) | (access.value << 8);
		} else if ((VGA_CURSOR_DATA == access.port) && (0x0F == index)) {
			position = (position & 0xFF00) | access.value;
		}
	}
	return position;
}


// Init: screen cleared with background color, cursor at origin and hidden
static void testInit() {
	hw::reset();
	auto console {vga::console_t {}};
	for (auto &symbol : vgaMemory) {
		symbol = {.symbol = '#', .color = 0x00};
	}
	IGROS_CHECK(console.init());
	auto blank {true};
	for (const auto &symbol : vgaMemory) {
		blank = blank && (' ' == symbol.symbol) && (static_cast<igros::igros_byte_t>(vmemColor::Green) == symbol.color);
	}
	IGROS_CHECK(blank);
	IGROS_CHECK(0 == cursorWritten());
	IGROS_CHECK((0x0A == hw::portValue[VGA_CURSOR_CONTROL]) && (0x20 == hw::portValue[VGA_CURSOR_DATA]));
}


// Text output with color and control characters
static void testWrite() {
	hw::reset();
	auto console {vga::console_t {}};
	IGROS_CHECK(console.init());
	console.setColor(vmemColor::Blue, vmemColor::White);
	console.write("Hi");
	IGROS_CHECK(row(0, "Hi"));
	IGROS_CHECK(0x1F == at(0, 0).color);
	IGROS_CHECK(2 == cursorWritten());
	// Tab to next 8-column stop, backspace, carriage return, newline
	console.write('\t');
	IGROS_CHECK(8 == cursorWritten());
	console.write("x\b");
	IGROS_CHECK(8 == cursorWritten());
	console.write("y\rZ\n");
	IGROS_CHECK(row(0, "Zi      y"));
	IGROS_CHECK(VIDEO_MEM_WIDTH == cursorWritten());
	// Non-printable characters are ignored
	console.write(std::string_view {"\x01", 1});
	IGROS_CHECK(VIDEO_MEM_WIDTH == cursorWritten());
	// Backspace at line start goes to previous line end
	console.write('\b');
	IGROS_CHECK(VIDEO_MEM_WIDTH - 1 == cursorWritten());
}


// Line wrap and scrolling at screen bottom
static void testScroll() {
	hw::reset();
	auto console {vga::console_t {}};
	IGROS_CHECK(console.init());
	console.write("first\n");
	for (auto i {igros::igros_usize_t {1}}; i < VIDEO_MEM_HEIGHT - 1; ++i) {
		console.write("\n");
	}
	console.write("last");
	IGROS_CHECK(row(0, "first"));
	IGROS_CHECK(row(VIDEO_MEM_HEIGHT - 1, "last"));
	// Wrap past last column of bottom line scrolls screen up
	for (auto i {igros::igros_usize_t {4}}; i < VIDEO_MEM_WIDTH; ++i) {
		console.write('.');
	}
	IGROS_CHECK(row(VIDEO_MEM_HEIGHT - 2, "last...."));
	IGROS_CHECK(row(VIDEO_MEM_HEIGHT - 1, "    "));
	IGROS_CHECK(!row(0, "first"));
	IGROS_CHECK((VIDEO_MEM_HEIGHT - 1) * VIDEO_MEM_WIDTH == cursorWritten());
}


// Cursor position read back, cursor enable keeps start scanline
static void testCursor() {
	hw::reset();
	auto console {vga::console_t {}};
	console.cursorSet(vmemCursor {.x = 5, .y = 3});
	IGROS_CHECK((3 * VIDEO_MEM_WIDTH) + 5 == cursorWritten());
	// CRT controller model: data port returns selected register
	auto registers {std::vector<std::uint8_t>(0x10, 0)};
	registers[0x0A]	 = 0x2E;
	registers[0x0E]	 = 0x01;
	registers[0x0F]	 = 0x45;
	hw::portReadHook = [&registers](const std::uint16_t port) -> std::uint32_t {
		return (VGA_CURSOR_DATA == port) ? registers[hw::portValue[VGA_CURSOR_CONTROL]] : hw::portValue[port];
	};
	// 0x0145 = 325 = row 4, column 5
	const auto cursor {console.cursorGet()};
	IGROS_CHECK((5 == cursor.x) && (4 == cursor.y));
	console.cursorEnable();
	IGROS_CHECK(0x0E == hw::portValue[VGA_CURSOR_DATA]);
	console.cursorDisable();
	IGROS_CHECK(0x20 == hw::portValue[VGA_CURSOR_DATA]);
}


// Driver interface: text output and console controls
static void testDriver() {
	hw::reset();
	IGROS_CHECK(igros::drivers::init(vga::DRIVER));
	// Kernel console output goes to console driver
	vmemWrite("text");
	IGROS_CHECK(row(0, "text"));
	IGROS_CHECK(vga::DRIVER.ioctl(vga::DRIVER.context, static_cast<igros::igros_dword_t>(vga::IOCTL::CLEAR), nullptr));
	IGROS_CHECK(row(0, "    "));
	// Colors and cursor position (row 2, column 3)
	auto colors {vga::colors_t {.background = vmemColor::Red, .foreground = vmemColor::Yellow}};
	IGROS_CHECK(igros::drivers::ioctl(vga::DRIVER, vga::IOCTL::SET_COLOR, &colors));
	auto cursor {vmemCursor {.x = 3, .y = 2}};
	IGROS_CHECK(igros::drivers::ioctl(vga::DRIVER, vga::IOCTL::SET_CURSOR, &cursor));
	IGROS_CHECK(3 == igros::drivers::write(vga::DRIVER, "a\tb"));
	IGROS_CHECK(('a' == at(3, 2).symbol) && (0x4E == at(3, 2).color) && ('b' == at(8, 2).symbol));
	IGROS_CHECK((2 * VIDEO_MEM_WIDTH) + 9 == cursorWritten());
	// Cursor read back from CRT controller (position 0x0145 = row 4, column 5)
	hw::portReadHook = [](const std::uint16_t port) -> std::uint32_t {
		if (VGA_CURSOR_DATA != port) {
			return hw::portValue[port];
		}
		return (0x0E == hw::portValue[VGA_CURSOR_CONTROL]) ? 0x01u : 0x45u;
	};
	IGROS_CHECK(igros::drivers::ioctl(vga::DRIVER, vga::IOCTL::GET_CURSOR, &cursor));
	IGROS_CHECK((5 == cursor.x) && (4 == cursor.y));
	hw::portReadHook = nullptr;
	IGROS_CHECK(igros::drivers::ioctl(vga::DRIVER, vga::IOCTL::SHOW_CURSOR));
	IGROS_CHECK(igros::drivers::ioctl(vga::DRIVER, vga::IOCTL::HIDE_CURSOR));
	IGROS_CHECK(0x20 == hw::portValue[VGA_CURSOR_DATA]);
	// Missing argument, unknown request, console can't be read
	IGROS_CHECK(!igros::drivers::ioctl(vga::DRIVER, vga::IOCTL::SET_COLOR));
	IGROS_CHECK(!igros::drivers::ioctl(vga::DRIVER, vga::IOCTL::SET_CURSOR));
	IGROS_CHECK(!igros::drivers::ioctl(vga::DRIVER, vga::IOCTL::GET_CURSOR));
	IGROS_CHECK(!vga::DRIVER.ioctl(vga::DRIVER.context, 0xFFu, &cursor));
	IGROS_CHECK(&vga::console == vga::DRIVER.context);
	auto buffer {std::array<std::byte, 4> {}};
	IGROS_CHECK(0 == igros::drivers::read(vga::DRIVER, buffer)); // flawfinder: ignore (device read, bounded by span size)
}


auto main() -> int {
	testInit();
	testWrite();
	testScroll();
	testCursor();
	testDriver();
	return igros::test::result();
}

