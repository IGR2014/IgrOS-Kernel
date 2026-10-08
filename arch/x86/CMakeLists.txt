# Message
message(
	STATUS
	"Building x86 common files"
)

# Kernel x86 common header files
file(
	GLOB
	X86_HDR
	*.hpp
)
# Kernel x86 common source files
file(
	GLOB
	X86_SRC
	*.cpp
)

# Target sources
target_sources(
	${IGROS_KERNEL}
	PRIVATE
	${X86_HDR}
	${X86_SRC}
)
