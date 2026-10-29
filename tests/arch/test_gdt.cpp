////////////////////////////////////////////////////////////////
//
//	Global descriptor table and task state segment tests
//
//	File:	test_gdt.cpp
//	Date:	29 Oct 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


// C++
#include <bit>
#include <cstdint>
// IgrOS-Kernel arch
#include <arch/x86/native.hpp>
#if	defined (IGROS_ARCH_i386)
#include <arch/i386/gdt.hpp>
#include <arch/i386/tss.hpp>
#elif	defined (IGROS_ARCH_x86_64)
#include <arch/x86_64/gdt.hpp>
#include <arch/x86_64/tss.hpp>
#endif
// Tests
#include <tests/hw.hpp>


using namespace igros;
using gdt = x86::native::gdt;
using tss = x86::native::tss;
namespace hw = igros::test::hw;


// GDT handed to CPU by lgdt
[[nodiscard]]
static auto loadedGdt() -> const gdt::pointer_t& {
	return *static_cast<const gdt::pointer_t*>(hw::gdtPointer);
}

// 32-bit base of descriptor
[[nodiscard]]
static auto base32(const gdt::entry_t &entry) -> std::uint32_t {
	return static_cast<std::uint32_t>(entry.baseLow) | (static_cast<std::uint32_t>(entry.baseMid) << 16) | (static_cast<std::uint32_t>(entry.baseHigh) << 24);
}

// 20-bit limit of descriptor
[[nodiscard]]
static auto limit20(const gdt::entry_t &entry) -> std::uint32_t {
	return static_cast<std::uint32_t>(entry.limitLow) | (static_cast<std::uint32_t>(entry.limitFlags & 0x0F) << 16);
}

// Flat 4 GB segment with given access byte and flags nibble
static void checkFlat(const gdt::entry_t &entry, const std::uint8_t access, const std::uint8_t flags) {
	IGROS_CHECK(0 == base32(entry));
	IGROS_CHECK(0xFFFFF == limit20(entry));
	IGROS_CHECK(access == entry.access);
	IGROS_CHECK(flags == (entry.limitFlags >> 4));
}


// Table layout: null, kernel/user code/data, task state segment(s), task register
static void testInit() {
	hw::reset();
	hw::cr3 = 0x00123000;
	gdt::init();

	IGROS_CHECK(nullptr != hw::gdtPointer);
	const auto &pointer {loadedGdt()};
	IGROS_CHECK(7 * sizeof(gdt::entry_t) - 1 == pointer.size);
	const auto* const table {pointer.pointer};

	// Null descriptor
	IGROS_CHECK(0 == base32(table[0]));
	IGROS_CHECK(0 == limit20(table[0]));
	IGROS_CHECK(0 == table[0].access);

#if	defined (IGROS_ARCH_i386)
	// 4 KB granularity, 32-bit segments
	constexpr auto FLAGS {std::uint8_t {0x0C}};
#elif	defined (IGROS_ARCH_x86_64)
	// 4 KB granularity, long mode segments
	constexpr auto FLAGS {std::uint8_t {0x0A}};
#endif
	// Present, ring 0/3, code (execute/read) and data (read/write)
	checkFlat(table[gdt::KERNEL_CODE / sizeof(gdt::entry_t)], 0x9A, FLAGS);
	checkFlat(table[gdt::KERNEL_DATA / sizeof(gdt::entry_t)], 0x92, FLAGS);
	checkFlat(table[3], 0xFA, FLAGS);
	checkFlat(table[4], 0xF2, FLAGS);

	// Available TSS descriptor: present, ring 0, type 9, byte granular
	const auto &tssEntry {table[gdt::TSS / sizeof(gdt::entry_t)]};
	IGROS_CHECK(0x89 == tssEntry.access);
	IGROS_CHECK(0 == (tssEntry.limitFlags >> 4));
	IGROS_CHECK(sizeof(tss::segment_t) - 1 == limit20(tssEntry));

#if	defined (IGROS_ARCH_i386)
	IGROS_CHECK(std::bit_cast<std::uintptr_t>(&tss::kernel()) == base32(tssEntry));
	// Double fault task descriptor
	const auto &doubleFaultEntry {table[gdt::TSS_DOUBLE_FAULT / sizeof(gdt::entry_t)]};
	IGROS_CHECK(0x89 == doubleFaultEntry.access);
	IGROS_CHECK(sizeof(tss::segment_t) - 1 == limit20(doubleFaultEntry));
	IGROS_CHECK(std::bit_cast<std::uintptr_t>(&tss::doubleFault()) == base32(doubleFaultEntry));
#elif	defined (IGROS_ARCH_x86_64)
	// 16-byte descriptor: upper entry holds base bits 32..63
	const auto &upper {table[gdt::TSS / sizeof(gdt::entry_t) + 1]};
	const auto base {
		static_cast<std::uint64_t>(base32(tssEntry))		|
		(static_cast<std::uint64_t>(upper.limitLow) << 32)	|
		(static_cast<std::uint64_t>(upper.baseLow) << 48)
	};
	IGROS_CHECK(std::bit_cast<std::uintptr_t>(&tss::init()) == base);
	IGROS_CHECK(0 == upper.baseMid);
	IGROS_CHECK(0 == upper.access);
	IGROS_CHECK(0 == upper.limitFlags);
	IGROS_CHECK(0 == upper.baseHigh);
#endif

	// Task register holds kernel task
	IGROS_CHECK(gdt::TSS == hw::taskRegister);
}


