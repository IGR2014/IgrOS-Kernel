# Host unit tests (separate project: kernel itself is built with a cross toolchain)
#
# Usage:
#	cmake -S tests -B build/tests [-DIGROS_TEST_ARCH=i386|x86_64] [-DIGROS_TESTS_COVERAGE=ON]
#	cmake --build build/tests
#	ctest --test-dir build/tests --output-on-failure

# Minimum CMake version
cmake_minimum_required(VERSION 3.21.0)

# Tests project
project(
	"IgrOS-Kernel-tests"
	LANGUAGES CXX
)

# C++20 without extensions (as kernel)
set(
	CMAKE_CXX_STANDARD
	20
)
set(
	CMAKE_CXX_STANDARD_REQUIRED
	True
)
set(
	CMAKE_CXX_EXTENSIONS
	OFF
)

# Kernel architecture code paths under test (i386 needs 32-bit host code)
if(NOT IGROS_TEST_ARCH)
	if(CMAKE_SIZEOF_VOID_P EQUAL 4)
		set(
			IGROS_TEST_ARCH
			i386
		)
	else()
		set(
			IGROS_TEST_ARCH
			x86_64
		)
	endif()
endif()
if(NOT IGROS_TEST_ARCH MATCHES "^(i386|x86_64)$")
	message(
		FATAL_ERROR
		"Unsupported IGROS_TEST_ARCH: ${IGROS_TEST_ARCH}"
	)
endif()

# Coverage of code under test
option(
	IGROS_TESTS_COVERAGE
	"Build tests with coverage instrumentation (GCC/Clang)"
	OFF
)

# Kernel sources root
set(
	IGROS_SOURCE_DIR
	"${CMAKE_CURRENT_SOURCE_DIR}/.."
)

message(
	STATUS
	"IgrOS tests: ${IGROS_TEST_ARCH} (${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION})"
)

# Compiler options
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	add_compile_options(
		-Wall
		-Wextra
		-pedantic
		-Werror
		# Tests deliberately pass unusual arguments to printf-like functions
		-Wno-format
	)
	# 32-bit code paths on 64-bit host
	if((IGROS_TEST_ARCH STREQUAL "i386") AND (CMAKE_SIZEOF_VOID_P EQUAL 8))
		add_compile_options(
			-m32
		)
		add_link_options(
			-m32
		)
	endif()
	# Fixed (low) addresses: multiboot structures hold 32-bit addresses
	if(NOT WIN32)
		add_compile_options(
			-fno-pie
		)
		add_link_options(
			-no-pie
		)
	endif()
	# Coverage
	if(IGROS_TESTS_COVERAGE)
		add_compile_options(
			--coverage
			-O0
		)
		add_link_options(
			--coverage
		)
	endif()
endif()

# Kernel headers and architecture
include_directories(
	"${IGROS_SOURCE_DIR}"
)
add_compile_definitions(
	IGROS_ARCH_${IGROS_TEST_ARCH}
	IGROS_ARCH=${IGROS_TEST_ARCH}
	# MinGW: standard printf (%zu etc.)
	__USE_MINGW_ANSI_STDIO=1
	# Kernel version (from git in kernel build)
	IGROS_GIT_VERSION_MAJOR=0
	IGROS_GIT_VERSION_MINOR=0
	IGROS_GIT_VERSION_PATCH=0
	IGROS_GIT_VERSION_BUILD=0
	IGROS_GIT_VERSION="0.0.0"
)

# Tests
enable_testing()

# Add test: igros_test(<name> <test source> [kernel sources under test...])
# Assembly functions are always stubbed (stubs/cpu.cpp), console drivers - unless under test
function(igros_test NAME SOURCE)
	set(
		STUBS
		stubs/cpu.cpp
	)
	if(NOT "drivers/vga/vmem.cpp" IN_LIST ARGN)
		list(
			APPEND STUBS
			stubs/vmem.cpp
		)
	endif()
	if(NOT "drivers/uart/serial.cpp" IN_LIST ARGN)
		list(
			APPEND STUBS
			stubs/serial.cpp
		)
	endif()
	list(
		TRANSFORM ARGN
		PREPEND "${IGROS_SOURCE_DIR}/"
	)
	add_executable(
		test_${NAME}
		${SOURCE}
		${STUBS}
		${ARGN}
	)
	add_test(
		NAME ${NAME}
		COMMAND test_${NAME}
	)
	set_tests_properties(
		${NAME}
		PROPERTIES
		SKIP_RETURN_CODE 77
	)
endfunction()

# Kernel library
igros_test(kprint	klib/test_kprint.cpp	klib/kPrint.cpp)
igros_test(kstring	klib/test_kstring.cpp)
igros_test(kmemory	klib/test_kmemory.cpp)
igros_test(kmath	klib/test_kmath.cpp)
igros_test(kflags	klib/test_kflags.cpp)
igros_test(kalign	klib/test_kalign.cpp)
igros_test(libc		klib/test_libc.cpp	klib/libc.cpp)
# Calls to C library functions must reach kernel implementations
target_compile_options(
	test_libc
	PRIVATE
	-fno-builtin
)

# Memory
igros_test(phys		mem/test_phys.cpp	mem/phys.cpp)

# Architecture
igros_test(irq		arch/test_irq.cpp	arch/x86/irq.cpp)
igros_test(isr		arch/test_isr.cpp	arch/x86/isr.cpp arch/x86/irq.cpp arch/x86/exceptions.cpp klib/kPrint.cpp)
# Descriptor tables (lgdt/lidt/ltr and interrupt entry points stubbed)
igros_test(gdt		arch/test_gdt.cpp	arch/${IGROS_TEST_ARCH}/gdt.cpp arch/${IGROS_TEST_ARCH}/tss.cpp klib/kPrint.cpp)
igros_test(idt		arch/test_idt.cpp	arch/x86/idt.cpp)
foreach(TEST gdt idt)
	target_sources(
		test_${TEST}
		PRIVATE
		stubs/descriptors.cpp
	)
endforeach()
# CPU features: CPUID/MSR headers shadowed by fakes (searched before kernel headers)
igros_test(features	arch/test_features.cpp	arch/x86/features.cpp)
target_include_directories(
	test_features
	BEFORE
	PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/fakes"
)
# Paging test emulates MMU with Linux shared memory and signals
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	igros_test(paging	arch/test_paging.cpp	arch/x86/paging.cpp mem/phys.cpp arch/x86/isr.cpp arch/x86/irq.cpp klib/kPrint.cpp)
endif()

# Drivers
igros_test(driver	drivers/test_driver.cpp	klib/kPrint.cpp)
igros_test(vmem		drivers/test_vmem.cpp	drivers/vga/vmem.cpp)
igros_test(serial	drivers/test_serial.cpp	drivers/uart/serial.cpp arch/x86/isr.cpp arch/x86/irq.cpp klib/kPrint.cpp)
igros_test(keyboard	drivers/test_keyboard.cpp	drivers/input/keyboard.cpp arch/x86/isr.cpp arch/x86/irq.cpp klib/kPrint.cpp)
igros_test(pit		drivers/test_pit.cpp	drivers/clock/pit.cpp arch/x86/isr.cpp arch/x86/irq.cpp klib/kPrint.cpp)
igros_test(rtc		drivers/test_rtc.cpp	drivers/clock/rtc.cpp klib/kPrint.cpp)

# Multiboot and platform
igros_test(multiboot	multiboot/test_multiboot.cpp	multiboot/multiboot.cpp klib/kPrint.cpp)
igros_test(platform	platform/test_platform.cpp)
