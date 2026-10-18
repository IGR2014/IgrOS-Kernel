////////////////////////////////////////////////////////////////
//
//	Platform description tests
//
//	File:	test_platform.cpp
//	Date:	19 Oct 2026
//
//	Copyright (c) 2017 - 2026, Igor Baklykov
//	All rights reserved.
//
//


// C++
#include <cstring>
#include <string>
// IgrOS-Kernel platform
#include <platform/platform.hpp>
// Tests
#include <tests/hw.hpp>


using igros::platform::Platform;


// Kernel image symbols: 12 KB kernel at 1 MB above 1 GB kernel offset
asm(
	".globl " IGROS_TEST_SYMBOL(KERNEL_OFFSET_VIRT) "; "
	".set " IGROS_TEST_SYMBOL(KERNEL_OFFSET_VIRT) ", 0x40000000; "
	".globl " IGROS_TEST_SYMBOL(_SECTION_KERNEL_START_) "; "
	".set " IGROS_TEST_SYMBOL(_SECTION_KERNEL_START_) ", 0x40100000; "
	".globl " IGROS_TEST_SYMBOL(_SECTION_KERNEL_END_) "; "
	".set " IGROS_TEST_SYMBOL(_SECTION_KERNEL_END_) ", 0x40103000"
);


// Called platform hooks
static auto calls {std::string {}};


int main() {

	// Kernel image
	IGROS_CHECK(0x40000000 == Platform::kernelOffset());
	IGROS_CHECK(0x40100000 == reinterpret_cast<std::uintptr_t>(Platform::kernelStart()));
	IGROS_CHECK(0x40103000 == reinterpret_cast<std::uintptr_t>(Platform::kernelEnd()));
	IGROS_CHECK(0x3000 == Platform::kernelSize());

	// Architecture
#if	defined (IGROS_ARCH_i386)
	IGROS_CHECK(Platform::isI386() && !Platform::isX86_64());
#else
	IGROS_CHECK(!Platform::isI386() && Platform::isX86_64());
#endif
	IGROS_CHECK(!Platform::isARM32() && !Platform::isARM64() && !Platform::isAVR());

	// Hooks are called in order
	const Platform platform {
		"hooks",
		[] { calls += 'i'; },
		[] { calls += 'f'; },
		[] { calls += 's'; },
		[] { calls += 'r'; },
		[] { calls += 'u'; },
		[] { calls += 'w'; }
	};
	IGROS_CHECK(0 == std::strcmp("hooks", platform.name()));
	platform.initialize();
	platform.suspend();
	platform.wakeup();
	platform.finalize();
	platform.reboot();
	platform.shutdown();
	IGROS_CHECK("iuwfrs" == calls);

	// Missing hooks are skipped
	constexpr Platform empty {};
	IGROS_CHECK(nullptr == empty.name());
	empty.initialize();
	empty.suspend();
	empty.wakeup();
	empty.finalize();
	empty.reboot();
	empty.shutdown();
	IGROS_CHECK("iuwfrs" == calls);

	return igros::test::result();

}

