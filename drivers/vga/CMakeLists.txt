# Message
message(STATUS "Building VGA Drivers")

# Kernel VGA drivers C++ files
file(
	GLOB
	DRIVERS_VGA_SRC
	*.cpp
)

# Includes
include_directories(
	include/drivers/vga
)

# Target sources
target_sources(
	${IGROS_KERNEL}
	PRIVATE
	${DRIVERS_VGA_SRC}
)

