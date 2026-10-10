# Message
message(
	STATUS
	"Building sys files"
)

# Kernel sys header files
file(
	GLOB
	SYS_HDR
	CONFIGURE_DEPENDS
	*.hpp
)
# Kernel sys source files
file(
	GLOB
	SYS_SRC
	CONFIGURE_DEPENDS
	*.cpp
)

# Target sources
target_sources(
	${IGROS_KERNEL}
	PRIVATE
	${SYS_HDR}
	${SYS_SRC}
)

