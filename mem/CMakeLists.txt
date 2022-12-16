# Message
message(
	STATUS
	"Building mem files"
)

# Kernel mem header files
file(
	GLOB
	MEM_HDR
	*.hpp
)
# Kernel mem source files
file(
	GLOB
	MEM_SRC
	*.cpp
)

# Target sources
target_sources(
	${IGROS_KERNEL}
	PRIVATE
	${MEM_HDR}
	${MEM_SRC}
)

