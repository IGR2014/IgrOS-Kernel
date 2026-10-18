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
#include <span>
// IgrOS-Kernel library
#include <klib/kprint.hpp>
// Tests
#include <tests/hw.hpp>


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

	// Short and byte sizes (arguments promoted to int)
	checkFormat(64, "-1|65535|abcd", "%hd|%hu|%hx", -1, 0xFFFF, 0x1234ABCD);
	checkFormat(64, "-128|44|cd", "%hhd|%hhu|%hhx", 0x80, 300, 0x1234ABCD);

	// Other conversions
	checkFormat(64, "10|-5|4294967295", "%o|%i|%u", 8, -5, 0xFFFFFFFFu);
	checkFormat(64, "d=-3 l=7", "d=%td l=%lu", static_cast<std::ptrdiff_t>(-3), 7UL);
	checkFormat(64, "-42|42", "%zd|%zu", static_cast<std::ptrdiff_t>(-42), static_cast<std::size_t>(42));
	checkFormat(64, "a?b", "a%qb");

	// Pointer: zero-padded hex of pointer width
	char pointer[32];
	std::snprintf(pointer, sizeof(pointer), "%0*zx", static_cast<int>(sizeof(void*) * 2), static_cast<std::size_t>(0x1234));
	checkFormat(64, pointer, "%p", reinterpret_cast<void*>(0x1234));

	// Sign before zero padding, left-justify, uppercase hex
	checkFormat(64, "-0042|42   |-7   |ff|FF", "%05d|%-5d|%-05d|%x|%X", -42, 42, -7, 255u, 255u);
	// Width for strings and characters
	checkFormat(64, "[   ab|ab   |  Z]", "[%5s|%-5s|%3c]", "ab", "ab", 'Z');
	checkFormat(64, "[    ]", "[%4s]", static_cast<const char*>(nullptr));

	// 64-bit decimals (split into 9-digit chunks on 32-bit CPU)
	checkFormat(64, "4294967295|4294967296", "%llu|%llu", 4294967295ull, 4294967296ull);
	checkFormat(64, "1000000000|10000000000000000000", "%llu|%llu", 1000000000ull, 10000000000000000000ull);
	checkFormat(64, "123456789012345678|-1000000000000", "%llu|%lld", 123456789012345678ull, -1000000000000ll);
	checkFormat(80, "1000000000000000000000000000000000000000000000000000000000000000", "%llb", 0x8000000000000000ull);
	checkFormat(64, "1777777777777777777777", "%llo", 0xFFFFFFFFFFFFFFFFull);

	// Written length returned (without terminator, clipped to buffer)
	char lengthBuffer[8];
	IGROS_CHECK(5 == igros::klib::ksnprintf(lengthBuffer, sizeof(lengthBuffer), "%d-%s", 42, "ab"));
	IGROS_CHECK(7 == igros::klib::ksnprintf(lengthBuffer, sizeof(lengthBuffer), "%s", "1234567890"));
	IGROS_CHECK(0 == igros::klib::ksnprintf(lengthBuffer, 0, "%d", 1));

	// Integer to text: sign for decimal only, written only if it fits, no terminator
	char number[8] {'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X'};
	igros::klib::kitoa(number, -123);
	IGROS_CHECK(0 == std::memcmp(number, "-123XXXX", 8));
	igros::klib::kitoa(number, static_cast<std::int8_t>(-1), igros::klib::radix_t::HEX);
	IGROS_CHECK(0 == std::memcmp(number, "ff23XXXX", 8));
	igros::klib::kitoa(std::span {number}.first(3), 12345);
	IGROS_CHECK(0 == std::memcmp(number, "ff23XXXX", 8));
	static_assert([] {
		std::array<char, 8> digits {};
		return igros::klib::kdigits(digits, 0xBEEFu, igros::klib::radix_t::HEX, true) == "BEEF";
	}());

	// Invalid arguments: nothing written
	char untouched[4] {'X', 'X', 'X', 'X'};
	igros::klib::ksnprintf(nullptr, 4, "%d", 1);
	igros::klib::ksnprintf(untouched, 0, "%d", 1);
	igros::klib::ksnprintf(untouched, sizeof(untouched), nullptr);
	IGROS_CHECK(0 == std::memcmp(untouched, "XXXX", 4));

	// Kernel printf: line to console, interrupts state restored
	igros::test::hw::reset();
	igros::test::hw::irqEnabled = true;
	igros::klib::kprintf("x=%d", 5);
	IGROS_CHECK("x=5\n" == igros::test::hw::console);
	IGROS_CHECK(igros::test::hw::irqEnabled);

	return igros::test::result();

}

