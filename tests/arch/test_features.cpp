////////////////////////////////////////////////////////////////
//
//	CPU feature control tests (CPUID and MSR from fake hardware,
//	see tests/fakes/arch/x86_64)
//
//	File:	test_features.cpp
//	Date:	29 Oct 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


// C++
#include <cstdint>
// IgrOS-Kernel arch x86
#include <arch/x86/features.hpp>
// Tests
#include <tests/hw.hpp>


namespace hw = igros::test::hw;


#if	defined (IGROS_ARCH_x86_64)

// CPUID leaves and EFER
constexpr auto EXTENDED_MAX	{std::uint32_t {0x80000000}};
constexpr auto EXTENDED_INFO	{std::uint32_t {0x80000001}};
constexpr auto EFER		{std::uint32_t {0xC0000080}};
// CPUID.80000001h:EDX.NX, EFER.LME | EFER.LMA, EFER.NXE
constexpr auto EDX_NX		{std::uint32_t {1} << 20};
constexpr auto EFER_LONG_MODE	{std::uint64_t {0x0500}};
constexpr auto EFER_NXE		{std::uint64_t {0x0800}};


// No extended leaf 80000001h: NX can't be checked
static void testNoExtendedLeaf() {
	hw::reset();
	hw::cpuid[EXTENDED_MAX]	= {EXTENDED_MAX, 0, 0, 0};
	hw::cpuid[EXTENDED_INFO]	= {0, 0, 0, EDX_NX};	// not reachable: leaf above maximum
	hw::msr[EFER]		= EFER_LONG_MODE;
	IGROS_CHECK(!cpuEnableNoExecute());
	IGROS_CHECK(EFER_LONG_MODE == hw::msr[EFER]);
}


// Leaf present, NX not supported: EFER untouched
static void testNoNX() {
	hw::reset();
	hw::cpuid[EXTENDED_MAX]	= {EXTENDED_INFO, 0, 0, 0};
	hw::cpuid[EXTENDED_INFO]	= {0, 0, 0, ~EDX_NX};
	hw::msr[EFER]		= EFER_LONG_MODE;
	IGROS_CHECK(!cpuEnableNoExecute());
	IGROS_CHECK(EFER_LONG_MODE == hw::msr[EFER]);
}


// NX supported: EFER.NXE set, other bits kept
static void testNX() {
	hw::reset();
	hw::cpuid[EXTENDED_MAX]	= {0x80000008, 0, 0, 0};
	hw::cpuid[EXTENDED_INFO]	= {0, 0, 0, EDX_NX};
	hw::msr[EFER]		= EFER_LONG_MODE;
	IGROS_CHECK(cpuEnableNoExecute());
	IGROS_CHECK((EFER_LONG_MODE | EFER_NXE) == hw::msr[EFER]);
}

#endif


int main() {

#if	defined (IGROS_ARCH_i386)
	// No PAE page tables: never available
	hw::reset();
	IGROS_CHECK(!cpuEnableNoExecute());
#elif	defined (IGROS_ARCH_x86_64)
	testNoExtendedLeaf();
	testNoNX();
	testNX();
#endif

	return igros::test::result();

}
