# Cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 3.10.0)

# Message
MESSAGE(STATUS "Building i386 files")

# C++ Language syntax
ENABLE_LANGUAGE(CXX)

# Kernel i386 arch C++ files
FILE(
	GLOB
	I386_SRC
	*.cpp
)

# Includes
INCLUDE_DIRECTORIES(
	include/arch/i386
)

# Target sources
TARGET_SOURCES(
	${IGROS_KERNEL}
	PRIVATE
	${I386_SRC}
)

# Add boot subdirectory
ADD_SUBDIRECTORY(
	boot
	boot
)

