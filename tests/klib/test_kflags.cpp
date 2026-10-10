////////////////////////////////////////////////////////////////
//
//	kFlags tests
//
//	File:	test_kflags.cpp
//	Date:	11 Oct 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


// C++
#include <cstdint>
// IgrOS-Kernel library
#include <klib/kFlags.hpp>
// Tests
#include <tests/test.hpp>


// Test flags
enum class flags_t : std::uint32_t {
	NONE	= 0x00,
	A	= 0x01,
	B	= 0x02,
	C	= 0x04,
	AB	= 0x03
};


int main() {

	using igros::klib::make_kflags;

	const auto flags {make_kflags<flags_t>(flags_t::A, flags_t::C)};
	IGROS_CHECK(0x05 == flags.value());

	// Single bit flags
	IGROS_CHECK(flags.isSet(flags_t::A));
	IGROS_CHECK(!flags.isSet(flags_t::B));
	IGROS_CHECK(flags.isSet(flags_t::C));

	// Multi-bit flag is set only when all its bits are set
	IGROS_CHECK(!flags.isSet(flags_t::AB));
	IGROS_CHECK((flags | flags_t::B).isSet(flags_t::AB));

	// Empty flag is never set
	IGROS_CHECK(!flags.isSet(flags_t::NONE));

	// Bits
	IGROS_CHECK(flags.test(0));
	IGROS_CHECK(!flags.test(1));
	IGROS_CHECK(flags.test(2));
	IGROS_CHECK(!flags.test(31));

	// Compile time
	static_assert(make_kflags<flags_t>(flags_t::B).isSet(flags_t::B));
	static_assert(!make_kflags<flags_t>(flags_t::B).isSet(flags_t::AB));

	return igros::test::result();

}

