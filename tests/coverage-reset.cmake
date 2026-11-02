# - Coverage reset (script mode, used by coverage target of tests/CMakeLists.txt)
# Removes gcov counters (.gcda) of previous runs under DIR: counters would add up,
# and stale ones (source recompiled since) make gcov fail.
# Also removes coverage notes (.gcno) of targets that no longer exist (removed
# test): gcovr would process them and fail on their missing sources
#
#  Usage: cmake -DDIR=<build dir> -P coverage-reset.cmake
#


file(
	GLOB_RECURSE IGROS_GCDA_FILES
	"${DIR}/*.gcda"
)
if(IGROS_GCDA_FILES)
	file(
		REMOVE
		${IGROS_GCDA_FILES}
	)
endif()

# Object directories of current targets (written by CMake at generate time)
if(EXISTS "${DIR}/CMakeFiles/TargetDirectories.txt")
	file(
		STRINGS "${DIR}/CMakeFiles/TargetDirectories.txt"
		IGROS_TARGET_DIRS
	)
	file(
		GLOB IGROS_OBJECT_DIRS
		LIST_DIRECTORIES true
		"${DIR}/CMakeFiles/*.dir"
	)
	foreach(OBJECT_DIR IN LISTS IGROS_OBJECT_DIRS)
		if(NOT OBJECT_DIR IN_LIST IGROS_TARGET_DIRS)
			file(
				GLOB_RECURSE IGROS_STALE_GCNO_FILES
				"${OBJECT_DIR}/*.gcno"
			)
			if(IGROS_STALE_GCNO_FILES)
				message(
					STATUS
					"Coverage: removing notes of removed target ${OBJECT_DIR}"
				)
				file(
					REMOVE
					${IGROS_STALE_GCNO_FILES}
				)
			endif()
		endif()
	endforeach()
endif()
