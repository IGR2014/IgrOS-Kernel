# Message
message(
	STATUS
	"Building klib files"
)

# Kernel klib header files
file(
	GLOB
	KLIB_HDR
	CONFIGURE_DEPENDS
	*.hpp
)
# Kernel klib source files
file(
	GLOB
	KLIB_SRC
	CONFIGURE_DEPENDS
	*.cpp
)

# Target sources
target_sources(
	${IGROS_KERNEL}
	PRIVATE
	${KLIB_HDR}
	${KLIB_SRC}
)

