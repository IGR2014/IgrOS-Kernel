# Cmake version
cmake_minimum_required(VERSION 3.20.0)

# Arch
if(NOT DEFINED IGROS_ARCH)
	set(IGROS_ARCH	"i386")
endif()
# Compiler
if(NOT DEFINED IGROS_CXX)
	set(IGROS_CXX	"clang++")
endif()
# Preset parameters
set(IGROS_KERNEL	"kernel.bin")
set(IGROS_CONFIG_FILE	"config/cmake/${IGROS_CXX}-${IGROS_ARCH}.cmake")
set(IGROS_LINK_SCRIPT	"config/link/link-${IGROS_ARCH}.ld")
set(IGROS_VERSION_MAJOR	"0")
set(IGROS_VERSION_MINOR	"1")
set(IGROS_VERSION_BUILD	"34")
set(IGROS_VERSION	${IGROS_VERSION_MAJOR}.${IGROS_VERSION_MINOR}.${IGROS_VERSION_BUILD})

# Build config
include(${IGROS_CONFIG_FILE})

# Build in Source protection
file(
	TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH
)
if(EXISTS "${LOC_PATH}")
	message(FATAL_ERROR "Cannot build in a source directory (or any with CMakeLists.txt)!")
endif()

# Project
project(
	IgrOS-Kernel
	VERSION ${IGROS_VERSION}
	DESCRIPTION "Simple C++20 Kernel for i386/x86_64"
	LANGUAGES ASM-ATT CXX
)

# Enable CTest
enable_testing()

# Colored output option
option(
	GWE_CXX_COLORED_OUTPUT
	"Produce ANSI-colored output from compiler (GCC/Clang can do it)"
	OFF
)
# ClangTidy pass
option(
	GWE_CXX_CLANG_TIDY
	"Add ClangTidy pass to build process"
	OFF
)

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
	"${CMAKE_SOURCE_DIR}/install/${CMAKE_BUILD_TYPE}"
)
# CMake Module path
set(
	CMAKE_MODULE_PATH
	"${CMAKE_SOURCE_DIR}/cmake/modules"
)


# Enable CCache
include(EnableCCache)


# Use Clang-Tidy ?
if (GWE_CXX_CLANG_TIDY)
	# Clang-Tidy extra args
	if(("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC") OR (("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") AND ("${CMAKE_CXX_SIMULATE_ID}" STREQUAL "MSVC")))
		# Set extra Clang-Tidy checks
		set(
			GWE_CLANG_TIDY_ARGS
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
		${GWE_CLANG_TIDY_ARGS}
	)
endif()

# Printables
string(
	REPLACE " -" "\n\t\t-"
	CMAKE_ASM-ATT_FLAGS_PRINTABLE
	${CMAKE_ASM-ATT_FLAGS}
)
string(
	REPLACE " -" "\n\t\t-"
	CMAKE_CXX_FLAGS_PRINTABLE
	${CMAKE_CXX_FLAGS}
)
string(
	REPLACE " -" "\n\t\t-"
	CMAKE_LINKER_FLAGS_PRINTABLE
	${CMAKE_LINKER_FLAGS}
)

# Header
message(
	"\n"
	"===============================\n"
	"\n"
	"IgrOS Kernel Build v${IGROS_VERSION}\n"
	"\n"
	"Architecture:	${IGROS_ARCH}\n"
	"\n"
	"Compiler:	${CMAKE_CXX_COMPILER}\n"
	"	flags:	${CMAKE_CXX_FLAGS_PRINTABLE}\n"
	"\n"
	"Assembler:	${CMAKE_ASM_COMPILER}\n"
	"	flags:	${CMAKE_ASM-ATT_FLAGS_PRINTABLE}\n"
	"\n"
	"Linker:		${CMAKE_LINKER}\n"
	"	flags:	${CMAKE_LINKER_FLAGS_PRINTABLE}\n"
	"\n"
	"Script:		${IGROS_CONFIG_FILE}\n"
	"\n"
	"Binary:		${IGROS_KERNEL}\n"
	"\n"
	"===============================\n"
)

# Message
message(STATUS "Building Kernel ${IGROS_ARCH}")

# Kernel C++ files
file(
	GLOB
	KERNEL_SRC
	*.cpp
)

