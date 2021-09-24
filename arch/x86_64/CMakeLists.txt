# Message
message(STATUS "Building x86_64 files")

# Kernel x86_64 arch C++ files
file(
	GLOB
	x86_64_SRC
	*.cpp
)

# Includes
include_directories(
	include/arch/x86_64
)

# Target sources
target_sources(
	${IGROS_KERNEL}
	PRIVATE
	${x86_64_SRC}
)

# Add boot subdirectory
add_subdirectory(
	boot
)

