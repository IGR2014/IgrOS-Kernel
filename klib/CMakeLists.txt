# Message
message(STATUS "Building klib files")

# Kernel klib C++ files
file(
	GLOB
	KLIB_SRC
	*.cpp
)

# Includes
include_directories(
	include/klib
)

# Target sources
target_sources(
	${IGROS_KERNEL}
	PRIVATE
	${KLIB_SRC}
)

