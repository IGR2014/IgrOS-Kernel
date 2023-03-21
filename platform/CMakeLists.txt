# Message
message(
	STATUS
	"Building platform files"
)

# Kernel platform header files
file(
	GLOB
	PLATFORM_HEADERS
	platform-${CMAKE_SYSTEM_PROCESSOR}.hpp
)
# Kernel platform source files
file(
	GLOB
	PLATFORM_SOURCES
	platform-${CMAKE_SYSTEM_PROCESSOR}.cpp
)

# Target sources
target_sources(
	${IGROS_KERNEL}
	PRIVATE
	${PLATFORM_HEADERS}
	${PLATFORM_SOURCES}
)

