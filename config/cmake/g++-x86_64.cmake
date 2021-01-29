# Setup cross-compilation
set(CMAKE_SYSTEM_NAME		Linux)
set(CMAKE_SYSTEM_PROCESSOR	x86_64)

# Compiler
SET(
	CMAKE_CXX_COMPILER
	g++
)
# Compiler flags
SET(
	CMAKE_CXX_FLAGS
	"-Wall -Wextra -w -pedantic -Werror -ffreestanding -fno-exceptions -fno-rtti -fno-threadsafe-statics -fno-pie -nostdlib -O3 -s -mcld -mno-3dnow -mno-sse -mno-sse2 -mno-sse2avx -mno-sse3 -mno-ssse3 -mno-sse4 -mno-sse4.1 -mno-sse4.2 -mno-sse4a -mno-mmx -mno-avx -mno-fma4 -m64 -march=x86-64 -mno-red-zone -mcmodel=large"
)


# Assembler
SET(
	CMAKE_ASM_COMPILER
	as
)
# Assembler flags
SET(
	CMAKE_ASM-ATT_FLAGS
	"--64"
)

# Linker
SET(
	CMAKE_LINKER
	ld
)
# Linker flags
SET(
	CMAKE_LINKER_FLAGS
	"-n -m elf_x86_64 -z max-page-size=0x1000"
)

