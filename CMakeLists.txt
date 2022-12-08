# Cmake version
cmake_minimum_required(VERSION 3.21.0)

# Build in Source protection
file(
	TO_CMAKE_PATH
	"${PROJECT_BINARY_DIR}/CMakeLists.txt"
	LOC_PATH
)
# Check existance
if(EXISTS "${LOC_PATH}")
	# Error message
	message(
		FATAL_ERROR
		"Cannot build in a source directory (or any with CMakeLists.txt)!"
	)
endif()

# GWE C++ standard
set(
	CMAKE_CXX_STANDARD
	20
)
# Required C++20
set(
	CMAKE_CXX_STANDARD_REQUIRED
	True
)
# No GNU extensions!
set(
	CMAKE_CXX_EXTENSIONS
	OFF
)
# PIC
set(
	CMAKE_PLATFORM_INDEPENDENT_CODE
	ON
)
# Compile comands export
set(
	CMAKE_EXPORT_COMPILE_COMMANDS
	ON
)
# Install prefix
set(
	CMAKE_INSTALL_PREFIX
	"${CMAKE_SOURCE_DIR}/install/${CMAKE_BUILD_TYPE}/${CMAKE_SYSTEM_PROCESSOR}"
)
# CMake Module path
set(
	CMAKE_MODULE_PATH
	"${CMAKE_SOURCE_DIR}/cmake/modules"
)

# Get version from GIT
include(VersionGIT)
# Enable CCache
include(EnableCCache)
# Enable Doxygen
include(EnableDoxygen)

# Preset parameters
set(
	IGROS_KERNEL
	"kernel.bin"
)
set(
	IGROS_LINK_SCRIPT
	"${CMAKE_SOURCE_DIR}/config/link/link-${CMAKE_SYSTEM_PROCESSOR}.ld"
)

# Project
project(
	IgrOS-Kernel
	VERSION ${IGROS_GIT_VERSION}
	DESCRIPTION "Simple C++20 Kernel for i386/x86_64"
	LANGUAGES ASM-ATT CXX
)

# Enable CTest
enable_testing()

# Colored output option
option(
	IGROS_CXX_COLORED_OUTPUT
	"Produce ANSI-colored output from compiler (GCC/Clang can do it)"
	OFF
)
# ClangTidy pass
option(
	IGROS_CXX_CLANG_TIDY
	"Add ClangTidy pass to build process"
	OFF
)

# Use Clang-Tidy ?
if (IGROS_CXX_CLANG_TIDY)
	# Clang-Tidy extra args
	if(("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC") OR (("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") AND ("${CMAKE_CXX_SIMULATE_ID}" STREQUAL "MSVC")))
		# Set extra Clang-Tidy checks
		set(
			IGROS_CLANG_TIDY_ARGS
			"--extra-arg=/EHsc"
			"--extra-arg=/DNOMINMAX"
		)
	endif()
	# Set default Clang-Tidy checks
	set(
		CMAKE_CXX_CLANG_TIDY
		"clang-tidy"
		"-use-color"
		"-header-filter=.*"
		${IGROS_CLANG_TIDY_ARGS}
	)
endif()

# LTO
set(
	CMAKE_INTERPROCEDURAL_OPTIMIZATION
	"$<BOOL:$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>>>"
)

# Header
message(
	"\n"
	"===============================\n"
	"\n"
	"IgrOS Kernel v${IGROS_GIT_VERSION}\n"
	"\n"
	"Architecture:	${CMAKE_SYSTEM_PROCESSOR}\n"
	"\n"
	"Compiler:	${CMAKE_CXX_COMPILER}\n"
	"\n"
	"Assembler:	${CMAKE_ASM_COMPILER}\n"
	"\n"
	"Linker:		${CMAKE_LINKER}\n"
	"\n"
	"Script:		${CMAKE_TOOLCHAIN_FILE}\n"
	"\n"
	"Binary:		${IGROS_KERNEL}\n"
	"\n"
	"===============================\n"
)

# Message
message(STATUS "Building Kernel ${CMAKE_SYSTEM_PROCESSOR}")

# Kernel C++ files
file(
	GLOB
	KERNEL_HEADERS
	*.hpp
)
# Kernel C++ files
file(
	GLOB
	KERNEL_SOURCES
	*.cpp
)

# Includes
include_directories(
	include
)

# Kernel executable
add_executable(
	${IGROS_KERNEL}
	${KERNEL_HEADERS}
	${KERNEL_SOURCES}
)

# Kernel definitions
add_compile_definitions(
	IGROS_KERNEL
	IGROS_ARCH_${CMAKE_SYSTEM_PROCESSOR}
	IGROS_ARCH=${CMAKE_SYSTEM_PROCESSOR}
	IGROS_GIT_VERSION_MAJOR=${IGROS_GIT_VERSION_MAJOR}
	IGROS_GIT_VERSION_MINOR=${IGROS_GIT_VERSION_MINOR}
	IGROS_GIT_VERSION_PATCH=${IGROS_GIT_VERSION_PATCH}
	IGROS_GIT_VERSION_BUILD=${IGROS_GIT_COMMIT}
	IGROS_GIT_VERSION="${IGROS_GIT_VERSION}"
	IGROS_GIT_HASH="${IGROS_GIT_HASH}"
)

# Add arch subdirectory
add_subdirectory(
	arch
)
# Add multiboot subdirectory
add_subdirectory(
	multiboot
)
# Add klib subdirectory
add_subdirectory(
	klib
)
# Add mem subdirectory
add_subdirectory(
	mem
)
# Add sys subdirectory
add_subdirectory(
	sys
)
# Add drivers subdirectory
add_subdirectory(
	drivers
)

# Install rules
install(
	TARGETS ${IGROS_KERNEL}
	CONFIGURATIONS ${CMAKE_BUILD_TYPE}
	DESTINATION ${CMAKE_INSTALL_PREFIX}
)

# Link command
set(
	CMAKE_CXX_LINK_EXECUTABLE
	"${CMAKE_LINKER} <OBJECTS> -o ${IGROS_KERNEL} ${CMAKE_LINKER_FLAGS}"
)

# Install proxy
add_custom_target(
	install_proxy
	COMMAND ${CMAKE_COMMAND} --build "${CMAKE_BINARY_DIR}" --target install
	DEPENDS ${IGROS_KERNEL}
)

# Qemu for specified arch
set(
	QEMU
	"qemu-system-${CMAKE_SYSTEM_PROCESSOR}"
)

# Test kernel
add_custom_target(
	test_proxy
	USES_TERMINAL
	COMMAND ${QEMU} -kernel "${CMAKE_SOURCE_DIR}/install/${CMAKE_BUILD_TYPE}/${CMAKE_SYSTEM_PROCESSOR}/${IGROS_KERNEL}" -m 2G -vga std -rtc base=localtime -serial mon:stdio
	DEPENDS install_proxy
)

