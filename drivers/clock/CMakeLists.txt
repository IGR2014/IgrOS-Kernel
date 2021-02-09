# Cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 3.10.0)

# Message
MESSAGE(STATUS "Building Clock Drivers")

# C++ Language syntax
ENABLE_LANGUAGE(CXX)

# Kernel clock drivers C++ files
FILE(
	GLOB
	DRIVERS_CLOCK_SRC
	*.cpp
)

# Includes
INCLUDE_DIRECTORIES(
	include/drivers/clock
)

# Target sources
TARGET_SOURCES(
	${IGROS_KERNEL}
	PRIVATE
	${DRIVERS_CLOCK_SRC}
)

