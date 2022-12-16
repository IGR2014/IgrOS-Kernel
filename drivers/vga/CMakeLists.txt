# Message
message(
	STATUS
	"Building VGA"
)

# Kernel VGA drivers header files
file(
	GLOB
	DRIVERS_VGA_HDR
	*.hpp
)
# Kernel VGA drivers source files
file(
	GLOB
	DRIVERS_VGA_SRC
	*.cpp
)

# Target sources
target_sources(
	${IGROS_KERNEL}
	PRIVATE
	${DRIVERS_VGA_HDR}
	${DRIVERS_VGA_SRC}
)

