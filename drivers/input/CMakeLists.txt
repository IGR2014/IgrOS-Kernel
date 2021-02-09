# Cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 3.10.0)

# Message
MESSAGE(STATUS "Building Input Drivers")

# C++ Language syntax
ENABLE_LANGUAGE(CXX)

# Kernel input drivers C++ files
FILE(
	GLOB
	DRIVERS_INPUT_SRC
	*.cpp
)

# Includes
INCLUDE_DIRECTORIES(
	include/drivers/input
)

# Target sources
TARGET_SOURCES(
	${IGROS_KERNEL}
	PRIVATE
	${DRIVERS_INPUT_SRC}
)

