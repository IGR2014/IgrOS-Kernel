# - Find Doxygen
# Find the Doxygen and enables it
#


# Search Doxygen package
find_package(
	Doxygen
	COMPONENTS dot
)

# If Doxygen found
if(DOXYGEN_FOUND)

	# Documentation output directory
	set(
		DOXYGEN_OUTPUT_DIRECTORY
		"${PROJECT_SOURCE_DIR}/doc/"
	)

	# Documentation build target
	doxygen_add_docs(
		documentation
		"${PROJECT_SOURCE_DIR}/"
		WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
		COMMENT "Generate documentation"
	)

endif()

