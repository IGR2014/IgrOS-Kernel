# Cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 3.10.0)

# Message
MESSAGE(STATUS "Building klib files")

# C++ Language syntax
ENABLE_LANGUAGE(CXX)

# Kernel klib C++ files
FILE(
	GLOB
	KLIB_SRC
	*.cpp
)

# Includes
INCLUDE_DIRECTORIES(
	include/klib
)

# Target sources
TARGET_SOURCES(
	${IGROS_KERNEL}
	PRIVATE
	${KLIB_SRC}
)

