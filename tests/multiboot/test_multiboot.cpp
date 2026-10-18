////////////////////////////////////////////////////////////////
//
//	Multiboot information output tests
//
//	File:	test_multiboot.cpp
//	Date:	19 Oct 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


// C
#include <sys/mman.h>
// C++
#include <cstring>
// IgrOS-Kernel multiboot
#include <multiboot/multiboot.hpp>
// IgrOS-Kernel platform
#include <platform/platform.hpp>
// Tests
#include <tests/hw.hpp>


using igros::multiboot::info_t;
namespace hw = igros::test::hw;


// Kernel image symbols: 8 KB kernel at 1 MB, no kernel offset
asm(
	".globl " IGROS_TEST_SYMBOL(KERNEL_OFFSET_VIRT) "; "
	".set " IGROS_TEST_SYMBOL(KERNEL_OFFSET_VIRT) ", 0; "
	".globl " IGROS_TEST_SYMBOL(_SECTION_KERNEL_START_) "; "
	".set " IGROS_TEST_SYMBOL(_SECTION_KERNEL_START_) ", 0x100000; "
	".globl " IGROS_TEST_SYMBOL(_SECTION_KERNEL_END_) "; "
	".set " IGROS_TEST_SYMBOL(_SECTION_KERNEL_END_) ", 0x102000"
);


// Current platform (normally from platform-x86.cpp)
auto igros::platform::Platform::current() noexcept -> const Platform& {
	constinit static const Platform platform {"test", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
	return platform;
}


// Multiboot data must be reachable through 32-bit multiboot fields
template<class T>
[[nodiscard]]
static auto address32(const T* const object) noexcept -> std::uint32_t {
	return static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(object));
}

// Multiboot info with given flags
[[nodiscard]]
static auto makeInfo(const std::uint32_t flags) -> info_t& {
	alignas(8) static std::uint8_t raw[sizeof(info_t)] {};
	std::memset(raw, 0, sizeof(raw));
	std::memcpy(raw, &flags, sizeof(flags));
	hw::reset();
	return *reinterpret_cast<info_t*>(raw);
}


// Magic check halts on bad magic only
static void testMagic() {
	auto &info {makeInfo(0)};
	info.test(igros::multiboot::BOOTLOADER_MAGIC);
	IGROS_CHECK(hw::console.empty());
	IGROS_CHECK_HALTS(info.test(0x12345678));
	IGROS_CHECK(hw::printed("BAD MULTIBOOT MAGIC!!!"));
	IGROS_CHECK(hw::printed("Magic:\t\t0x12345678"));
}


// Nothing provided
static void testEmpty() {
	const auto &info {makeInfo(0)};
	info.printMemInfo();
	info.printMemMap();
	info.printVBEInfo();
	info.printFBInfo();
	IGROS_CHECK(hw::printed("No memory info provided..."));
	IGROS_CHECK(hw::printed("No memory map provided..."));
	IGROS_CHECK(hw::printed("No VBE info provided..."));
	IGROS_CHECK(hw::printed("No FrameBuffer info provided..."));
}


// Flags table
static void testFlags() {
	const auto &info {makeInfo((1u << 0) | (1u << 6) | (1u << 12))};
	info.printFlags();
	IGROS_CHECK(hw::printed("Flags:\t\t\t0x00001041"));
	IGROS_CHECK(hw::printed("BIOS memory map:\t[ Y ]"));
	IGROS_CHECK(hw::printed("Boot device:\t\t[ N ]"));
	IGROS_CHECK(hw::printed("Memory map:\t\t[ Y ]"));
	IGROS_CHECK(hw::printed("VBE:\t\t\t[ N ]"));
	IGROS_CHECK(hw::printed("FB:\t\t\t[ Y ]"));
}


// Memory info and memory map entries
static void testMemory() {
	#pragma pack(push, 1)
	struct rawEntry {
		std::uint32_t	size;
		std::uint64_t	address;
		std::uint64_t	length;
		std::uint32_t	type;
	};
	#pragma pack(pop)
	static const rawEntry map[] {
		{20, 0x0000000000000000ull, 0x000000000009FC00ull, 1},
		{20, 0x0000000000100000ull, 0x0000000007EE0000ull, 2},
	};
	auto &info {makeInfo((1u << 0) | (1u << 6))};
	info.memLow	= 639;
	info.memHigh	= 130048;
	info.mmapAddr	= address32(map);
	info.mmapLength	= sizeof(map);
	info.printMemInfo();
	info.printMemMap();
	IGROS_CHECK(hw::printed("Low:\t639 Kb."));
	IGROS_CHECK(hw::printed("High:\t130048 Kb."));
	IGROS_CHECK(hw::printed("Size:\t48 bytes"));
	IGROS_CHECK(hw::printed("[1] 0x0000000000000000 - 0x000000000009fc00"));
	IGROS_CHECK(hw::printed("[2] 0x0000000000100000 - 0x0000000007fe0000"));
}


