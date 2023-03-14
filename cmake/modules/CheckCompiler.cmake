# - Check Compiler
# checks compiler version
#


# Check compiler is Clang
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	# Clang minimum required version
	set(
		IGROS_COMPILER_VERSION_MINIMUM
		"15.0.0"
	)
# Check compiler is GCC
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	# GCC minimum required version
	set(
		IGROS_COMPILER_VERSION_MINIMUM
		"12.0.0"
	)
# Unknown compiler
else()
	# Placeholder minimum required version
	set(
		IGROS_COMPILER_VERSION_MINIMUM
		"0.0.0"
	)
	# Error message
	message(
		FATAL_ERRROR
		"Unknown/unsupported compiler ${CMAKE_CXX_COMPILER_ID}!"
	)
endif()

# Check version
if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "${IGROS_COMPILER_VERSION_MINIMUM}")
	# Error message
	message(
		FATAL_ERRROR
		"Unsupported ${CMAKE_CXX_COMPILER_ID} version: ${IGROS_COMPILER_VERSION_MINIMUM}+ required, ${CMAKE_CXX_COMPILER_VERSION} found!"
	)
endif()

