# - Get version from GIT
# Get GIT version data


# Find GIT (optional - sources may be built without repository)
find_package(Git QUIET)


# Run GIT command
# Sets OUT variable in parent scope only on success
function(IGROS_GIT_run OUT)

	# Check GIT presence
	if(NOT GIT_FOUND)
		return()
	endif()
	# Run GIT command
	execute_process(
		COMMAND ${GIT_EXECUTABLE} ${ARGN}
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		RESULT_VARIABLE IGROS_GIT_RESULT
		OUTPUT_VARIABLE IGROS_GIT_OUTPUT
		OUTPUT_STRIP_TRAILING_WHITESPACE
		ERROR_QUIET
	)
	# Check result
	if(IGROS_GIT_RESULT EQUAL 0)
		set(
			${OUT}
			"${IGROS_GIT_OUTPUT}"
			PARENT_SCOPE
		)
	endif()

endfunction()


# Defult values
set(IGROS_GIT_VERSION_MAJOR 0)
set(IGROS_GIT_VERSION_MINOR 0)
set(IGROS_GIT_VERSION_PATCH 0)
set(IGROS_GIT_COMMIT 0)
set(IGROS_GIT_HASH 0)


# Get GIT version
set(IGROS_GIT_OUTPUT "")
IGROS_GIT_run(IGROS_GIT_OUTPUT describe --abbrev=0)
if("${IGROS_GIT_OUTPUT}" MATCHES "([0-9]+)\\.([0-9]+)\\.([0-9]+)")
	set(IGROS_GIT_VERSION_MAJOR ${CMAKE_MATCH_1})
	set(IGROS_GIT_VERSION_MINOR ${CMAKE_MATCH_2})
	set(IGROS_GIT_VERSION_PATCH ${CMAKE_MATCH_3})
endif()

# Get GIT commit number
set(IGROS_GIT_OUTPUT "")
IGROS_GIT_run(IGROS_GIT_OUTPUT rev-list --all --count)
if("${IGROS_GIT_OUTPUT}" MATCHES "^([0-9]+)$")
	set(IGROS_GIT_COMMIT ${CMAKE_MATCH_1})
endif()

# Get GIT commit hash
set(IGROS_GIT_OUTPUT "")
IGROS_GIT_run(IGROS_GIT_OUTPUT rev-parse --short HEAD)
if("${IGROS_GIT_OUTPUT}" MATCHES "^([0-9a-f]+)$")
	set(IGROS_GIT_HASH ${CMAKE_MATCH_1})
endif()

# Set version
set(
	IGROS_GIT_VERSION
	"${IGROS_GIT_VERSION_MAJOR}.${IGROS_GIT_VERSION_MINOR}.${IGROS_GIT_VERSION_PATCH}.${IGROS_GIT_COMMIT}"
)


# Show message
message(
	STATUS
	"Version Git: ${IGROS_GIT_VERSION} (${IGROS_GIT_HASH})"
)

