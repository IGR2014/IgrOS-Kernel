# Message
MESSAGE(STATUS "Building boot files")

# AT&T Assembler syntax
ENABLE_LANGUAGE(ASM-ATT)

# Kernel boot x86_64 Assembler files
FILE(
	GLOB
	BOOT_x86_64_SRC
	*.s
)

# Target sources
TARGET_SOURCES(
	${IGROS_KERNEL}
	PRIVATE
	${BOOT_x86_64_SRC}
)

