# Message
message(
	STATUS
	"Building sys files"
)

# Kernel sys header files
file(
	GLOB
	SYS_HDR
	*.hpp
)
# Kernel sys source files
file(
	GLOB
	SYS_SRC
	*.cpp
)

# Target sources
target_sources(
	${IGROS_KERNEL}
	PRIVATE
	${SYS_HDR}
	${SYS_SRC}
)

