# Cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 3.10.0)

# Message
MESSAGE(STATUS "Building VGA Drivers")

# C++ Language syntax
ENABLE_LANGUAGE(CXX)

# Kernel VGA drivers C++ files
FILE(
	GLOB
	DRIVERS_VGA_SRC
	*.cpp
)

# Includes
INCLUDE_DIRECTORIES(
	include/drivers/vga
)

# Target sources
TARGET_SOURCES(
	${IGROS_KERNEL}
	PRIVATE
	${DRIVERS_VGA_SRC}
)

