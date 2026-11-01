# - Coverage reset (script mode, used by coverage target of tests/CMakeLists.txt)
# Removes gcov counters (.gcda) of previous runs under DIR: counters would add up,
# and stale ones (source recompiled since) make gcov fail
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
