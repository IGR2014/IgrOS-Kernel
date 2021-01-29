# Cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 3.10.0)

# Message
MESSAGE(STATUS "Building multiboot files")

# C++ Language syntax
ENABLE_LANGUAGE(CXX)

# Kernel multiboot C++ files
FILE(
	GLOB
	MULTIBOOT_SRC
	*.cpp
)

# Includes
INCLUDE_DIRECTORIES(
	include/multiboot
)

# Target sources
TARGET_SOURCES(
	${IGROS_KERNEL}
	PRIVATE
	${MULTIBOOT_SRC}
)

