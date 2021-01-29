# Message
MESSAGE(STATUS "Building boot files")

# AT&T Assembler syntax
ENABLE_LANGUAGE(ASM-ATT)

# Kernel boot i386 Assembler files
FILE(
	GLOB
	BOOT_i386_SRC
	*.s
)

# Target sources
TARGET_SOURCES(
	${IGROS_KERNEL}
	PRIVATE
	${BOOT_i386_SRC}
)

