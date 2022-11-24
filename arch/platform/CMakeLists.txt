# Message
message(
	STATUS
	"Building platform files"
)

# Kernel platform C++ headers
file(
	GLOB
	PLATFORM_HEADER
	platform-${CMAKE_SYSTEM_PROCESSOR}.hpp
)
# Kernel platform C++ files
file(
	GLOB
	PLATFORM_SOURCE
	platform-${CMAKE_SYSTEM_PROCESSOR}.cpp
)

# Includes
include_directories(
	"include"
)

# Target sources
target_sources(
	${IGROS_KERNEL}
	PRIVATE
	${PLATFORM_HEADER}
	${PLATFORM_SOURCE}
)

