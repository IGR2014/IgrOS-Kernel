# Message
message(
	STATUS
	"Building Clock Drivers"
)

# Kernel clock drivers header files
file(
	GLOB
	DRIVERS_CLOCK_HDR
	*.hpp
)
# Kernel clock drivers source files
file(
	GLOB
	DRIVERS_CLOCK_SRC
	*.cpp
)

# Target sources
target_sources(
	${IGROS_KERNEL}
	PRIVATE
	${DRIVERS_CLOCK_HDR}
	${DRIVERS_CLOCK_SRC}
)

