# Setup cross-compilation
set(CMAKE_SYSTEM_NAME		Linux)
set(CMAKE_SYSTEM_PROCESSOR	i386)

# Compiler
SET(
	CMAKE_CXX_COMPILER
	clang++
)
# Compiler flags
SET(
	CMAKE_CXX_FLAGS
	"-Wall -Wextra -w -pedantic -Werror -fno-builtin -fno-builtin-function -ffreestanding -fno-exceptions -fno-rtti -fno-threadsafe-statics -fno-pie -nostdlib -O3 -s -mno-3dnow -mno-sse -mno-sse2 -mno-sse3 -mno-ssse3 -mno-sse4 -mno-sse4.1 -mno-sse4.2 -mno-sse4a -mno-mmx -mno-avx -mno-fma4 -m32 -march=i386 -mno-red-zone -target i386-linux-elf"
)


# Assembler
SET(
	CMAKE_ASM_COMPILER
	as
)
# Assembler flags
SET(
	CMAKE_ASM-ATT_FLAGS
	"--32"
)

# Linker
SET(
	CMAKE_LINKER
	ld
)
# Linker flags
SET(
	CMAKE_LINKER_FLAGS
	"-n -m elf_i386 -z max-page-size=0x1000"
)

