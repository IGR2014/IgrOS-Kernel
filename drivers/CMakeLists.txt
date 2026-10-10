# Message
message(
	STATUS
	"Building Drivers"
)

# Kernel drivers header files
file(
	GLOB
	DRIVERS_HDR
	CONFIGURE_DEPENDS
	*.hpp
)
# Kernel drivers source files
file(
	GLOB
	DRIVERS_SRC
	CONFIGURE_DEPENDS
	*.cpp
)

# Add clock subdirectory
add_subdirectory(
	clock
)
# Add input subdirectory
add_subdirectory(
	input
)
# Add uart subdirectory
add_subdirectory(
	uart
)
# Add vga subdirectory
add_subdirectory(
	vga
)

# Target sources
target_sources(
	${IGROS_KERNEL}
	PRIVATE
	${DRIVERS_HDR}
	${DRIVERS_SRC}
)

