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
#include <klib/kmath.hpp>
// Tests
#include <tests/test.hpp>


// Compare kdivmod with native division
template<class A, class B>
static void checkDivision(const A dividend, const B divisor) {
	const auto result	{igros::klib::kdivmod(dividend, divisor)};
	const auto quotient	{static_cast<A>(dividend / divisor)};
	const auto remainder	{static_cast<A>(dividend % divisor)};
	if ((quotient != result.quotient) || (remainder != result.reminder)) {
		std::printf("%lld / %lld: got %lld, %lld (expected %lld, %lld)\n",
			static_cast<long long>(dividend), static_cast<long long>(divisor),
			static_cast<long long>(result.quotient), static_cast<long long>(result.reminder),
			static_cast<long long>(quotient), static_cast<long long>(remainder));
	}
	IGROS_CHECK((quotient == result.quotient) && (remainder == result.reminder));
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

#if	defined (IGROS_ARCH_i386)
	// Software division by zero returns dividend as remainder (native one traps)
	const auto byZero {igros::klib::kdivmod(std::uint64_t {42}, std::uint64_t {0})};
	IGROS_CHECK((0 == byZero.quotient) && (42 == byZero.reminder));
#endif

	return igros::test::result();

}

