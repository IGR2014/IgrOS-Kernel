# Cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 3.10.0)

# Message
MESSAGE(STATUS "Building platform files")

# C++ Language syntax
ENABLE_LANGUAGE(CXX)

# Kernel platform C++ files
FILE(
	GLOB
	PLATFORM_SRC
	*-${IGROS_ARCH}.cpp
)

# Includes
INCLUDE_DIRECTORIES(
	include/
)

# Target sources
TARGET_SOURCES(
	${IGROS_KERNEL}
	PRIVATE
	${PLATFORM_SRC}
)

