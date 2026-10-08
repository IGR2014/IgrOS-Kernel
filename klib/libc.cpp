////////////////////////////////////////////////////////////////
//
//	Freestanding C runtime functions required by GCC/Clang
//
//	File:	libc.cpp
//	Date:	09 Oct 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


// C++
#include <cstddef>


// Compiler may turn copy/fill loops back into libc calls (infinite recursion)
#if	defined (__GNUC__) && !defined (__clang__)
	#define IGROS_NO_LIBCALLS	__attribute__((optimize("no-tree-loop-distribute-patterns")))
#else
	#define IGROS_NO_LIBCALLS
#endif


// Signatures must match C standard library (compiler emits calls to them)
extern "C" {


	// Fill memory with byte value
	IGROS_NO_LIBCALLS
	void* memset(void* dst, int val, std::size_t size) {
		auto d {static_cast<unsigned char*>(dst)};
		for (auto i {static_cast<std::size_t>(0)}; i < size; ++i) {
			d[i] = static_cast<unsigned char>(val);
		}
		return dst;
	}

	// Copy non-overlapping memory
	IGROS_NO_LIBCALLS
	void* memcpy(void* dst, const void* src, std::size_t size) {
		auto d {static_cast<unsigned char*>(dst)};
		auto s {static_cast<const unsigned char*>(src)};
		for (auto i {static_cast<std::size_t>(0)}; i < size; ++i) {
			d[i] = s[i];
		}
		return dst;
	}

	// Copy possibly overlapping memory
	IGROS_NO_LIBCALLS
	void* memmove(void* dst, const void* src, std::size_t size) {
		auto d {static_cast<unsigned char*>(dst)};
		auto s {static_cast<const unsigned char*>(src)};
		if (d < s) {
			for (auto i {static_cast<std::size_t>(0)}; i < size; ++i) {
				d[i] = s[i];
			}
		} else if (d > s) {
			for (auto i {size}; i > 0; --i) {
				d[i - 1] = s[i - 1];
			}
		}
		return dst;
	}

	// Compare memory
	IGROS_NO_LIBCALLS
	int memcmp(const void* lhs, const void* rhs, std::size_t size) {
		auto l {static_cast<const unsigned char*>(lhs)};
		auto r {static_cast<const unsigned char*>(rhs)};
		for (auto i {static_cast<std::size_t>(0)}; i < size; ++i) {
			if (l[i] != r[i]) {
				return (l[i] < r[i]) ? -1 : 1;
			}
		}
		return 0;
	}


}	// extern "C"
