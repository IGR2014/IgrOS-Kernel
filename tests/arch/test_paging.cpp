////////////////////////////////////////////////////////////////
//
//	Paging (page tables) tests
//
//	File:	test_paging.cpp
//	Date:	19 Oct 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


// C
#include <signal.h>
#include <sys/mman.h>
#include <unistd.h>
// C++
#include <cstring>
#include <vector>
// IgrOS-Kernel arch
#include <arch/x86/paging.hpp>
// Tests
#include <tests/hw.hpp>


using igros::x86::paging;
using igros::mem::phys;
namespace hw = igros::test::hw;


// Low physical memory (page tables live there) is a shared memory file mapped
// at kernel offset, so other virtual pages can alias it like with real MMU
#define IGROS_TEST_KERNEL_OFFSET	0x20000000
constexpr auto KERNEL_OFFSET	{std::uintptr_t {IGROS_TEST_KERNEL_OFFSET}};
constexpr auto LOW_MEMORY	{static_cast<std::size_t>(igros::mem::LOW_MEMORY_LIMIT)};

asm(
	".globl " IGROS_TEST_SYMBOL(KERNEL_OFFSET_VIRT) "; "
	".set " IGROS_TEST_SYMBOL(KERNEL_OFFSET_VIRT) ", " IGROS_TEST_STR(IGROS_TEST_KERNEL_OFFSET) "; "
	".globl " IGROS_TEST_SYMBOL(_SECTION_KERNEL_START_) "; "
	".set " IGROS_TEST_SYMBOL(_SECTION_KERNEL_START_) ", " IGROS_TEST_STR(IGROS_TEST_KERNEL_OFFSET) " + 0x100000; "
	".globl " IGROS_TEST_SYMBOL(_SECTION_TEXT_START_) "; "
	".set " IGROS_TEST_SYMBOL(_SECTION_TEXT_START_) ", " IGROS_TEST_STR(IGROS_TEST_KERNEL_OFFSET) " + 0x100000; "
	".globl " IGROS_TEST_SYMBOL(_SECTION_RODATA_START_) "; "
	".set " IGROS_TEST_SYMBOL(_SECTION_RODATA_START_) ", " IGROS_TEST_STR(IGROS_TEST_KERNEL_OFFSET) " + 0x103000; "
	".globl " IGROS_TEST_SYMBOL(_SECTION_DATA_START_) "; "
	".set " IGROS_TEST_SYMBOL(_SECTION_DATA_START_) ", " IGROS_TEST_STR(IGROS_TEST_KERNEL_OFFSET) " + 0x105000; "
	".globl " IGROS_TEST_SYMBOL(_KERNEL_STACK_GUARD_) "; "
	".set " IGROS_TEST_SYMBOL(_KERNEL_STACK_GUARD_) ", " IGROS_TEST_STR(IGROS_TEST_KERNEL_OFFSET) " + 0x106000; "
	".globl " IGROS_TEST_SYMBOL(_SECTION_KERNEL_END_) "; "
	".set " IGROS_TEST_SYMBOL(_SECTION_KERNEL_END_) ", " IGROS_TEST_STR(IGROS_TEST_KERNEL_OFFSET) " + 0x108000"
);

// Fake kernel image layout (physical): code, read-only data, data + bss
constexpr auto KERNEL_TEXT	{std::uintptr_t {0x100000}};
constexpr auto KERNEL_RODATA	{std::uintptr_t {0x103000}};
constexpr auto KERNEL_DATA	{std::uintptr_t {0x105000}};
constexpr auto KERNEL_END	{std::uintptr_t {0x108000}};
// Boot stack guard page (in bss)
constexpr auto KERNEL_GUARD	{std::uintptr_t {0x106000}};

// Entry bits
constexpr auto WRITABLE		{static_cast<paging::entry_t>(paging::FLAGS::WRITABLE)};
constexpr auto HUGE		{static_cast<paging::entry_t>(paging::FLAGS::HUGE)};
constexpr auto NO_EXECUTE	{static_cast<paging::entry_t>(paging::FLAGS::NO_EXECUTE)};
// CR0 write protection
constexpr auto CR0_WP		{std::uintptr_t {0x00010000}};


