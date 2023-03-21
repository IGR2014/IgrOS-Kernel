# Message
message(
	STATUS
	"Building x86_64 files"
)

# Kernel x86_64 arch header files
file(
	GLOB
	x86_64_HDR
	*.hpp
)
# Kernel x86_64 arch source files
file(
	GLOB
	x86_64_SRC
	*.cpp
)

# Target sources
target_sources(
	${IGROS_KERNEL}
	PRIVATE
	${x86_64_HDR}
	${x86_64_SRC}
)

# Add boot subdirectory
add_subdirectory(
	boot
)

