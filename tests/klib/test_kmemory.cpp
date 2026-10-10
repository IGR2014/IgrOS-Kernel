////////////////////////////////////////////////////////////////
//
//	Kernel memory functions tests
//
//	File:	test_kmemory.cpp
//	Date:	11 Oct 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


// C++
#include <cstdint>
// IgrOS-Kernel library
#include <klib/kmemory.hpp>
// Tests
#include <tests/test.hpp>


using namespace igros::klib;


// VGA-like cell (fill value must be a whole cell, not an int-promoted expression)
struct cell {
	signed char	symbol;
	unsigned char	color;
};


int main() {

	// Fill typed elements, nothing past count
	cell screen[8] {};
	kmemset(screen, 4, cell {' ', 0x2A});
	for (auto i {0}; i < 8; ++i) {
		if (i < 4) {
			IGROS_CHECK((' ' == screen[i].symbol) && (0x2A == screen[i].color));
		} else {
			IGROS_CHECK((0 == screen[i].symbol) && (0 == screen[i].color));
		}
	}

	// Value is converted to element type
	char text[6] {"abcde"};
	kmemset(text, 3, static_cast<unsigned char>('x'));
	IGROS_CHECK(('x' == text[0]) && ('x' == text[2]) && ('d' == text[3]));

	// Zero objects bytewise
	std::uint32_t words[4] {1, 2, 3, 4};
	kmemzero(words);
	IGROS_CHECK((0 == words[0]) && (2 == words[1]));
	kmemzero(words, 4);
	for (const auto word : words) {
		IGROS_CHECK(0 == word);
	}
	IGROS_CHECK(nullptr == kmemzero(static_cast<std::uint32_t*>(nullptr)));

	// Copy
	const char source[] {"copy"};
	char destination[5] {};
	IGROS_CHECK(destination == kmemcpy(destination, source, sizeof(source)));
	IGROS_CHECK(('c' == destination[0]) && ('y' == destination[3]) && ('\0' == destination[4]));
	IGROS_CHECK(destination == kmemcpy(destination, destination, sizeof(destination)));
	IGROS_CHECK(destination == kmemcpy(destination, source, 0));
	IGROS_CHECK(nullptr == kmemcpy(nullptr, source, 1));

	return igros::test::result();

}

