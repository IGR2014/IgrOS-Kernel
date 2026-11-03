////////////////////////////////////////////////////////////////
//
//	kFlags tests
//
//	File:	test_kflags.cpp
//	Date:	03 Nov 2026
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


// Anonymous namespace
namespace {

	// Test flags
	enum class flags_t : std::uint32_t {
		NONE = 0x00,
		A    = 0x01,
		B    = 0x02,
		C    = 0x04,
		AB   = 0x03,
	};

} // namespace


auto main() -> int {

	using igros::klib::make_kflags;

	const auto flags {make_kflags<flags_t>(flags_t::A, flags_t::C)};
	IGROS_CHECK(0x05 == flags.value());

	// Single bit flags
	IGROS_CHECK(flags.isSet(flags_t::A));
	IGROS_CHECK(!flags.isSet(flags_t::B));
	IGROS_CHECK(flags.isSet(flags_t::C));

	// Multi-bit flag is set only when all its bits are set
	IGROS_CHECK(!flags.isSet(flags_t::AB));
	IGROS_CHECK(make_kflags<flags_t>(flags_t::A, flags_t::B).isSet(flags_t::AB));

	// Empty flag is never set
	IGROS_CHECK(!flags.isSet(flags_t::NONE));
	IGROS_CHECK(0 == igros::klib::kFlags<flags_t> {}.value());

	// From raw value (unknown bits kept)
	const auto raw {igros::klib::kFlags<flags_t>::from(0x83)};
	IGROS_CHECK(0x83 == raw.value());
	IGROS_CHECK(raw.isSet(flags_t::AB));
	IGROS_CHECK(!raw.isSet(flags_t::C));
	static_assert(igros::klib::kFlags<flags_t>::from(0x04).isSet(flags_t::C));

	// Set flag (other bits kept, original unchanged)
	const auto set {raw.set(flags_t::C)};
	IGROS_CHECK(0x87 == set.value());
	IGROS_CHECK(0x83 == raw.value());
	IGROS_CHECK(raw.set(flags_t::A).value() == raw.value());
	static_assert(make_kflags<flags_t>(flags_t::A).set(flags_t::B).isSet(flags_t::AB));

	// Compile time
	static_assert(make_kflags<flags_t>(flags_t::B).isSet(flags_t::B));
	static_assert(!make_kflags<flags_t>(flags_t::B).isSet(flags_t::AB));

	return igros::test::result();
}

