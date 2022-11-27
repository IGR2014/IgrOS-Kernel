# Setup cross-compilation
set(
	CMAKE_SYSTEM_NAME
	Linux
)
set(
	CMAKE_SYSTEM_PROCESSOR
	x86_64
)

# Compiler
set(
	CMAKE_CXX_COMPILER
	g++
)
# Compiler flags
set(
	CMAKE_CXX_FLAGS
	"-Wall -Wextra -w -pedantic -Werror -fno-builtin -fno-builtin-function -ffreestanding -fno-exceptions -fno-rtti -fno-threadsafe-statics -fno-pie -nostdlib -O3 -s  -mno-mmx -mno-3dnow -mno-sse -mno-sse2 -mno-sse3 -mno-ssse3 -mno-sse4 -mno-sse4.1 -mno-sse4.2 -mno-sse4a -mno-avx -mno-fma4 -m64 -march=x86-64 -mno-red-zone -mcmodel=large"
)

# Assembler
set(
	CMAKE_ASM_COMPILER
	as
)
# Assembler flags
set(
	CMAKE_ASM-ATT_FLAGS
	"--64"
)

# Linker
set(
	CMAKE_LINKER
	ld
)
# Linker flags
set(
	CMAKE_LINKER_FLAGS
	"-T ${CMAKE_SOURCE_DIR}/config/link/link-x86_64.ld -n -m elf_x86_64 -z max-page-size=0x1000 --no-warn-rwx-segments --no-warn-execstack"
)

