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

