# Message
message(
	STATUS
	"Building arch files"
)

# Kernel arch C++ headers
file(
	GLOB
	ARCH_HEADERS
	*.hpp
)
# Kernel arch C++ files
file(
	GLOB
	ARCH_SOURCES
	*.cpp
)

# Target sources
target_sources(
	${IGROS_KERNEL}
	PRIVATE
	${ARCH_HEADERS}
	${ARCH_SOURCES}
)

# Add proper arch subdirectory
add_subdirectory(
	${CMAKE_SYSTEM_PROCESSOR}
)

