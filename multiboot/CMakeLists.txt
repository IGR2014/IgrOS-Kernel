# Message
message(
	STATUS
	"Building multiboot files"
)

# Kernel multiboot header files
file(
	GLOB
	MULTIBOOT_HDR
	*.hpp
)
# Kernel multiboot src files
file(
	GLOB
	MULTIBOOT_SRC
	*.cpp
)

# Target sources
target_sources(
	${IGROS_KERNEL}
	PRIVATE
	${MULTIBOOT_HDR}
	${MULTIBOOT_SRC}
)

