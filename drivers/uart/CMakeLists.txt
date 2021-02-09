# Cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 3.10.0)

# Message
MESSAGE(STATUS "Building UART Drivers")

# C++ Language syntax
ENABLE_LANGUAGE(CXX)

# Kernel UART drivers C++ files
FILE(
	GLOB
	DRIVERS_UART_SRC
	*.cpp
)

# Includes
INCLUDE_DIRECTORIES(
	include/drivers/uart
)

# Target sources
TARGET_SOURCES(
	${IGROS_KERNEL}
	PRIVATE
	${DRIVERS_UART_SRC}
)

