# Message
message(
	STATUS
	"Building i386 files"
)

# Kernel i386 arch header files
file(
	GLOB
	I386_HDR
	*.hpp
)
# Kernel i386 arch source files
file(
	GLOB
	I386_SRC
	*.cpp
)

# Target sources
target_sources(
	${IGROS_KERNEL}
	PRIVATE
	${I386_HDR}
	${I386_SRC}
)

# Add boot subdirectory
add_subdirectory(
	boot
)

