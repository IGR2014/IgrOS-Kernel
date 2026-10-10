# Message
message(
	STATUS
	"Building dev files"
)

# Kernel dev header files
file(
	GLOB
	DEV_HDR
	CONFIGURE_DEPENDS
	*.hpp
)
# Kernel dev source files
file(
	GLOB
	DEV_SRC
	CONFIGURE_DEPENDS
	*.cpp
)

# Target sources
target_sources(
	${IGROS_KERNEL}
	PRIVATE
	${DEV_HDR}
	${DEV_SRC}
)

