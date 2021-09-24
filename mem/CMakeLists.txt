# Message
message(STATUS "Building mem files")

# Kernel mem C++ files
file(
	GLOB
	MEM_SRC
	*.cpp
)

# Includes
include_directories(
	include/mem
)

# Target sources
target_sources(
	${IGROS_KERNEL}
	PRIVATE
	${MEM_SRC}
)

