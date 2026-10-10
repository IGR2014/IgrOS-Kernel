################################################################
#
#	clang++ x86_64 kernel toolchain
#
################################################################


# Toolchain parameters
set(
	IGROS_TOOLCHAIN_ARCH
	x86_64
)
set(
	IGROS_TOOLCHAIN_COMPILER
	clang++
)

# Common toolchain setup
include(
	${CMAKE_CURRENT_LIST_DIR}/toolchain-common.cmake
)
