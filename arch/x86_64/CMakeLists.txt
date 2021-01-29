# Cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 3.10.0)

# Message
MESSAGE(STATUS "Building x86_64 files")

# C++ Language syntax
ENABLE_LANGUAGE(CXX)

# Kernel x86_64 arch C++ files
FILE(
	GLOB
	x86_64_SRC
	*.cpp
)

# Includes
INCLUDE_DIRECTORIES(
	include/arch/x86_64
)

# Target sources
TARGET_SOURCES(
	${IGROS_KERNEL}
	PRIVATE
	${x86_64_SRC}
)

# Add boot subdirectory
ADD_SUBDIRECTORY(
	boot
	boot
)

