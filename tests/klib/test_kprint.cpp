////////////////////////////////////////////////////////////////
//
//	kvsnprintf / kitoa tests
//
//	File:	test_kprint.cpp
//	Date:	11 Oct 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


// C++
#include <cstdint>
#include <cstring>
// IgrOS-Kernel library
#include <klib/kprint.hpp>
// Tests
#include <tests/test.hpp>


// Format into buffer of given size, check result and that nothing was written past it
template<class ...Args>
static void checkFormat(const igros::igros_usize_t size, const char* const expected, const char* const format, Args ...args) {
	char buffer[128];
	std::memset(buffer, 'X', sizeof(buffer));
	igros::klib::ksnprintf(buffer, size, format, args...);
	const auto matches {0 == std::strcmp(buffer, expected)};
	auto untouched {true};
	for (auto i {size}; i < sizeof(buffer); ++i) {
		untouched = untouched && ('X' == buffer[i]);
	}
	if (!matches || !untouched) {
		std::printf("format \"%s\": got \"%s\", expected \"%s\"%s\n", format, buffer, expected, untouched ? "" : " (buffer overflow)");
	}
	IGROS_CHECK(matches && untouched);
}


int main() {

	// Basic conversions
	checkFormat(64, "hello 42", "hello %d", 42);
	checkFormat(64, "-7|ff|0012", "%d|%x|%04d", -7, 255u, 12);
	checkFormat(64, "s=abc c=Z %", "s=%s c=%c %%", "abc", 'Z');
	checkFormat(64, "0", "%d", 0);
	checkFormat(64, "0000002a", "%08x", 42u);
	checkFormat(64, "11111111111111111111111111111111", "%b", 0xFFFFFFFFu);

	// Signed limits and two's complement
	checkFormat(64, "-2147483648", "%d", static_cast<std::int32_t>(0x80000000u));
	checkFormat(64, "ffffffff", "%x", 0xFFFFFFFFu);
	checkFormat(64, "-9223372036854775808", "%lld", static_cast<long long>(0x8000000000000000ull));
	checkFormat(64, "18446744073709551615", "%llu", 0xFFFFFFFFFFFFFFFFull);

	// Length modifiers
	checkFormat(64, "L=123", "L=%ld", 123L);
	checkFormat(64, "Q=12345678901", "Q=%llu", 12345678901ull);
	checkFormat(64, "size=42", "size=%zu", static_cast<std::size_t>(42));
	checkFormat(64, "x=ff", "x=%zx", static_cast<std::size_t>(255));

	// Field width (multiple digits)
	checkFormat(64, "[        12]", "[%10d]", 12);
	checkFormat(64, "00000000deadbeef", "%016llx", 0xDEADBEEFull);
	checkFormat(64, "ffffffffffffffff", "%016llx", 0xFFFFFFFFFFFFFFFFull);

	// Output truncated to buffer size (never past it)
	checkFormat(8, "1234567", "%s", "1234567890");
	checkFormat(8, "abcdefg", "abcdefghijk");
	checkFormat(6, "     ", "%9d", 5);
	checkFormat(5, "ab12", "ab%d", 123456);
	checkFormat(1, "", "anything %d", 1);

	// Unterminated placeholder at format end
	checkFormat(64, "tail", "tail%");

	return igros::test::result();

}

