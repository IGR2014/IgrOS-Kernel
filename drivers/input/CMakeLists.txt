# Message
message(STATUS "Building Input Drivers")

# Kernel input drivers C++ files
file(
	GLOB
	DRIVERS_INPUT_SRC
	*.cpp
)

# Includes
include_directories(
	include/drivers/input
)

# Target sources
target_sources(
	${IGROS_KERNEL}
	PRIVATE
	${DRIVERS_INPUT_SRC}
)

