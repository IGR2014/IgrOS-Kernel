# Message
message(
	STATUS
	"Building boot files"
)

# Kernel boot i386 Assembler files
file(
	GLOB
	BOOT_i386_SRC
	CONFIGURE_DEPENDS
	*.s
)

# Target sources
target_sources(
	${IGROS_KERNEL}
	PRIVATE
	${BOOT_i386_SRC}
)

