# Message
message(STATUS "Building Drivers")

# Kernel drivers C++ files
file(
	GLOB
	DRIVERS_SRC
	*.cpp
)

# Add clock subdirectory
add_subdirectory(
	clock
)
# Add input subdirectory
add_subdirectory(
	input
)
# Add uart subdirectory
add_subdirectory(
	uart
)
# Add vga subdirectory
add_subdirectory(
	vga
)

# Includes
include_directories(
	include/drivers
)

# Target sources
target_sources(
	${IGROS_KERNEL}
	PRIVATE
	${DRIVERS_SRC}
)