# Includes
include_directories(
	include
)

# Kernel executable
add_executable(
	${IGROS_KERNEL}
	${KERNEL_SRC}
)

# Kernel definitions
add_compile_definitions(
	IGROS_ARCH_${IGROS_ARCH}
	IGROS_ARCH=${IGROS_ARCH}
	IGROS_KERNEL
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

# Debug build target
add_custom_target(
	debug
	COMMAND ${CMAKE_COMMAND} -DCMAKE_TOOLCHAIN_FILE=${IGROS_CONFIG_FILE} -DCMAKE_BUILD_TYPE=Debug -S"${CMAKE_SOURCE_DIR}" -H"${CMAKE_SOURCE_DIR}" -B"${CMAKE_BINARY_DIR}/${IGROS_ARCH}" ${CMAKE_SOURCE_DIR}
	COMMAND ${CMAKE_COMMAND} --build "${CMAKE_BINARY_DIR}/${IGROS_ARCH}" --target all
	COMMENT "Switch CMAKE_BUILD_TYPE to Debug"
)
# Profiling build target
add_custom_target(
	profiling
	COMMAND ${CMAKE_COMMAND} -DCMAKE_TOOLCHAIN_FILE=${IGROS_CONFIG_FILE} -DCMAKE_BUILD_TYPE=Profiling -S"${CMAKE_SOURCE_DIR}" -H"${CMAKE_SOURCE_DIR}" -B"${CMAKE_BINARY_DIR}/${IGROS_ARCH}" ${CMAKE_SOURCE_DIR}
	COMMAND ${CMAKE_COMMAND} --build "${CMAKE_BINARY_DIR}/${IGROS_ARCH}" --target all
	COMMENT "Switch CMAKE_BUILD_TYPE to Profiling"
)
# Release build target
add_custom_target(
	release
	COMMAND ${CMAKE_COMMAND} -DCMAKE_TOOLCHAIN_FILE=${IGROS_CONFIG_FILE} -DCMAKE_BUILD_TYPE=Release -S"${CMAKE_SOURCE_DIR}" -H"${CMAKE_SOURCE_DIR}" -B"${CMAKE_BINARY_DIR}/${IGROS_ARCH}" ${CMAKE_SOURCE_DIR}
	COMMAND ${CMAKE_COMMAND} --build "${CMAKE_BINARY_DIR}/${IGROS_ARCH}" --target all
	COMMENT "Switch CMAKE_BUILD_TYPE to Release"
)

# Install rules
install(
	TARGETS ${IGROS_KERNEL}
	CONFIGURATIONS ${CMAKE_BUILD_TYPE}
	DESTINATION "${CMAKE_SOURCE_DIR}/${CMAKE_BUILD_TYPE}/${IGROS_ARCH}"
)

# Link command
set(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_LINKER} <OBJECTS> -o ${IGROS_KERNEL} -T ${CMAKE_SOURCE_DIR}/${IGROS_LINK_SCRIPT} ${CMAKE_LINKER_FLAGS}")

# Qemu for specified arch
set(QEMU "qemu-system-${IGROS_ARCH}")

# Install proxy
add_custom_target(
	install_proxy
	COMMAND ${CMAKE_COMMAND} --build "${CMAKE_BINARY_DIR}" --target install
	DEPENDS ${IGROS_KERNEL}
)

# Test kernel
add_custom_target(
	test_proxy
	USES_TERMINAL
	COMMAND ${QEMU} -kernel "${CMAKE_SOURCE_DIR}/${CMAKE_BUILD_TYPE}/${IGROS_ARCH}/${IGROS_KERNEL}" -m 2G -vga std -rtc base=localtime -serial mon:stdio
	DEPENDS install_proxy
)

# Search Doxygen package
find_package(
	Doxygen
	COMPONENTS dot
)
# If Doxygen found
if(DOXYGEN_FOUND)
	# Documentation output directory
	set(
		DOXYGEN_OUTPUT_DIRECTORY
		"${PROJECT_SOURCE_DIR}/doc/"
	)
	# Documentation build target
	doxygen_add_docs(
		documentation
		"${PROJECT_SOURCE_DIR}/"
		WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
		COMMENT "Generate documentation"
	)
endif()

