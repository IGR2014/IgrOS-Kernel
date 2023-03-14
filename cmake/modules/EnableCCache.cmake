# - Find CCache
# Find the CCache and enables it
#
#  CCache_VERSION				- CCache executable version.
#  CCache_FOUND					- True if CCache found.
#


# Check cached
if(CCache_VERSION)

	# Already in cache, be silent
	set(
		CCache_FIND_QUIETLY
		TRUE
	)

endif()

# Find ccache executable
find_program(
	CCache_CCache_FOUND
	ccache
)

# CCache found
if(CCache_CCache_FOUND)

	# Get CCache version
	execute_process(
		COMMAND "${CCache_CCache_FOUND}" --version
		OUTPUT_VARIABLE CCache_VERSION
		OUTPUT_STRIP_TRAILING_WHITESPACE
	)

	# CCache version value
	string(
		REGEX
		MATCH "[0-9]+\.[0-9]+(\.[0-9]+)*"
		CCache_VERSION ${CCache_VERSION}
	)

	# Enable CCache for C
	set(
		CMAKE_C_COMPILER_LAUNCHER
		${CCache_CCache_FOUND}
	)
	# Enable CCache for C++
	set(
		CMAKE_CXX_COMPILER_LAUNCHER
		${CCache_CCache_FOUND}
	)

	# Enable CCache for C (over ENV)
	set(
		$ENV{CMAKE_C_COMPILER_LAUNCHER}
		${CCache_CCache_FOUND}
	)
	# Enable CCache for C++ (over ENV)
	set(
		$ENV{CMAKE_CXX_COMPILER_LAUNCHER}
		${CCache_CCache_FOUND}
	)

	# Fix for MSVC debug prevention of ccache work
	set(
		CMAKE_MSVC_DEBUG_INFORMATION_FORMAT
		"$<$<CONFIG:Debug,RelWithDebInfo>:Embedded>"
	)

endif()

# Handle the QUIETLY and REQUIRED arguments and set CCache_FOUND to TRUE if
# all listed variables are TRUE.
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
	CCache
	REQUIRED_VARS CCache_CCache_FOUND
	VERSION_VAR CCache_VERSION
	HANDLE_COMPONENTS
)

# Advanced
mark_as_advanced(
	CCache_CCache_FOUND
)

