////////////////////////////////////////////////////////////////
///
///	@file
///	@brief		Freestanding C runtime functions required by GCC/Clang
///	@date		30 Oct 2026
///
///	@copyright	Copyright (c) 2017 - 2026, Igor Baklykov
///			All rights reserved.
///	@author		Igor Baklykov
///


// C++
#include <bit>
#include <cstddef>


// Compiler may turn copy/fill loops back into libc calls (infinite recursion)
#if	defined (__GNUC__) && !defined (__clang__)
	#define IGROS_NO_LIBCALLS	__attribute__((optimize("no-tree-loop-distribute-patterns")))
#else
	#define IGROS_NO_LIBCALLS
#endif


// Signatures must match C standard library (compiler emits calls to them)
extern "C" {


	// Declarations (<cstring> isn't freestanding)
	auto	memset(void* dst, int val, std::size_t size) -> void*;
	auto	memcpy(void* dst, const void* src, std::size_t size) -> void*;
	auto	memmove(void* dst, const void* src, std::size_t size) -> void*;
	auto	memcmp(const void* lhs, const void* rhs, std::size_t size) -> int;
	auto	memchr(const void* src, int val, std::size_t size) -> void*;
	auto	strlen(const char* src) -> std::size_t;	// flawfinder: ignore (implements strlen)


	/// @brief Fill memory with byte value
	/// @param dst Destination memory
	/// @param val Byte value (converted to unsigned char)
	/// @param size Number of bytes
	/// @return dst
	IGROS_NO_LIBCALLS
	auto memset(void* dst, int val, std::size_t size) -> void* {
		auto* d {static_cast<unsigned char*>(dst)};
		for (auto i {static_cast<std::size_t>(0)}; i < size; ++i) {
			d[i] = static_cast<unsigned char>(val);
		}
		return dst;
	}

	/// @brief Copy non-overlapping memory
	/// @param dst Destination memory
	/// @param src Source memory (must not overlap destination)
	/// @param size Number of bytes
	/// @return dst
	IGROS_NO_LIBCALLS
	auto memcpy(void* dst, const void* src, std::size_t size) -> void* {
		auto* d {static_cast<unsigned char*>(dst)};
		const auto* s {static_cast<const unsigned char*>(src)};
		for (auto i {static_cast<std::size_t>(0)}; i < size; ++i) {
			d[i] = s[i];
		}
		return dst;
	}

	/// @brief Copy possibly overlapping memory
	/// @param dst Destination memory
	/// @param src Source memory (may overlap destination)
	/// @param size Number of bytes
	/// @return dst
	IGROS_NO_LIBCALLS
	auto memmove(void* dst, const void* src, std::size_t size) -> void* {
		auto* d {static_cast<unsigned char*>(dst)};
		if (const auto* s {static_cast<const unsigned char*>(src)}; d < s) {
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

	/// @brief Compare memory
	/// @param lhs First memory block
	/// @param rhs Second memory block
	/// @param size Number of bytes
	/// @return Negative, zero or positive: first differing byte (unsigned) of lhs is less, equal or greater
	IGROS_NO_LIBCALLS
	auto memcmp(const void* lhs, const void* rhs, std::size_t size) -> int {
		const auto* l {static_cast<const unsigned char*>(lhs)};
		const auto* r {static_cast<const unsigned char*>(rhs)};
		for (auto i {static_cast<std::size_t>(0)}; i < size; ++i) {
			if (l[i] != r[i]) {
				return (l[i] < r[i]) ? -1 : 1;
			}
		}
		return 0;
	}

	/// @brief Find byte value in memory
	/// @param src Searched memory
	/// @param val Byte value (converted to unsigned char)
	/// @param size Number of bytes
	/// @return Pointer to first matching byte, nullptr if not found
	IGROS_NO_LIBCALLS
	auto memchr(const void* src, int val, std::size_t size) -> void* {
		const auto* s {static_cast<const unsigned char*>(src)};
		for (auto i {static_cast<std::size_t>(0)}; i < size; ++i) {
			if (static_cast<unsigned char>(val) == s[i]) {
				// C signature returns non-const pointer to const input (writing through it
				// stays caller's responsibility, as in libc)
				return std::bit_cast<void*>(s + i);
			}
		}
		return nullptr;
	}

	/// @brief String length (std::string_view length of C string)
	/// @param src Null-terminated string
	/// @return Number of characters before terminator
	IGROS_NO_LIBCALLS
	auto strlen(const char* src) -> std::size_t {	// flawfinder: ignore (implements strlen)
		auto size {static_cast<std::size_t>(0)};
		while ('\0' != src[size]) {
			++size;
		}
		return size;
	}


}	// extern "C"
