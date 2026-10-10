////////////////////////////////////////////////////////////////
//
//	Kernel string functions tests
//
//	File:	test_kstring.cpp
//	Date:	11 Oct 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


// C++
#include <cstring>
// IgrOS-Kernel library
#include <klib/kstring.hpp>
// Tests
#include <tests/test.hpp>


using namespace igros::klib;


int main() {

	// Length (also at compile time)
	static_assert(4 == kstrlen("abcd"));
	IGROS_CHECK(0 == kstrlen(""));
	IGROS_CHECK(3 == kstrlen("abc"));
	IGROS_CHECK(0 == kstrlen(nullptr));

	// End of string keeps const-ness of argument
	const char* const text {"hello"};
	IGROS_CHECK(text + 5 == kstrend(text));

	// Copy: at most size characters, always terminated, nothing past it
	char buffer[8];
	std::memset(buffer, 'X', sizeof(buffer));
	kstrcpy(buffer, "hello", 3);
	IGROS_CHECK(0 == std::strcmp(buffer, "hel"));
	IGROS_CHECK('X' == buffer[4]);
	kstrcpy(buffer, "hi", 5);
	IGROS_CHECK(0 == std::strcmp(buffer, "hi"));
	kstrcpy(buffer, "", 5);
	IGROS_CHECK('\0' == buffer[0]);

	// Concatenation truncated to destination buffer size
	char concat[8] {"ab"};
	kstrcat(concat, "cdefghij", sizeof(concat));
	IGROS_CHECK(0 == std::strcmp(concat, "abcdefg"));
	char full[4] {'a', 'b', 'c', 'd'};
	kstrcat(full, "x", sizeof(full));
	IGROS_CHECK('d' == full[3]);

	// Compare
	IGROS_CHECK(0 == kstrcmp("abc", "abc", 10));
	IGROS_CHECK(0 > kstrcmp("abc", "abd", 10));
	IGROS_CHECK(0 < kstrcmp("abd", "abc", 10));
	IGROS_CHECK(0 == kstrcmp("abc", "abd", 2));
	IGROS_CHECK(0 == kstrcmp("abc", "xyz", 0));
	IGROS_CHECK(0 > kstrcmp("ab", "abc", 10));
	IGROS_CHECK(0 < kstrcmp("\xff", "a", 1));
	IGROS_CHECK(0 > kstrcmp(nullptr, "a", 1));
	IGROS_CHECK(0 < kstrcmp("a", nullptr, 1));
	IGROS_CHECK(0 == kstrcmp(nullptr, nullptr, 1));

	// Find character
	IGROS_CHECK(text + 2 == kstrchr(text, 'l', 10));
	IGROS_CHECK(nullptr == kstrchr(text, 'o', 4));
	IGROS_CHECK(nullptr == kstrchr(text, 'z', 10));
	IGROS_CHECK(text + 5 == kstrchr(text, '\0', 10));
	char mutableText[] {"abc"};
	char* const found {kstrchr(mutableText, 'b', 3)};
	IGROS_CHECK(mutableText + 1 == found);

	// Reverse
	char odd[] {"abcde"};
	kstrinv(odd, 10);
	IGROS_CHECK(0 == std::strcmp(odd, "edcba"));
	char even[] {"abcd"};
	kstrinv(even, 4);
	IGROS_CHECK(0 == std::strcmp(even, "dcba"));
	char partial[] {"abcdef"};
	kstrinv(partial, 3);
	IGROS_CHECK(0 == std::strcmp(partial, "cbadef"));
	char empty[4] {};
	empty[1] = 'Q';
	kstrinv(empty, 3);
	IGROS_CHECK('Q' == empty[1]);
	char zero[] {"ab"};
	kstrinv(zero, 0);
	IGROS_CHECK(0 == std::strcmp(zero, "ab"));

	return igros::test::result();

}

