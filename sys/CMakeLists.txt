# Cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 3.10.0)

# Message
MESSAGE(STATUS "Building sys files")

# C++ Language syntax
ENABLE_LANGUAGE(CXX)

# Kernel sys C++ files
FILE(
	GLOB
	SYS_SRC
	*.cpp
)

# Includes
INCLUDE_DIRECTORIES(
	include/sys
)

# Target sources
TARGET_SOURCES(
	${IGROS_KERNEL}
	PRIVATE
	${SYS_SRC}
)

