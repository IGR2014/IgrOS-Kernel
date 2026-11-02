////////////////////////////////////////////////////////////////
//
//	Kernel string functions tests
//
//	File:	test_kstring.cpp
//	Date:	11 Oct 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


// IgrOS-Kernel library
#include <klib/kString.hpp>
// Tests
#include <tests/test.hpp>


using namespace igros::klib;


int main() {

	// View (also at compile time), null pointer is empty string
	static_assert("abcd" == kstrview("abcd"));
	IGROS_CHECK(kstrview("").empty());
	IGROS_CHECK(3 == kstrview("abc").size());
	IGROS_CHECK(kstrview(nullptr).empty());
	const char* const text {"hello"};
	IGROS_CHECK(text == kstrview(text).data());

	return igros::test::result();

}

