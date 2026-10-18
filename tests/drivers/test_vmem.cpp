////////////////////////////////////////////////////////////////
//
//	VGA text mode driver tests
//
//	File:	test_vmem.cpp
//	Date:	19 Oct 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


// C++
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

asm(
	".globl " IGROS_TEST_SYMBOL(KERNEL_OFFSET_VIRT) "; "
	".set " IGROS_TEST_SYMBOL(KERNEL_OFFSET_VIRT) ", " IGROS_TEST_SYMBOL(vgaMemory) " - 0xB8000"
);


// Cursor ports
constexpr auto VGA_CURSOR_CONTROL	{std::uint16_t {0x3D4}};
constexpr auto VGA_CURSOR_DATA		{std::uint16_t {0x3D5}};


// Symbol at screen position
[[nodiscard]]
static auto at(const igros::igros_usize_t x, const igros::igros_usize_t y) -> const vmemSymbol& {
	return vgaMemory[y * VIDEO_MEM_WIDTH + x];
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
	for (auto &symbol : vgaMemory) {
		symbol = {'#', 0x00};
	}
	IGROS_CHECK(vmemInit());
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
	IGROS_CHECK(vmemInit());
	vmemSetColor(static_cast<igros::igros_byte_t>(vmemColor::Blue), static_cast<igros::igros_byte_t>(vmemColor::White));
	vmemWrite("Hi");
	IGROS_CHECK(row(0, "Hi"));
	IGROS_CHECK(0x1F == at(0, 0).color);
	IGROS_CHECK(2 == cursorWritten());
	// Tab to next 8-column stop, backspace, carriage return, newline
	vmemWrite('\t');
	IGROS_CHECK(8 == cursorWritten());
	vmemWrite("x\b");
	IGROS_CHECK(8 == cursorWritten());
	vmemWrite("y\rZ\n");
	IGROS_CHECK(row(0, "Zi      y"));
	IGROS_CHECK(VIDEO_MEM_WIDTH == cursorWritten());
	// Non-printable characters are ignored
	vmemWrite("\x01", 1);
	IGROS_CHECK(VIDEO_MEM_WIDTH == cursorWritten());
	// Backspace at line start goes to previous line end
	vmemWrite('\b');
	IGROS_CHECK(VIDEO_MEM_WIDTH - 1 == cursorWritten());
}


// Line wrap and scrolling at screen bottom
static void testScroll() {
	hw::reset();
	IGROS_CHECK(vmemInit());
	vmemWrite("first\n");
	for (auto i {igros::igros_usize_t {1}}; i < VIDEO_MEM_HEIGHT - 1; ++i) {
		vmemWrite("\n");
	}
	vmemWrite("last");
	IGROS_CHECK(row(0, "first"));
	IGROS_CHECK(row(VIDEO_MEM_HEIGHT - 1, "last"));
	// Wrap past last column of bottom line scrolls screen up
	for (auto i {igros::igros_usize_t {4}}; i < VIDEO_MEM_WIDTH; ++i) {
		vmemWrite('.');
	}
	IGROS_CHECK(row(VIDEO_MEM_HEIGHT - 2, "last...."));
	IGROS_CHECK(row(VIDEO_MEM_HEIGHT - 1, "    "));
	IGROS_CHECK(!row(0, "first"));
	IGROS_CHECK((VIDEO_MEM_HEIGHT - 1) * VIDEO_MEM_WIDTH == cursorWritten());
}


// Cursor position read back, cursor enable keeps start scanline
static void testCursor() {
	hw::reset();
	vmemCursorSet(vmemCursor {.x = 5, .y = 3});
	IGROS_CHECK(3 * VIDEO_MEM_WIDTH + 5 == cursorWritten());
	// CRT controller model: data port returns selected register
	auto registers {std::vector<std::uint8_t>(0x10, 0)};
	registers[0x0A] = 0x2E;
	registers[0x0E] = 0x01;
	registers[0x0F] = 0x45;
	hw::portReadHook = [&registers](const std::uint16_t port) -> std::uint32_t {
		return (VGA_CURSOR_DATA == port) ? registers[hw::portValue[VGA_CURSOR_CONTROL]] : hw::portValue[port];
	};
	// 0x0145 = 325 = row 4, column 5
	const auto cursor {vmemCursorGet()};
	IGROS_CHECK((5 == cursor.x) && (4 == cursor.y));
	vmemCursorEnable();
	IGROS_CHECK(0x0E == hw::portValue[VGA_CURSOR_DATA]);
	vmemCursorDisable();
	IGROS_CHECK(0x20 == hw::portValue[VGA_CURSOR_DATA]);
}


int main() {
	testInit();
	testWrite();
	testScroll();
	testCursor();
	return igros::test::result();
}

