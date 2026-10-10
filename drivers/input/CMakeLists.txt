# Message
message(
	STATUS
	"Building Input Drivers"
)

# Kernel input drivers header files
file(
	GLOB
	DRIVERS_INPUT_HDR
	CONFIGURE_DEPENDS
	*.hpp
)
# Kernel input drivers source files
file(
	GLOB
	DRIVERS_INPUT_SRC
	CONFIGURE_DEPENDS
	*.cpp
)

# Target sources
target_sources(
	${IGROS_KERNEL}
	PRIVATE
	${DRIVERS_INPUT_HDR}
	${DRIVERS_INPUT_SRC}
)

