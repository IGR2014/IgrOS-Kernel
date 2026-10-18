////////////////////////////////////////////////////////////////
//
//	Kernel alignment helpers tests
//
//	File:	test_kalign.cpp
//	Date:	19 Oct 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


// C++
#include <cstdint>
// IgrOS-Kernel library
#include <klib/kAlign.hpp>
// Tests
#include <tests/test.hpp>


using igros::klib::kAlign;


// Alignment must be power of two (checked at compile time)
template<igros::igros_usize_t ALIGN>
concept validAlign = requires { kAlign::up<ALIGN>(0u); };

static_assert(validAlign<0x1000>);
static_assert(validAlign<0>);
static_assert(!validAlign<0x1800>);


// Integers: compile time
static_assert(0x2000u == kAlign::up<0x1000>(0x1001u));
static_assert(0x1000u == kAlign::up<0x1000>(0x1000u));
static_assert(0x1000u == kAlign::down<0x1000>(0x1FFFu));
static_assert(kAlign::check<0x1000>(0x3000u));
static_assert(!kAlign::check<0x1000>(0x3008u));
// No natural alignment for integers
static_assert(0x1001u == kAlign::up(0x1001u));
// 64-bit addresses are not truncated (i386 too)
static_assert(0x100002000ull == kAlign::up<0x1000>(0x100001001ull));
static_assert(0x100001000ull == kAlign::down<0x1000>(0x100001FFFull));


int main() {

	// Pointers: explicit and natural (pointed type) alignment
	const auto* const ptr {reinterpret_cast<const std::uint32_t*>(0x1006)};
	IGROS_CHECK(0x1008 == reinterpret_cast<std::uintptr_t>(kAlign::up(ptr)));
	IGROS_CHECK(0x1004 == reinterpret_cast<std::uintptr_t>(kAlign::down(ptr)));
	IGROS_CHECK(0x1010 == reinterpret_cast<std::uintptr_t>(kAlign::up<16>(ptr)));
	IGROS_CHECK(0x1000 == reinterpret_cast<std::uintptr_t>(kAlign::down<16>(ptr)));
	IGROS_CHECK(!kAlign::check(ptr));
	IGROS_CHECK(kAlign::check(kAlign::up(ptr)));
	IGROS_CHECK(kAlign::check<0x1000>(reinterpret_cast<const void*>(0x5000)));
	IGROS_CHECK(!kAlign::check<0x1000>(reinterpret_cast<const void*>(0x5800)));

	return igros::test::result();

}