// Physical memory file
static auto memoryFile {-1};

// Software MMU: map faulting page to physical page from page tables
static void pageFault(const int signal, siginfo_t* const info, void* const) {
	const auto page		{std::bit_cast<std::uintptr_t>(info->si_addr) & ~(paging::PAGE_SIZE - 1)};
	const auto address	{paging::translate(static_cast<igros::mem::virt_t>(page))};
	if ((paging::INVALID == address) || (address >= igros::mem::LOW_MEMORY_LIMIT) ||
		(MAP_FAILED == ::mmap(std::bit_cast<void*>(page), paging::PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED, memoryFile, static_cast<off_t>(address)))) {
		::signal(signal, SIG_DFL);
	}
}

// Map physical memory, install software MMU
[[nodiscard]]
static auto setupMemory() -> bool {
	memoryFile = ::memfd_create("igros-phys", 0);
	if ((memoryFile < 0) || (0 != ::ftruncate(memoryFile, static_cast<off_t>(LOW_MEMORY)))) {
		return false;
	}
	const auto memory {::mmap(std::bit_cast<void*>(KERNEL_OFFSET), LOW_MEMORY, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED_NOREPLACE, memoryFile, 0)};
	if (std::bit_cast<void*>(KERNEL_OFFSET) != memory) {
		return false;
	}
	struct sigaction action {};
	action.sa_sigaction	= pageFault;
	action.sa_flags		= SA_SIGINFO;
	return 0 == ::sigaction(SIGSEGV, &action, nullptr);
}


// Page table at physical address
[[nodiscard]]
static auto table(const igros::mem::phys_t address) -> paging::entry_t* {
	return std::bit_cast<paging::entry_t*>(KERNEL_OFFSET + static_cast<std::uintptr_t>(address));
}

// Entry for virtual address at given level (tables must exist)
[[nodiscard]]
static auto entryAt(const std::uintptr_t virt, const igros::igros_usize_t level) -> paging::entry_t& {
	auto entries {table(paging::root())};
	for (auto current {paging::LEVELS - 1}; ; --current) {
		auto &entry {entries[(virt >> (paging::PAGE_SHIFT + paging::INDEX_BITS * current)) & (paging::ENTRIES - 1)]};
		if (current == level) {
			return entry;
		}
		entries = table(entry & paging::ADDRESS_MASK);
	}
}

// Allocate empty page table
[[nodiscard]]
static auto newTable() -> igros::mem::phys_t {
	const auto address {phys::alloc(igros::mem::LOW_MEMORY_LIMIT)};
	std::memset(table(address), 0, paging::PAGE_SIZE);
	return address;
}

// Map page with flags
template<class ...Flags>
[[nodiscard]]
static auto map(const igros::mem::phys_t address, const std::uintptr_t virt, Flags ...flags) -> bool {
	return paging::map(address, static_cast<igros::mem::virt_t>(virt), igros::klib::make_kflags<paging::FLAGS>(paging::FLAGS::PRESENT, flags...));
}

// Translate virtual address
[[nodiscard]]
static auto translate(const std::uintptr_t virt) -> igros::mem::phys_t {
	return paging::translate(static_cast<igros::mem::virt_t>(virt));
}


// Build kernel page tables (forget host mapping of self-test scratch page:
// software MMU keeps it after kernel unmaps it, repeated self-test would see stale page)
static void initPaging() {
	::munmap(paging::dynamicStart(), paging::PAGE_SIZE);
	paging::init();
}

// No physical memory for page tables
static void testInitNoMemory() {
	hw::reset();
	initPaging();
	IGROS_CHECK(hw::printed("Paging:\t\tFAILED (no memory for page tables)"));
	IGROS_CHECK(0 == hw::cr3);
}


