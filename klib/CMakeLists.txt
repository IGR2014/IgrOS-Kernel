# Message
message(
	STATUS
	"Building klib files"
)

# Kernel klib header files
file(
	GLOB
	KLIB_HDR
	*.hpp
)
# Kernel klib source files
file(
	GLOB
	KLIB_SRC
	*.cpp
)

# Target sources
target_sources(
	${IGROS_KERNEL}
	PRIVATE
	${KLIB_HDR}
	${KLIB_SRC}
)

