# Message
message(STATUS "Building sys files")

# Kernel sys C++ files
file(
	GLOB
	SYS_SRC
	*.cpp
)

# Includes
include_directories(
	include/sys
)

# Target sources
target_sources(
	${IGROS_KERNEL}
	PRIVATE
	${SYS_SRC}
)

