################################################################
#
#	Common part of kernel toolchains
#
#	Toolchain file sets before including this one:
#		IGROS_TOOLCHAIN_ARCH		- i386 | x86_64
#		IGROS_TOOLCHAIN_COMPILER	- g++ | clang++
#
################################################################


# Setup cross-compilation
set(
	CMAKE_SYSTEM_NAME
	Linux
)
set(
	CMAKE_SYSTEM_PROCESSOR
	${IGROS_TOOLCHAIN_ARCH}
)

# Compiler
set(
	CMAKE_CXX_COMPILER
	${IGROS_TOOLCHAIN_COMPILER}
)

# Freestanding kernel code: no runtime, no exceptions/RTTI, no FPU/SIMD, no PIC
set(
	IGROS_TOOLCHAIN_CXX_FLAGS
	"-Wall -Wextra -pedantic -Werror -ffreestanding -fno-builtin -fno-exceptions -fno-rtti -fno-threadsafe-statics -fno-pic -fno-pie -mno-mmx -mno-3dnow -mno-sse -mno-sse2 -mno-sse3 -mno-ssse3 -mno-sse4 -mno-sse4.1 -mno-sse4.2 -mno-sse4a -mno-avx -mno-fma4"
)

# Architecture specifics
if(IGROS_TOOLCHAIN_ARCH STREQUAL "i386")
	string(
		APPEND IGROS_TOOLCHAIN_CXX_FLAGS
		" -m32 -march=i386"
	)
	set(
		IGROS_TOOLCHAIN_ASM_FLAGS
		"--32"
	)
	set(
		IGROS_TOOLCHAIN_LINKER_EMULATION
		elf_i386
	)
elseif(IGROS_TOOLCHAIN_ARCH STREQUAL "x86_64")
	# Kernel is linked at -2 GB: large code model; no red zone in interrupt context
	string(
		APPEND IGROS_TOOLCHAIN_CXX_FLAGS
		" -m64 -march=x86-64 -mno-red-zone -mcmodel=large"
	)
	set(
		IGROS_TOOLCHAIN_ASM_FLAGS
		"--64"
	)
	set(
		IGROS_TOOLCHAIN_LINKER_EMULATION
		elf_x86_64
	)
else()
	message(
		FATAL_ERROR
		"Unsupported toolchain architecture: ${IGROS_TOOLCHAIN_ARCH}"
	)
endif()

# Compiler specifics
if(IGROS_TOOLCHAIN_COMPILER MATCHES "clang")
	string(
		APPEND IGROS_TOOLCHAIN_CXX_FLAGS
		" -target ${IGROS_TOOLCHAIN_ARCH}-linux-elf"
	)
	set(
		CMAKE_LINKER
		ld.lld
	)
else()
	set(
		CMAKE_LINKER
		ld
	)
endif()

# Compiler flags (optimization comes from build type)
set(
	CMAKE_CXX_FLAGS_INIT
	"${IGROS_TOOLCHAIN_CXX_FLAGS}"
)
set(
	CMAKE_CXX_FLAGS_DEBUG_INIT
	"-Og"
)

# Assembler (GNU as, AT&T syntax)
set(
	CMAKE_ASM_COMPILER
	as
)
set(
	CMAKE_ASM-ATT_FLAGS
	"${IGROS_TOOLCHAIN_ASM_FLAGS}"
)

# Linker flags (linker is invoked directly by kernel target, see CMakeLists.txt;
# not CMAKE_EXE_LINKER_FLAGS - CMake checks link through compiler driver)
set(
	IGROS_TOOLCHAIN_LINKER_FLAGS
	# SHELL: keeps option pairs together (CMake de-duplicates options like "-z")
	-n
	"SHELL:-m ${IGROS_TOOLCHAIN_LINKER_EMULATION}"
	"SHELL:-z max-page-size=0x1000"
	"SHELL:-z noexecstack"
)

# Freestanding: compiler checks can't link executables
set(
	CMAKE_TRY_COMPILE_TARGET_TYPE
	STATIC_LIBRARY
)
