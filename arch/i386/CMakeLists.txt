# Message
message(STATUS "Building i386 files")

# Kernel i386 arch C++ files
file(
	GLOB
	I386_SRC
	*.cpp
)

# Includes
include_directories(
	include/arch/i386
)

# Target sources
target_sources(
	${IGROS_KERNEL}
	PRIVATE
	${I386_SRC}
)

# Add boot subdirectory
add_subdirectory(
	boot
)

