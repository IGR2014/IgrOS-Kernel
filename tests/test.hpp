////////////////////////////////////////////////////////////////
//
//	Minimal host unit test helpers
//
//	File:	test.hpp
//	Date:	11 Oct 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


#pragma once


// C++
#include <cstdio>


// Test helpers namespace
namespace igros::test {


	// Number of failed checks
	inline auto failures {0};

	// Exit code for skipped test (CTest SKIP_RETURN_CODE)
	constexpr auto SKIPPED {77};

	// Test exit code (prints summary)
	[[nodiscard]]
	inline auto result() noexcept -> int {
		if (0 != failures) {
			std::printf("%d check(s) FAILED\n", failures);
			return 1;
		}
		std::printf("ALL OK\n");
		return 0;
	}


}	// namespace igros::test


// Check condition, report failure location and continue
#define IGROS_CHECK(condition)										\
	do {												\
		if (!(condition)) {									\
			++igros::test::failures;							\
			std::printf("%s:%d: check failed: %s\n", __FILE__, __LINE__, #condition);	\
		}											\
	} while (false)

