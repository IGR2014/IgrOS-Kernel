////////////////////////////////////////////////////////////////
//
//	Integer literals tests (values checked at compile time)
//
//	File:	test_literals.cpp
//	Date:	30 Oct 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


// C++
#include <cstdint>
#include <limits>
#include <type_traits>
// IgrOS-Kernel arch
#include <arch/types.hpp>
// Tests
#include <tests/test.hpp>


using namespace igros;


// Out of range literal must not compile (built by literals_* tests, see CMakeLists.txt)
#if	defined (IGROS_TEST_LITERAL)
[[maybe_unused]] constexpr auto OUT_OF_RANGE {IGROS_TEST_LITERAL};
#endif


// Literal types
static_assert(std::is_same_v<igros_byte_t,	decltype(0_u8)>);
static_assert(std::is_same_v<igros_word_t,	decltype(0_u16)>);
static_assert(std::is_same_v<igros_dword_t,	decltype(0_u32)>);
static_assert(std::is_same_v<igros_quad_t,	decltype(0_u64)>);
static_assert(std::is_same_v<igros_sbyte_t,	decltype(0_i8)>);
static_assert(std::is_same_v<igros_sword_t,	decltype(0_i16)>);
static_assert(std::is_same_v<igros_sdword_t,	decltype(0_i32)>);
static_assert(std::is_same_v<igros_squad_t,	decltype(0_i64)>);
static_assert(std::is_same_v<igros_usize_t,	decltype(0_usize)>);
static_assert(std::is_same_v<igros_ssize_t,	decltype(0_ssize)>);

// Bases and digit separators
static_assert(0_u32 == 0u);
static_assert(42_u32 == 42u);
static_assert(052_u32 == 42u);
static_assert(0x2A_u32 == 42u);
static_assert(0X2a_u32 == 42u);
static_assert(0b101010_u32 == 42u);
static_assert(0B101010_u32 == 42u);
static_assert(1'000'000_u32 == 1000000u);
static_assert(0xDEAD'BEEF_u32 == 0xDEADBEEFu);

// Type limits are accepted
static_assert(255_u8 == std::numeric_limits<std::uint8_t>::max());
static_assert(0xFFFF_u16 == std::numeric_limits<std::uint16_t>::max());
static_assert(0xFFFFFFFF_u32 == std::numeric_limits<std::uint32_t>::max());
static_assert(0xFFFFFFFFFFFFFFFF_u64 == std::numeric_limits<std::uint64_t>::max());
static_assert(18446744073709551615_u64 == std::numeric_limits<std::uint64_t>::max());
static_assert(127_i8 == std::numeric_limits<std::int8_t>::max());
static_assert(32767_i16 == std::numeric_limits<std::int16_t>::max());
static_assert(0x7FFFFFFF_i32 == std::numeric_limits<std::int32_t>::max());
static_assert(0x7FFFFFFFFFFFFFFF_i64 == std::numeric_limits<std::int64_t>::max());
// Unary minus applies to literal value
static_assert(-127_i8 == -127);


int main() {
	// Everything is checked at compile time
	return igros::test::result();
}
