# Message
message(STATUS "Building Clock Drivers")

# Kernel clock drivers C++ files
file(
	GLOB
	DRIVERS_CLOCK_SRC
	*.cpp
)

# Includes
include_directories(
	include/drivers/clock
)

# Target sources
target_sources(
	${IGROS_KERNEL}
	PRIVATE
	${DRIVERS_CLOCK_SRC}
)