#if	defined (IGROS_ARCH_i386)

// Task state segments: kernel task and double fault task
static void testTaskStateSegments() {
	hw::reset();
	hw::cr3 = 0x00456000;
	tss::init(gdt::KERNEL_CODE, gdt::KERNEL_DATA);

	// Kernel task: ring 0 stack segment only
	const auto &kernel {tss::kernel()};
	IGROS_CHECK(gdt::KERNEL_DATA == kernel.ss0);
	IGROS_CHECK(0 == kernel.cr3);
	IGROS_CHECK(0 == kernel.eip);
	IGROS_CHECK(sizeof(tss::segment_t) == kernel.iomapBase);

	// Double fault task: current page directory, own aligned stack, interrupts disabled
	const auto &task {tss::doubleFault()};
	IGROS_CHECK(0x00456000 == task.cr3);
	IGROS_CHECK(0x00000002 == task.eflags);
	IGROS_CHECK(0 != task.esp);
	IGROS_CHECK(0 == (task.esp % 16));
	IGROS_CHECK(0 != task.eip);
	IGROS_CHECK(gdt::KERNEL_CODE == task.cs);
	IGROS_CHECK(gdt::KERNEL_DATA == task.ss);
	IGROS_CHECK(gdt::KERNEL_DATA == task.ds);
	IGROS_CHECK(gdt::KERNEL_DATA == task.es);
	IGROS_CHECK(gdt::KERNEL_DATA == task.fs);
	IGROS_CHECK(gdt::KERNEL_DATA == task.gs);
	IGROS_CHECK(sizeof(tss::segment_t) == task.iomapBase);
}


// Double fault task entry reports interrupted state (saved in kernel task) and halts
static void testDoubleFaultTask() {
	hw::reset();
	hw::cr2 = 0xDEADB000;
	tss::init(gdt::KERNEL_CODE, gdt::KERNEL_DATA);
	auto &kernel {tss::kernel()};
	kernel.eip	= 0xC0101234;
	kernel.esp	= 0xC0200FF0;
	kernel.ebp	= 0xC0200FF8;

	const auto task {std::bit_cast<void (*)()>(static_cast<std::uintptr_t>(tss::doubleFault().eip))};
	IGROS_CHECK_HALTS(task());
	IGROS_CHECK(hw::printed("DOUBLE FAULT"));
	IGROS_CHECK(hw::printed("0xc0101234"));
	IGROS_CHECK(hw::printed("0xc0200ff0"));
	IGROS_CHECK(hw::printed("0xc0200ff8"));
	IGROS_CHECK(hw::printed("0xdeadb000"));
}

#elif	defined (IGROS_ARCH_x86_64)

// Task state segment: three distinct aligned interrupt stacks, no ring stacks, no I/O bitmap
static void testTaskStateSegment() {
	const auto &segment {tss::init()};
	for (auto index {0U}; index < 3U; ++index) {
		IGROS_CHECK(0 != segment.ist[index]);
		IGROS_CHECK(0 == (segment.ist[index] % 16));
	}
	IGROS_CHECK(segment.ist[0] != segment.ist[1]);
	IGROS_CHECK(segment.ist[1] != segment.ist[2]);
	IGROS_CHECK(segment.ist[0] != segment.ist[2]);
	// Stacks don't overlap
	const auto distance = [](const std::uint64_t a, const std::uint64_t b) {
		return (a > b) ? (a - b) : (b - a);
	};
	IGROS_CHECK(distance(segment.ist[0], segment.ist[1]) >= tss::STACK_SIZE);
	IGROS_CHECK(distance(segment.ist[1], segment.ist[2]) >= tss::STACK_SIZE);
	for (auto index {3U}; index < 7U; ++index) {
		IGROS_CHECK(0 == segment.ist[index]);
	}
	for (const auto rsp : segment.rsp) {
		IGROS_CHECK(0 == rsp);
	}
	IGROS_CHECK(sizeof(tss::segment_t) == segment.iomapBase);
	// Repeated init keeps the same segment and stacks
	const auto first {segment.ist[0]};
	IGROS_CHECK(&segment == &tss::init());
	IGROS_CHECK(first == tss::init().ist[0]);
}

#endif


int main() {

	testInit();
#if	defined (IGROS_ARCH_i386)
	testTaskStateSegments();
	testDoubleFaultTask();
#elif	defined (IGROS_ARCH_x86_64)
	testTaskStateSegment();
#endif

	return igros::test::result();

}
