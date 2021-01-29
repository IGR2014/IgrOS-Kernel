# Cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 3.10.0)

# Message
MESSAGE(STATUS "Building mem files")

# C++ Language syntax
ENABLE_LANGUAGE(CXX)

# Kernel mem C++ files
FILE(
	GLOB
	MEM_SRC
	*.cpp
)

# Includes
INCLUDE_DIRECTORIES(
	include/mem
)

# Target sources
TARGET_SOURCES(
	${IGROS_KERNEL}
	PRIVATE
	${MEM_SRC}
)

