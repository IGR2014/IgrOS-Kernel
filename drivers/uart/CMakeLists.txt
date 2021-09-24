# Message
message(STATUS "Building UART Drivers")

# Kernel UART drivers C++ files
file(
	GLOB
	DRIVERS_UART_SRC
	*.cpp
)

# Includes
include_directories(
	include/drivers/uart
)

# Target sources
target_sources(
	${IGROS_KERNEL}
	PRIVATE
	${DRIVERS_UART_SRC}
)