// Kernel mapping: low memory at kernel offset and identity, verified by self-test
static void testInit() {
	hw::reset();
	hw::noExecute = true;
	initPaging();
	IGROS_CHECK(hw::printed("Paging:\t\tOK ("));
	IGROS_CHECK(0 != paging::root());
	IGROS_CHECK(0x1000 == translate(KERNEL_OFFSET + 0x1000));
	IGROS_CHECK(0x3FF123 == translate(KERNEL_OFFSET + 0x3FF123));
	IGROS_CHECK(0x2345 == translate(0x2345));
	IGROS_CHECK(paging::INVALID == translate(0));
	IGROS_CHECK(paging::INVALID == translate(KERNEL_OFFSET + LOW_MEMORY));
	// No RAM above low memory: direct map is low memory only
	IGROS_CHECK(igros::mem::LOW_MEMORY_LIMIT == paging::directMapEnd());
	// Kernel write protection on
	IGROS_CHECK(0 != (hw::cr0 & CR0_WP));
	// Code: read + execute
	for (auto page {KERNEL_TEXT}; page < KERNEL_RODATA; page += paging::PAGE_SIZE) {
		const auto entry {entryAt(KERNEL_OFFSET + page, 0)};
		IGROS_CHECK((0 == (entry & WRITABLE)) && (0 == (entry & NO_EXECUTE)));
	}
	// Read-only data: read
	for (auto page {KERNEL_RODATA}; page < KERNEL_DATA; page += paging::PAGE_SIZE) {
		const auto entry {entryAt(KERNEL_OFFSET + page, 0)};
		IGROS_CHECK((0 == (entry & WRITABLE)) && (NO_EXECUTE == (entry & NO_EXECUTE)));
	}
	// Boot stack guard page isn't mapped at kernel offset, its neighbours are
	IGROS_CHECK(paging::INVALID == translate(KERNEL_OFFSET + KERNEL_GUARD));
	IGROS_CHECK(KERNEL_GUARD - 1 == translate(KERNEL_OFFSET + KERNEL_GUARD - 1));
	IGROS_CHECK(KERNEL_GUARD + paging::PAGE_SIZE == translate(KERNEL_OFFSET + KERNEL_GUARD + paging::PAGE_SIZE));
	// Data, low memory around kernel and identity mapping: read + write, no execute
	for (const auto virt : {KERNEL_OFFSET + KERNEL_DATA, KERNEL_OFFSET + KERNEL_END - paging::PAGE_SIZE, KERNEL_OFFSET + KERNEL_END,
				KERNEL_OFFSET + 0xB8000, KERNEL_OFFSET + KERNEL_TEXT - paging::PAGE_SIZE, KERNEL_TEXT, std::uintptr_t {0x1000}}) {
		const auto entry {entryAt(virt, 0)};
		IGROS_CHECK((WRITABLE == (entry & WRITABLE)) && (NO_EXECUTE == (entry & NO_EXECUTE)));
	}
}

// No NX support: no NX bits (reserved bits would fault)
static void testInitNoExecuteUnsupported() {
	hw::reset();
	initPaging();
	IGROS_CHECK(hw::printed("Paging:\t\tOK ("));
	IGROS_CHECK(!hw::printed(", NX)"));
	IGROS_CHECK(0 == (entryAt(KERNEL_OFFSET + KERNEL_DATA, 0) & NO_EXECUTE));
	IGROS_CHECK(0 == (entryAt(KERNEL_OFFSET + KERNEL_RODATA, 0) & NO_EXECUTE));
	// Explicit NX flag is dropped too
	hw::cr3 = static_cast<std::uintptr_t>(newTable());
	IGROS_CHECK(map(0x7000, 0x00C00000, paging::FLAGS::NO_EXECUTE));
	IGROS_CHECK(0 == (entryAt(0x00C00000, 0) & NO_EXECUTE));
}

