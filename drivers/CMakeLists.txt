# Cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 3.10.0)

# Message
MESSAGE(STATUS "Building Drivers")

# C++ Language syntax
ENABLE_LANGUAGE(CXX)

# Kernel drivers C++ files
FILE(
	GLOB
	DRIVERS_SRC
	*.cpp
)

# Add clock subdirectory
ADD_SUBDIRECTORY(
	clock
	clock
)
# Add input subdirectory
ADD_SUBDIRECTORY(
	input
	input
)
# Add uart subdirectory
ADD_SUBDIRECTORY(
	uart
	uart
)
# Add vga subdirectory
ADD_SUBDIRECTORY(
	vga
	vga
)

# Includes
INCLUDE_DIRECTORIES(
	include/drivers
)

# Target sources
TARGET_SOURCES(
	${IGROS_KERNEL}
	PRIVATE
	${DRIVERS_SRC}
)

