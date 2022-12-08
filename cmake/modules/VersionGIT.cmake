# - Get version from GIT
# Get GIT version data


# GIT version string
function(IGROS_GIT_get_version)

	# Get GIT version
	execute_process(
		COMMAND git describe --abbrev=0
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		OUTPUT_VARIABLE IGROS_GIT_VERSION
	)
	# Parse RegExp
	string(
		REGEX MATCH
		"(([0-9]+)\.([0-9]+)\.([0-9]+))"
		IGROS_GIT_VERSION "${IGROS_GIT_VERSION}"
	)
	# Set major version
	set(
		IGROS_GIT_VERSION_MAJOR
		${CMAKE_MATCH_2}
		PARENT_SCOPE
	)
	# Set minor version
	set(
		IGROS_GIT_VERSION_MINOR
		${CMAKE_MATCH_3}
		PARENT_SCOPE
	)
	# Set patch version
	set(
		IGROS_GIT_VERSION_PATCH
		${CMAKE_MATCH_4}
		PARENT_SCOPE
	)

endfunction()


# GIT version string
function(IGROS_GIT_get_commit_number)

	# Get GIT commit number
	execute_process(
		COMMAND git rev-list --all --count
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		OUTPUT_VARIABLE IGROS_GIT_COMMIT
	)
	# Parse RegExp
	string(
		REGEX MATCH
		"([0-9]+)"
		IGROS_GIT_COMMIT "${IGROS_GIT_COMMIT}"
	)
	# Set commit number
	set(
		IGROS_GIT_COMMIT
		${IGROS_GIT_COMMIT}
		PARENT_SCOPE
	)

endfunction()


# GIT version string
function(IGROS_GIT_get_commit_hash)

	# Get GIT hash
	execute_process(
		COMMAND git rev-parse --short HEAD
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		OUTPUT_VARIABLE IGROS_GIT_HASH
	)
	# Parse RegExp
	string(
		REGEX MATCH
		"([0-9a-z]+)"
		IGROS_GIT_HASH "${IGROS_GIT_HASH}"
	)
	# Set commit hash
	set(
		IGROS_GIT_HASH
		${IGROS_GIT_HASH}
		PARENT_SCOPE
	)

endfunction()


# Get GIT version
IGROS_GIT_get_version()
# Get GIT commit number
IGROS_GIT_get_commit_number()
# Get GIT commit hash
IGROS_GIT_get_commit_hash()


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

