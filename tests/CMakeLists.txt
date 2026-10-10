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
)

# Tests
enable_testing()

# Add test: igros_test(<name> <test source> [kernel sources under test...])
function(igros_test NAME SOURCE)
	list(
		TRANSFORM ARGN
		PREPEND "${IGROS_SOURCE_DIR}/"
	)
	add_executable(
		test_${NAME}
		${SOURCE}
		stubs.cpp
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

igros_test(kprint	klib/test_kprint.cpp	klib/kprint.cpp klib/kmemory.cpp)
igros_test(kstring	klib/test_kstring.cpp)
igros_test(kmemory	klib/test_kmemory.cpp	klib/kmemory.cpp)
igros_test(kmath	klib/test_kmath.cpp)
igros_test(phys		mem/test_phys.cpp	mem/phys.cpp)

