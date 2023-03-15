# - Find Doxygen
# Find the Doxygen and enable it
#


# Search Doxygen package
find_package(
	Doxygen
	COMPONENTS dot
)

# If Doxygen found
if(DOXYGEN_FOUND)

	# Doxygen recursive
	set(
		DOXYGEN_RECURSIVE
		True
	)

	# Doxygen generate HTML docs
	set(
		DOXYGEN_GENERATE_HTML
		YES
	)

	# Documentation exclued directories
	set(
		DOXYGEN_EXCLUDE_PATTERNS
		"*/.ccache/*"
		"*/.git/*"
		"*/.github/*"
		"*/.idea/*"
		"*/.kdev4/*"
		"*/.vs/*"
		"*/.vscode/*"
		"*/build/*"
		"*/cmake/*"
		"*/docs/*"
		"*/install/*"
	)

	# Doxygen warning format
	set(
		DOXYGEN_WARN_FORMAT
		"$file:$line: $text"
	)

	# Documentation output directory
	set(
		DOXYGEN_OUTPUT_DIRECTORY
		"${PROJECT_SOURCE_DIR}/docs/"
	)

	# Documentation build target
	doxygen_add_docs(
		documentation
		"${PROJECT_SOURCE_DIR}"
		COMMENT "Generate documentation"
	)

endif()

