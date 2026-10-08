# Message
message(
	STATUS
	"Building platform files"
)

# Platform name (i386 and x86_64 share x86 platform)
set(
	IGROS_PLATFORM
	${CMAKE_SYSTEM_PROCESSOR}
)
if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(i386|x86_64)$")
	set(
		IGROS_PLATFORM
		x86
	)
endif()

# Kernel platform header files
file(
	GLOB
	PLATFORM_HEADERS
	platform-${IGROS_PLATFORM}.hpp
)
# Kernel platform source files
file(
	GLOB
	PLATFORM_SOURCES
	platform-${IGROS_PLATFORM}.cpp
)

# Target sources
target_sources(
	${IGROS_KERNEL}
	PRIVATE
	${PLATFORM_HEADERS}
	${PLATFORM_SOURCES}
)