// VBE info: strings are real mode segment:offset pointers (below 1 MB)
static void testVBE() {
	static igros::multiboot::vbeConfig	config {};
	static igros::multiboot::vbeMode	mode {};
	std::memcpy(config.signature, "VESA", 4);
	config.version		= 0x0300;
	config.memory		= 256;
	mode.width		= 1024;
	mode.height		= 768;
	mode.bpp		= 32;
	mode.physbase		= 0xFD000000;
	auto &info {makeInfo(1u << 11)};
	info.vbeControlInfo	= address32(&config);
	info.vbeModeInfo	= address32(&mode);
	info.vbeModeCurrent	= 0x118;
	// No strings
	info.printVBEInfo();
	IGROS_CHECK(hw::printed("Signature:\tVESA"));
	IGROS_CHECK(hw::printed("Version:\t3.0"));
	IGROS_CHECK(hw::printed("OEM:\t\t\"<none>\""));
	IGROS_CHECK(hw::printed("Current mode:\t#280 (1024x768, 32bpp, 0xfd000000)"));
	IGROS_CHECK(hw::printed("Video memory:\t16384 Kb."));
	// Strings in low memory page at 0x9000:0x1000 (0x91000)
	auto* const low {static_cast<char*>(::mmap(reinterpret_cast<void*>(0x91000), 0x1000, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED_NOREPLACE, -1, 0))};
	if (reinterpret_cast<void*>(0x91000) != low) {
		std::printf("can't map low memory, VBE strings not tested\n");
		return;
	}
	std::strcpy(low, "OEM");
	std::strcpy(low + 0x10, "Vendor");
	std::strcpy(low + 0x20, "Card");
	std::strcpy(low + 0x30, "Rev");
	config.oem		= 0x90001000;
	config.vendor		= 0x90001010;
	config.productName	= 0x90001020;
	config.productRev	= 0x90001030;
	hw::console.clear();
	info.printVBEInfo();
	IGROS_CHECK(hw::printed("OEM:\t\t\"OEM\""));
	IGROS_CHECK(hw::printed("Vendor name:\t\"Vendor\""));
	IGROS_CHECK(hw::printed("Card name:\t\"Card\""));
	IGROS_CHECK(hw::printed("Card rev.:\t\"Rev\""));
	::munmap(low, 0x1000);
}


// Framebuffer types
static void testFrameBuffer() {
	auto &info {makeInfo(1u << 12)};
	info.fbAddress	= 0xFD000000;
	info.fbPitch	= 4096;
	info.fbWidth	= 1024;
	info.fbHeight	= 768;
	info.fbBpp	= 32;
	info.fbType	= 1;
	info.printFBInfo();
	IGROS_CHECK(hw::printed("Current mode:\t(1024x768, 32bpp, 4096, RGB)"));
	IGROS_CHECK(hw::printed("Address:\t0x00000000fd000000"));
	IGROS_CHECK(hw::printed("Size:\t\t3145728 bytes"));
	hw::console.clear();
	info.fbType	= 0;
	info.printFBInfo();
	IGROS_CHECK(hw::printed(", Indexed)"));
	hw::console.clear();
	info.fbType	= 2;
	info.printFBInfo();
	IGROS_CHECK(hw::printed(", Text)"));
}


// Kernel header: platform, image, version, loader and command line
static void testHeader() {
	static const char loader[]	{"GRUB 2.12"};
	static const char cmdline[]	{"/boot/kernel.bin debug"};
	auto &info {makeInfo((1u << 2) | (1u << 9))};
	info.cmdLine		= address32(cmdline);
	info.bootloaderName	= address32(loader);
	info.printHeader();
	IGROS_CHECK(hw::printed("Arch:\t\ttest"));
	IGROS_CHECK(hw::printed("Size:\t\t8 Kb."));
	IGROS_CHECK(hw::printed("Version:\tv0.0.0"));
	IGROS_CHECK(hw::printed("Loader:\t\t\"GRUB 2.12\""));
	IGROS_CHECK(hw::printed("Command line:\t\"/boot/kernel.bin debug\""));
	// Not provided
	makeInfo(0).printHeader();
	IGROS_CHECK(hw::printed("Loader:\t\t\"\""));
}


int main() {
	testMagic();
	testEmpty();
	testFlags();
	testMemory();
	testVBE();
	testFrameBuffer();
	testHeader();
	return igros::test::result();
}

