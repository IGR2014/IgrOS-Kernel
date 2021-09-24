# Message
message(STATUS "Building platform files")

# Kernel platform C++ files
file(
	GLOB
	PLATFORM_SRC
	platform-${IGROS_ARCH}.cpp
)

# Includes
include_directories(
	include/
)

# Target sources
target_sources(
	${IGROS_KERNEL}
	PRIVATE
	${PLATFORM_SRC}
)

