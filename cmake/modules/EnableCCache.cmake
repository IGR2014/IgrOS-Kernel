# - Find CCache
# Find the CCache and enables it
#


# Look for CCache
find_program(CCACHE ccache)


# CCache found
if(CCACHE)


	# Enable CCache for C
	set(
		CMAKE_C_COMPILER_LAUNCHER
		${CCACHE}
	)
	# Enable CCache for C++
	set(
		CMAKE_CXX_COMPILER_LAUNCHER
		${CCACHE}
	)


	# Message
	message(
		STATUS
		"Found CCache: ${CCACHE}"
	)


# No CCache found
else()


	# Message
	message(
		STATUS
		"Coud NOT find CCache: (missing CCACHE)"
	)


endif()

