////////////////////////////////////////////////////////////////
//
//	Physical page allocator and multiboot memory map tests
//
//	File:	test_phys.cpp
//	Date:	11 Oct 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


// C++
#include <cstdint>
#include <cstring>
// IgrOS-Kernel memory
#include <mem/phys.hpp>
// IgrOS-Kernel platform
#include <platform/platform.hpp>
// Tests
#include <tests/test.hpp>


// Linker script symbols at one address: kernel end == kernel offset, so the
// kernel image reservation is empty and only allocator logic is tested
#define IGROS_STR2(x)	#x
#define IGROS_STR(x)	IGROS_STR2(x)
#define IGROS_SYMBOL(name)	IGROS_STR(__USER_LABEL_PREFIX__) #name

asm(
	".data; "
	".globl " IGROS_SYMBOL(KERNEL_OFFSET_VIRT) "; " IGROS_SYMBOL(KERNEL_OFFSET_VIRT) ":; "
	".globl " IGROS_SYMBOL(_SECTION_KERNEL_START_) "; " IGROS_SYMBOL(_SECTION_KERNEL_START_) ":; "
	".globl " IGROS_SYMBOL(_SECTION_KERNEL_END_) "; " IGROS_SYMBOL(_SECTION_KERNEL_END_) ":; "
	".byte 0; "
	".text"
);


using igros::mem::phys;


// Raw multiboot memory map entry
#pragma pack(push, 1)
struct rawEntry {
	std::uint32_t	size;
	std::uint64_t	address;
	std::uint64_t	length;
	std::uint32_t	type;
};
#pragma pack(pop)


// Multiboot data must be reachable through 32-bit multiboot fields
template<class T>
[[nodiscard]]
static auto address32(const T* const object) noexcept -> std::uint32_t {
	return static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(object));
}


// Range marking, allocation and freeing
static void testRanges() {
	phys::markFree(0x1000, 0x5000);
	IGROS_CHECK(4 == phys::freePages());
	// No new whole page inside range
	phys::markFree(0x1800, 0x2800);
	IGROS_CHECK(4 == phys::freePages());
	// Every touched page becomes used
	phys::markUsed(0x2800, 0x3001);
	IGROS_CHECK(2 == phys::freePages());
	IGROS_CHECK(0x1000 == phys::alloc());
	IGROS_CHECK(0x4000 == phys::alloc());
	// Out of memory
	IGROS_CHECK(0 == phys::alloc());
	phys::free(0x4000);
	IGROS_CHECK(1 == phys::freePages());
	// Double, unaligned and null frees are ignored
	phys::free(0x4000);
	phys::free(0x4001);
	phys::free(0);
	IGROS_CHECK(1 == phys::freePages());
	// Limit
	IGROS_CHECK(0 == phys::alloc(0x4000));
	IGROS_CHECK(0x4000 == phys::alloc(0x5000));
}


// Initialization from multiboot memory map (QEMU-like 128 MB layout)
static auto testInit() -> bool {

	static rawEntry map[] {
		{20, 0x0000000000000000ull, 0x000000000009FC00ull, 1},	// Low memory (always reserved)
		{20, 0x000000000009FC00ull, 0x0000000000000400ull, 2},	// EBDA
		{20, 0x0000000000100000ull, 0x0000000007EE0000ull, 1},	// 1 MB .. 127.9 MB
		{20, 0x00000000FFFC0000ull, 0x0000000000040000ull, 2},	// BIOS ROM
		{20, 0x0000000100000000ull, 0x0000000040000000ull, 1},	// Above 4 GB - not tracked
	};
	alignas(8) static std::uint8_t infoRaw[sizeof(igros::multiboot::info_t)] {};

	if ((reinterpret_cast<std::uintptr_t>(map) > 0xFFFFFFFFu) || (reinterpret_cast<std::uintptr_t>(infoRaw) > 0xFFFFFFFFu)) {
		std::printf("multiboot test data above 4 GB (position independent executable?)\n");
		return false;
	}

	auto &info {*reinterpret_cast<igros::multiboot::info_t*>(infoRaw)};
	// Memory info + memory map flags
	const std::uint32_t flags {(1u << 0) | (1u << 6)};
	std::memcpy(infoRaw, &flags, sizeof(flags));
	info.mmapAddr	= address32(map);
	info.mmapLength	= sizeof(map);

	// Memory map view visits every entry
	auto entries	{0};
	auto last	{std::uint64_t {0}};
	for (const auto &entry : info.memoryMap()) {
		++entries;
		last = entry.address;
	}
	IGROS_CHECK(5 == entries);
	IGROS_CHECK(0x100000000ull == last);

	phys::init(info);

	// Available 1 MB .. 0x7FE0000 minus pages holding test multiboot data
	const auto available	{std::size_t {0x7EE0000 >> 12}};
	auto reserved		{std::size_t {0}};
	for (const auto address : {reinterpret_cast<std::uintptr_t>(map), reinterpret_cast<std::uintptr_t>(infoRaw)}) {
		reserved += ((address >= 0x100000) && (address < 0x7FE0000)) ? 1 : 0;
	}
	IGROS_CHECK((phys::freePages() <= available) && (phys::freePages() + 2 >= available - reserved));

	// Low memory is never given out
	IGROS_CHECK(0 == phys::alloc(0x100000));
	// Allocate everything: aligned, ascending, inside available range
	auto count	{std::size_t {0}};
	auto previous	{std::uint64_t {0}};
	auto valid	{true};
	for (auto page {phys::alloc()}; 0 != page; page = phys::alloc()) {
		valid = valid && (page > previous) && (page >= 0x100000) && (page < 0x7FE0000) && (0 == (page & 0xFFF));
		previous = page;
		++count;
	}
	IGROS_CHECK(valid);
	IGROS_CHECK(0 == phys::freePages());
	IGROS_CHECK(count + 2 >= available - reserved);

	// Entry with zero size must not hang memory map iteration
	static rawEntry broken[] {
		{0,  0x100000, 0x1000, 1},
		{20, 0x200000, 0x1000, 1}
	};
	info.mmapAddr	= address32(broken);
	info.mmapLength	= sizeof(broken);
	entries = 0;
	for ([[maybe_unused]] const auto &entry : info.memoryMap()) {
		if (++entries > 10) {
			break;
		}
	}
	IGROS_CHECK(2 == entries);

	return true;

}


int main() {

	// Kernel image reservation must be empty for these tests
	if (0 != (reinterpret_cast<std::uintptr_t>(&_SECTION_KERNEL_END_) - igros::platform::Platform::kernelOffset())) {
		std::printf("linker symbols are not at one address\n");
		return igros::test::SKIPPED;
	}

	testRanges();
	if (!testInit()) {
		return igros::test::SKIPPED;
	}

	return igros::test::result();

}

