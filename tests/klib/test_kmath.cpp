////////////////////////////////////////////////////////////////
//
//	kdivmod tests (i386 build tests 64-bit software division)
//
//	File:	test_kmath.cpp
//	Date:	11 Oct 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


// C++
#include <cstdint>
// IgrOS-Kernel library
#include <klib/kMath.hpp>
// Tests
#include <tests/test.hpp>


// Compare kdivmod with native division
template<class A, class B>
static void checkDivision(const A dividend, const B divisor) {
	const auto result	{igros::klib::kdivmod(dividend, divisor)};
	const auto quotient	{static_cast<A>(dividend / divisor)};
	const auto remainder	{static_cast<A>(dividend % divisor)};
	if ((quotient != result.quotient) || (remainder != result.remainder)) {
		std::printf("%lld / %lld: got %lld, %lld (expected %lld, %lld)\n",
			static_cast<long long>(dividend), static_cast<long long>(divisor),
			static_cast<long long>(result.quotient), static_cast<long long>(result.remainder),
			static_cast<long long>(quotient), static_cast<long long>(remainder));
	}
	IGROS_CHECK((quotient == result.quotient) && (remainder == result.remainder));
}


int main() {

	// Unsigned 64-bit by 32-bit and 64-bit divisors
	const std::uint64_t dividends[] {
		0, 1, 7, 10, 255, 1193181, 0xFFFFFFFFull, 0x100000000ull,
		12345678901ull, 0x8000000000000000ull, 0xFFFFFFFFFFFFFFFFull
	};
	const std::uint32_t divisors[] {1, 2, 3, 8, 10, 16, 1193181, 0x7FFFFFFF, 0x80000000u, 0xFFFFFFFFu};
	for (const auto a : dividends) {
		for (const auto b : divisors) {
			checkDivision<std::uint64_t, std::uint32_t>(a, b);
			checkDivision<std::uint64_t, std::uint64_t>(a, b);
		}
		checkDivision<std::uint64_t, std::uint64_t>(a, 0x100000001ull);
		checkDivision<std::uint64_t, std::uint64_t>(a, 0xFFFFFFFFFFFFFFFFull);
	}

	// Signed 64-bit (truncation towards zero)
	const std::int64_t signedDividends[] {
		0, 1, -1, 7, -7, 1000000007, -1000000007, 12345678901, -12345678901, INT64_MAX, INT64_MIN + 1
	};
	const std::int32_t signedDivisors[] {1, -1, 3, -3, 10, -10, INT32_MAX, INT32_MIN};
	for (const auto a : signedDividends) {
		for (const auto b : signedDivisors) {
			checkDivision<std::int64_t, std::int32_t>(a, b);
			checkDivision<std::int64_t, std::int64_t>(a, b);
		}
		checkDivision<std::int64_t, std::int64_t>(a, 0x100000001ll);
		checkDivision<std::int64_t, std::int64_t>(a, -0x100000001ll);
	}

	// Any integer sizes (64-bit dividend with narrow divisor uses software division on i386)
	checkDivision<std::uint64_t, std::uint16_t>(0xFFFFFFFFFFFFFFFFull, 0xFFFF);
	checkDivision<std::uint64_t, std::uint8_t>(12345678901ull, 7);
	checkDivision<std::int64_t, std::int16_t>(-12345678901ll, -300);
	checkDivision<std::uint32_t, std::uint64_t>(0xFFFFFFFFu, 0x100000000ull);
	checkDivision<std::int32_t, std::int64_t>(-7, 2);
	checkDivision<std::uint8_t, std::uint8_t>(200, 7);

	// Software 64-bit division on every architecture (hardware 64/32 path at run time on i386)
	using igros::klib::kudivmod64;
	for (const auto a : dividends) {
		for (const auto b : divisors) {
			IGROS_CHECK((kudivmod64(a, b) == igros::klib::divmod_t<std::uint64_t> {a / b, a % b}));
		}
		IGROS_CHECK((kudivmod64(a, 0x100000001ull) == igros::klib::divmod_t<std::uint64_t> {a / 0x100000001ull, a % 0x100000001ull}));
	}

	// Compile time (shift-subtract path)
	static_assert(kudivmod64(0xFFFFFFFFFFFFFFFFull, 10) == igros::klib::divmod_t<std::uint64_t> {1844674407370955161ull, 5});
	static_assert(kudivmod64(12345678901ull, 0x100000001ull) == igros::klib::divmod_t<std::uint64_t> {2, 3755744307ull});
	static_assert(kudivmod64(7, 8) == igros::klib::divmod_t<std::uint64_t> {0, 7});
	static_assert(igros::klib::kdivmod(std::int64_t {-12345678901}, std::int64_t {1000}) == igros::klib::divmod_t<std::int64_t> {-12345678, -901});

	// Mixed signedness is rejected
	static_assert(igros::klib::kdivisible<std::uint64_t, std::uint32_t>);
	static_assert(!igros::klib::kdivisible<std::int64_t, std::uint32_t>);
	static_assert(!igros::klib::kdivisible<std::uint32_t, int>);

#if	defined (IGROS_ARCH_i386)
	// Software division by zero returns dividend as remainder (native one traps)
	const auto byZero {igros::klib::kdivmod(std::uint64_t {42}, std::uint64_t {0})};
	IGROS_CHECK((0 == byZero.quotient) && (42 == byZero.remainder));
#endif

	return igros::test::result();

}

