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

# Add x86 family common code
if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(i386|x86_64)$")
	add_subdirectory(
		x86
	)
endif()

# Add proper arch subdirectory
add_subdirectory(
	${CMAKE_SYSTEM_PROCESSOR}
)