// RAM above low memory: mapped at kernel offset with large pages, page tables reachable there
static void testInitDirectMap() {
	hw::reset();
	hw::noExecute = true;
	// RAM up to 16 MB (only first 4 MB are backed by test memory: page tables come from there)
	phys::markFree(igros::mem::LOW_MEMORY_LIMIT, 0x01000000);
	initPaging();
	IGROS_CHECK(hw::printed("Paging:\t\tOK ("));
	IGROS_CHECK(hw::printed("direct map 16 MB"));
	IGROS_CHECK(0x01000000 == paging::directMapEnd());
	IGROS_CHECK(0x00A01234 == translate(KERNEL_OFFSET + 0x00A01234));
	IGROS_CHECK(0x00FFFFFF == translate(KERNEL_OFFSET + 0x00FFFFFF));
	IGROS_CHECK(paging::INVALID == translate(KERNEL_OFFSET + 0x01000000));
	// Large pages: read + write, no execute
	const auto large {entryAt(KERNEL_OFFSET + 0x00800000, 1)};
	IGROS_CHECK((HUGE == (large & HUGE)) && (WRITABLE == (large & WRITABLE)) && (NO_EXECUTE == (large & NO_EXECUTE)));
	// Identity mapping covers low memory only
	IGROS_CHECK(paging::INVALID == translate(0x00800000));
	// Dynamic mappings start right above direct map limit
	IGROS_CHECK(KERNEL_OFFSET + paging::DIRECT_MAP_LIMIT == std::bit_cast<std::uintptr_t>(paging::dynamicStart()));
}


// Map, translate and unmap single page
static void testMap() {
	hw::reset();
	hw::cr3 = static_cast<std::uintptr_t>(newTable());
	constexpr auto VIRT {std::uintptr_t {0x00C00000}};
	IGROS_CHECK(paging::INVALID == translate(VIRT));
	IGROS_CHECK(map(0x7000, VIRT, paging::FLAGS::WRITABLE));
	IGROS_CHECK(0x7123 == translate(VIRT + 0x123));
	IGROS_CHECK((1 == hw::invalidated.size()) && (std::bit_cast<const void*>(VIRT) == hw::invalidated[0]));
	const auto entry {entryAt(VIRT, 0)};
	IGROS_CHECK(0x7000 == (entry & paging::ADDRESS_MASK));
	IGROS_CHECK(0 != (entry & static_cast<paging::entry_t>(paging::FLAGS::WRITABLE)));
	// Remap replaces entry
	IGROS_CHECK(map(0x8000, VIRT));
	IGROS_CHECK(0x8000 == translate(VIRT));
	IGROS_CHECK(0 == (entryAt(VIRT, 0) & static_cast<paging::entry_t>(paging::FLAGS::WRITABLE)));
	// Unmap
	paging::unmap(static_cast<igros::mem::virt_t>(VIRT));
	IGROS_CHECK(paging::INVALID == translate(VIRT));
	IGROS_CHECK(3 == hw::invalidated.size());
	// Unmapping missing page (no tables) does nothing
	paging::unmap(static_cast<igros::mem::virt_t>(VIRT + 0x10000000));
	IGROS_CHECK(3 == hw::invalidated.size());
}


// Invalid physical/virtual addresses
static void testMapInvalid() {
	hw::reset();
	hw::cr3 = static_cast<std::uintptr_t>(newTable());
	IGROS_CHECK(!map(0x7001, 0x00C00000));
	IGROS_CHECK(!map(0x7000, 0x00C00800));
	IGROS_CHECK(!map(static_cast<igros::mem::phys_t>(paging::ADDRESS_MASK) + paging::PAGE_SIZE, 0x00C00000));
	IGROS_CHECK(hw::invalidated.empty());
	// Root table outside low memory can't be accessed
	hw::cr3 = static_cast<std::uintptr_t>(igros::mem::LOW_MEMORY_LIMIT);
	IGROS_CHECK(!map(0x7000, 0x00C00000));
	IGROS_CHECK(paging::INVALID == translate(0x00C00000));
}


