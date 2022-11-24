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

# Includes
include_directories(
	"include/arch"
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
# Add platform subdirectory
add_subdirectory(
	platform
)

