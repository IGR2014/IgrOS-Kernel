# Message
message(
	STATUS
	"Building multiboot files"
)

# Kernel multiboot header files
file(
	GLOB
	MULTIBOOT_HDR
	CONFIGURE_DEPENDS
	*.hpp
)
# Kernel multiboot src files
file(
	GLOB
	MULTIBOOT_SRC
	CONFIGURE_DEPENDS
	*.cpp
)

# Target sources
target_sources(
	${IGROS_KERNEL}
	PRIVATE
	${MULTIBOOT_HDR}
	${MULTIBOOT_SRC}
)

