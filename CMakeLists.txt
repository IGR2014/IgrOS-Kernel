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
# LTO
set(
	CMAKE_INTERPROCEDURAL_OPTIMIZATION
	"$<BOOL:$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>>>"
)
# PIC
set(
	CMAKE_POSITION_INDEPENDENT_CODE
	ON
)
# Hidden inlines
set(
	CMAKE_VISIBILITY_INLINES_HIDDEN
	True
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

# Preset parameters
set(
	IGROS_KERNEL
	"kernel.bin"
)

# Project
project(
	"IgrOS-Kernel"
	VERSION ${IGROS_GIT_VERSION}
	DESCRIPTION "Simple C++20 Kernel for i386/x86_64"
	HOMEPAGE_URL "https://github.com/IGR2014/IgrOS-Kernel"
	LANGUAGES ASM ASM-ATT CXX
)

# Check Compiler
include(CheckCompiler)
# Enable CCache
include(EnableCCache)
# Enable Doxygen
include(EnableDoxygen)

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

# Header
message(
	STATUS
	"IgrOS Kernel"
)
message(
	STATUS
	"Architecture: ${CMAKE_SYSTEM_PROCESSOR}"
)
message(
	STATUS
	"Compiler: ${CMAKE_CXX_COMPILER}"
)
message(
	STATUS
	"Assembler: ${CMAKE_ASM_COMPILER}"
)
message(
	STATUS
	"Linker: ${CMAKE_LINKER}"
)
message(
	STATUS
	"Script: ${CMAKE_TOOLCHAIN_FILE}"
)
message(
	STATUS
	"Binary: ${IGROS_KERNEL}"
)

# Message
message(
	STATUS
	"Building Kernel ${CMAKE_SYSTEM_PROCESSOR}"
)

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
	${CMAKE_SOURCE_DIR}
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

# Clang compiler flags
set(
	IGROS_CLANG_COMPILER_FLAGS
	"$<$<BOOL:IGROS_CXX_COLORED_OUTPUT>:-fcolor-diagnostics>"
)
# GNU compiler flags
set(
	IGROS_GNU_COMPILER_FLAGS
	"$<$<BOOL:IGROS_CXX_COLORED_OUTPUT>:-fdiagnostics-color=always>"
)
# Compile options
add_compile_options(
	# Add compiler flags if Clang
	"$<$<CXX_COMPILER_ID:Clang>:${IGROS_CLANG_COMPILER_FLAGS}>"
	# Add compiler flags if GCC
	"$<$<CXX_COMPILER_ID:GNU>:${IGROS_GNU_COMPILER_FLAGS}>"
)

# Add arch subdirectory
add_subdirectory(
	arch
)
# Add dev subdirectory
add_subdirectory(
	dev
)
# Add drivers subdirectory
add_subdirectory(
	drivers
)
# Add klib subdirectory
add_subdirectory(
	klib
)
# Add mem subdirectory
add_subdirectory(
	mem
)
# Add multiboot subdirectory
add_subdirectory(
	multiboot
)
# Add platform subdirectory
add_subdirectory(
	platform
)
# Add sys subdirectory
add_subdirectory(
	sys
)

# Link command
set(
	CMAKE_CXX_LINK_EXECUTABLE
	"${CMAKE_LINKER} <OBJECTS> -o ${IGROS_KERNEL} ${CMAKE_LINKER_FLAGS}"
)

# Get compiler name
get_filename_component(
	IGROS_COMPILER_NAME
	${CMAKE_CXX_COMPILER}
	NAME_WE
)

# Create directory
install(
	DIRECTORY
	DESTINATION "${IGROS_COMPILER_NAME}-${CMAKE_SYSTEM_PROCESSOR}/iso/boot/grub"
)
# Create GRUB directory structure
install(
	CODE
"file(
	WRITE \"${CMAKE_INSTALL_PREFIX}${IGROS_COMPILER_NAME}-${CMAKE_SYSTEM_PROCESSOR}/iso/boot/grub/grub.cfg\"
\"menuentry \\\"IgrOS-Kernel\\\" {
	multiboot /boot/${IGROS_KERNEL}
}\"
)"
)
# Install kernel
install(
	TARGETS ${IGROS_KERNEL}
	DESTINATION "${IGROS_COMPILER_NAME}-${CMAKE_SYSTEM_PROCESSOR}/iso/boot"
)
# Create ISO
install(
	CODE
"execute_process(
	COMMAND grub-mkrescue -o \"${CMAKE_INSTALL_PREFIX}${IGROS_COMPILER_NAME}-${CMAKE_SYSTEM_PROCESSOR}/kernel.iso\" \"${CMAKE_INSTALL_PREFIX}${IGROS_COMPILER_NAME}-${CMAKE_SYSTEM_PROCESSOR}/iso\"
	OUTPUT_QUIET
	ERROR_QUIET
)"
)
# Create ISO
install(
	CODE
"execute_process(
	COMMAND objdump --disassemble-all \"${CMAKE_INSTALL_PREFIX}${IGROS_COMPILER_NAME}-${CMAKE_SYSTEM_PROCESSOR}/iso/boot/kernel.bin\"
	OUTPUT_FILE \"${CMAKE_INSTALL_PREFIX}${IGROS_COMPILER_NAME}-${CMAKE_SYSTEM_PROCESSOR}/kernel.disasm.txt\"
	OUTPUT_QUIET
	ERROR_QUIET
)"
)

