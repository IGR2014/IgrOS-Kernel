# Message
message(
	STATUS
	"Building UART"
)

# Kernel UART drivers header files
file(
	GLOB
	DRIVERS_UART_HDR
	*.hpp
)
# Kernel UART drivers source files
file(
	GLOB
	DRIVERS_UART_SRC
	*.cpp
)

# Target sources
target_sources(
	${IGROS_KERNEL}
	PRIVATE
	${DRIVERS_UART_HDR}
	${DRIVERS_UART_SRC}
)

