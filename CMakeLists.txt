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
	OFF
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
	CONFIGURE_DEPENDS
	*.hpp
)
# Kernel C++ files
file(
	GLOB
	KERNEL_SOURCES
	CONFIGURE_DEPENDS
	*.cpp
)


# Kernel executable
add_executable(
	${IGROS_KERNEL}
	${KERNEL_HEADERS}
	${KERNEL_SOURCES}
)

# Kernel includes
target_include_directories(
	${IGROS_KERNEL}
	PRIVATE
	${CMAKE_SOURCE_DIR}
)
# Kernel definitions
target_compile_definitions(
	${IGROS_KERNEL}
	PRIVATE
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
	"$<$<BOOL:${IGROS_CXX_COLORED_OUTPUT}>:-fcolor-diagnostics>"
)
# GNU compiler flags
set(
	IGROS_GNU_COMPILER_FLAGS
	"$<$<BOOL:${IGROS_CXX_COLORED_OUTPUT}>:-fdiagnostics-color=always>"
)
# Compile options (C++ only: assembler gets toolchain assembler flags)
target_compile_options(
	${IGROS_KERNEL}
	PRIVATE
	# Freestanding kernel, architecture and compiler flags (from toolchain)
	"$<$<COMPILE_LANGUAGE:CXX>:${IGROS_TOOLCHAIN_CXX_FLAGS}>"
	# Optimization by build type (not left to cached CMAKE_CXX_FLAGS_<CONFIG>)
	"$<$<COMPILE_LANGUAGE:CXX>:$<$<CONFIG:Debug>:-Og;-g>>"
	"$<$<COMPILE_LANGUAGE:CXX>:$<$<CONFIG:Release>:-O3>>"
	"$<$<COMPILE_LANGUAGE:CXX>:$<$<CONFIG:RelWithDebInfo>:-O2;-g>>"
	"$<$<COMPILE_LANGUAGE:CXX>:$<$<CONFIG:MinSizeRel>:-Os>>"
	# Colored diagnostics
	"$<$<COMPILE_LANG_AND_ID:CXX,Clang>:${IGROS_CLANG_COMPILER_FLAGS}>"
	"$<$<COMPILE_LANG_AND_ID:CXX,GNU>:${IGROS_GNU_COMPILER_FLAGS}>"
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

# Kernel linker script
set(
	IGROS_LINKER_SCRIPT
	"${CMAKE_SOURCE_DIR}/config/link/link-${CMAKE_SYSTEM_PROCESSOR}.ld"
)
# Link with linker script (relink when it changes)
target_link_options(
	${IGROS_KERNEL}
	PRIVATE
	${IGROS_TOOLCHAIN_LINKER_FLAGS}
	-T "${IGROS_LINKER_SCRIPT}"
)
set_target_properties(
	${IGROS_KERNEL}
	PROPERTIES
	LINK_DEPENDS "${IGROS_LINKER_SCRIPT}"
)
# Link command (kernel is linked by linker directly, not by compiler driver)
set(
	CMAKE_CXX_LINK_EXECUTABLE
	"${CMAKE_LINKER} <LINK_FLAGS> <OBJECTS> -o <TARGET>"
)
# No linker-generated dependency file: CMake adds it in compiler driver
# syntax (-Wl,... / -Xlinker ...), which the linker itself doesn't accept
set(
	CMAKE_LINK_DEPENDS_USE_LINKER
	FALSE
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
	DESTINATION "${CMAKE_INSTALL_PREFIX}/iso/boot/grub"
)
# Create GRUB directory structure
install(
	CODE
"file(
	WRITE \"${CMAKE_INSTALL_PREFIX}/iso/boot/grub/grub.cfg\"
\"set timeout=0
set default=0
menuentry \\\"IgrOS-Kernel\\\" {
	multiboot /boot/${IGROS_KERNEL}
}\"
)"
)
# Install kernel
install(
	TARGETS ${IGROS_KERNEL}
	DESTINATION "${CMAKE_INSTALL_PREFIX}/iso/boot"
)
# Create ISO
install(
	CODE
"execute_process(
	COMMAND grub-mkrescue -o \"${CMAKE_INSTALL_PREFIX}/kernel.iso\" \"${CMAKE_INSTALL_PREFIX}/iso\"
	RESULT_VARIABLE IGROS_ISO_RESULT
	OUTPUT_VARIABLE IGROS_ISO_OUTPUT
	ERROR_VARIABLE IGROS_ISO_OUTPUT
)
if(NOT IGROS_ISO_RESULT EQUAL 0)
	message(FATAL_ERROR \"grub-mkrescue failed: \${IGROS_ISO_OUTPUT}\")
endif()"
)
# Create ISO
install(
	CODE
"execute_process(
	COMMAND objdump --disassemble-all \"${CMAKE_INSTALL_PREFIX}/iso/boot/kernel.bin\"
	OUTPUT_FILE \"${CMAKE_INSTALL_PREFIX}/kernel.disasm.txt\"
	OUTPUT_QUIET
	ERROR_QUIET
)"
)

