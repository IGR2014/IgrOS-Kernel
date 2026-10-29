////////////////////////////////////////////////////////////////
//
//	Freestanding C library functions tests (compiler-generated calls)
//
//	File:	test_libc.cpp
//	Date:	19 Oct 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


// C++
#include <cstddef>
// Tests
#include <tests/test.hpp>


// Kernel implementations replace host ones in this test (built with -fno-builtin)
extern "C" {
	void*	memset(void* dst, int val, std::size_t size);
	void*	memcpy(void* dst, const void* src, std::size_t size);
	void*	memmove(void* dst, const void* src, std::size_t size);
	int	memcmp(const void* lhs, const void* rhs, std::size_t size);
}


// Buffer equals text (size bytes)
[[nodiscard]]
static auto equals(const char* const buffer, const char* const text, const std::size_t size) -> bool {
	for (auto i {std::size_t {0}}; i < size; ++i) {
		if (buffer[i] != text[i]) {
			return false;
		}
	}
	return true;
}


int main() {

	char buffer[16] {};

	// memset: byte value, exact size, returns destination
	IGROS_CHECK(buffer == memset(buffer, 'x', 4));
	IGROS_CHECK(equals(buffer, "xxxx\0", 5));
	// Value is converted to unsigned char (0x141 -> 0x41): checked on purpose
	// cppcheck-suppress memsetValueOutOfRange
	memset(buffer, 0x141, 2);
	IGROS_CHECK(equals(buffer, "AAxx", 4));
	// (size in variable: literal 0 triggers transposed arguments warning)
	auto empty {std::size_t {0}};
	memset(buffer, 'z', empty);
	IGROS_CHECK(equals(buffer, "AAxx", 4));

	// memcpy
	IGROS_CHECK(buffer == memcpy(buffer, "hello", 5));
	IGROS_CHECK(equals(buffer, "hello\0", 6));

	// memmove: overlapping both directions
	memcpy(buffer, "abcdef", 7);
	IGROS_CHECK(buffer + 2 == memmove(buffer + 2, buffer, 4));
	IGROS_CHECK(equals(buffer, "ababcd", 6));
	memcpy(buffer, "abcdef", 7);
	memmove(buffer, buffer + 2, 4);
	IGROS_CHECK(equals(buffer, "cdefef", 6));
	memmove(buffer, buffer, 6);
	IGROS_CHECK(equals(buffer, "cdefef", 6));

	// memcmp: unsigned bytes, first difference (constant inputs on purpose)
	// cppcheck-suppress-begin staticStringCompare
	IGROS_CHECK(0 == memcmp("abc", "abc", 3));
	IGROS_CHECK(0 == memcmp("abc", "abd", 2));
	IGROS_CHECK(memcmp("abc", "abd", 3) < 0);
	IGROS_CHECK(memcmp("abd", "abc", 3) > 0);
	IGROS_CHECK(memcmp("\x80", "\x7F", 1) > 0);
	// cppcheck-suppress-end staticStringCompare

	return igros::test::result();

}

