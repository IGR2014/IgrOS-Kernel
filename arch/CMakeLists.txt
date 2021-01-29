# Cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 3.10.0)

# Message
MESSAGE(STATUS "Building arch files")

# C++ Language syntax
ENABLE_LANGUAGE(CXX)

# Kernel arch C++ files
FILE(
	GLOB
	ARCH_SRC
	*.cpp
)

# Includes
INCLUDE_DIRECTORIES(
	include/arch
)

# Target sources
TARGET_SOURCES(
	${IGROS_KERNEL}
	PRIVATE
	${ARCH_SRC}
)

# Add proper arch subdirectory
ADD_SUBDIRECTORY(
	${IGROS_ARCH}
	${IGROS_ARCH}
)

