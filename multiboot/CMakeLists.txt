# Message
message(STATUS "Building multiboot files")

# Kernel multiboot C++ files
file(
	GLOB
	MULTIBOOT_SRC
	*.cpp
)

# Includes
include_directories(
	include/multiboot
)

# Target sources
target_sources(
	${IGROS_KERNEL}
	PRIVATE
	${MULTIBOOT_SRC}
)