// User pages: intermediate entries get user access too
static void testUser() {
	hw::reset();
	hw::cr3 = static_cast<std::uintptr_t>(newTable());
	constexpr auto VIRT	{std::uintptr_t {0x00C00000}};
	constexpr auto USER	{static_cast<paging::entry_t>(paging::FLAGS::USER)};
	IGROS_CHECK(map(0x7000, VIRT, paging::FLAGS::WRITABLE));
	IGROS_CHECK(0 == (entryAt(VIRT, paging::LEVELS - 1) & USER));
	IGROS_CHECK(map(0x8000, VIRT + paging::PAGE_SIZE, paging::FLAGS::USER));
	for (auto level {igros::igros_usize_t {0}}; level < paging::LEVELS; ++level) {
		IGROS_CHECK(0 != (entryAt(VIRT + paging::PAGE_SIZE, level) & USER));
	}
	IGROS_CHECK(0 == (entryAt(VIRT, 0) & USER));
}


// Huge pages: translated with large page offset, can't be split by map
static void testHuge() {
	hw::reset();
	hw::cr3 = static_cast<std::uintptr_t>(newTable());
	constexpr auto VIRT	{std::uintptr_t {0x00C00000}};
	const auto hugeSize	{paging::PAGE_SIZE << paging::INDEX_BITS};
	IGROS_CHECK(map(0x7000, VIRT));
	// Neighbour large page in same directory
	const auto hugeVirt	{(VIRT + hugeSize) & ~(hugeSize - 1)};
	const auto hugePhys	{static_cast<paging::entry_t>(hugeSize) * 3};
	entryAt(hugeVirt, 1)	= hugePhys | static_cast<paging::entry_t>(paging::FLAGS::HUGE) | static_cast<paging::entry_t>(paging::FLAGS::PRESENT);
	IGROS_CHECK(hugePhys + 0x12345 == translate(hugeVirt + 0x12345));
	IGROS_CHECK(!map(0x9000, hugeVirt + paging::PAGE_SIZE));
	paging::unmap(static_cast<igros::mem::virt_t>(hugeVirt));
	IGROS_CHECK(hugePhys == translate(hugeVirt));
}


// No memory for intermediate tables
static void testNoMemory() {
	hw::reset();
	hw::cr3 = static_cast<std::uintptr_t>(newTable());
	auto pages {std::vector<igros::mem::phys_t> {}};
	for (auto page {phys::alloc(igros::mem::LOW_MEMORY_LIMIT)}; 0 != page; page = phys::alloc(igros::mem::LOW_MEMORY_LIMIT)) {
		pages.push_back(page);
	}
	IGROS_CHECK(!map(0x7000, 0x00C00000));
	for (const auto page : pages) {
		phys::free(page);
	}
}


// Page fault handler reports reason and halts
static void testFaultHandler() {
	auto regs {igros::x86::register_t {}};
	regs.number	= 14;
	hw::reset();
	hw::cr2		= 0xDEAD000;
	regs.param	= 0x02;
	IGROS_CHECK_HALTS(paging::exHandler(&regs));
	IGROS_CHECK(hw::printed("Name:\t\tPAGE FAULT"));
	IGROS_CHECK(hw::printed("Address:\t0x000000000dead000"));
	IGROS_CHECK(hw::printed("Reason:\t\tmissing page, write access, kernel mode\n"));
	hw::reset();
	regs.param	= 0x1D;
	IGROS_CHECK_HALTS(paging::exHandler(&regs));
	IGROS_CHECK(hw::printed("Reason:\t\tprotected page, read access, user mode, reserved bit set, instruction fetch"));
}


int main() {
	if (!setupMemory()) {
		std::printf("can't map physical memory at 0x%lx\n", static_cast<unsigned long>(KERNEL_OFFSET));
		return igros::test::SKIPPED;
	}
	testInitNoMemory();
	phys::markFree(paging::PAGE_SIZE, igros::mem::LOW_MEMORY_LIMIT);
	phys::markUsed(KERNEL_TEXT, KERNEL_END);
	testInit();
	testInitNoExecuteUnsupported();
	testMap();
	testMapInvalid();
	testUser();
	testHuge();
	testNoMemory();
	testFaultHandler();
	testInitDirectMap();
	return igros::test::result();
}

