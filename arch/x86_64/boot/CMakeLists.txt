# Message
message(
	STATUS
	"Building boot files"
)

# Kernel boot x86_64 Assembler files
file(
	GLOB
	BOOT_x86_64_SRC
	*.s
)

# Target sources
target_sources(
	${IGROS_KERNEL}
	PRIVATE
	${BOOT_x86_64_SRC}
)

