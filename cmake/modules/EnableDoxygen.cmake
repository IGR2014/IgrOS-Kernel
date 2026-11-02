# - Doxygen documentation
# Adds "documentation" target: HTML docs of kernel sources (architecture of this build)
# in <build dir>/docs/html, README as main page. Doxygen warnings (documentation errors
# and incomplete parameter docs; undocumented code isn't one) go to
# ${IGROS_REPORTS_DIR}/doxygen-warnings.txt for the analysis report.
#


# Search Doxygen package (Graphviz dot for diagrams, if installed)
find_package(
	Doxygen
	OPTIONAL_COMPONENTS dot
)

# If Doxygen found
if(DOXYGEN_FOUND)

	# Project name and brief come from project(); README is the main page
	set(
		DOXYGEN_USE_MDFILE_AS_MAINPAGE
		"${PROJECT_SOURCE_DIR}/README.md"
	)
	# README headings get GitHub anchors (README links to "#analysis")
	set(
		DOXYGEN_MARKDOWN_ID_STYLE
		GITHUB
	)
	set(
		DOXYGEN_FILE_PATTERNS
		*.hpp
		*.cpp
	)
	set(
		DOXYGEN_RECURSIVE
		YES
	)
	# Paths relative to sources (same file names in several architecture directories)
	set(
		DOXYGEN_STRIP_FROM_PATH
		"${PROJECT_SOURCE_DIR}"
	)
	set(
		DOXYGEN_STRIP_FROM_INC_PATH
		"${PROJECT_SOURCE_DIR}"
	)

	# Kernel internals are documented too
	set(
		DOXYGEN_EXTRACT_PRIVATE
		YES
	)
	set(
		DOXYGEN_EXTRACT_STATIC
		YES
	)
	set(
		DOXYGEN_EXTRACT_ANON_NSPACES
		YES
	)

	# Architecture of this build (code under IGROS_ARCH_* conditions)
	set(
		DOXYGEN_PREDEFINED
		"IGROS_KERNEL"
		"IGROS_ARCH_${CMAKE_SYSTEM_PROCESSOR}"
		"IGROS_ARCH=${CMAKE_SYSTEM_PROCESSOR}"
		"IGROS_GIT_VERSION=\"${IGROS_GIT_VERSION}\""
	)

	# HTML with sources, navigation tree and diagrams
	set(
		DOXYGEN_GENERATE_HTML
		YES
	)
	set(
		DOXYGEN_GENERATE_TREEVIEW
		YES
	)
	# doxygen-awesome-css theme (config/doxygen/doxygen-awesome, MIT), sidebar layout, dark
	# mode toggle (dark by default), colors of the analysis site (config/doxygen/igros.css)
	set(
		IGROS_DOXYGEN_THEME_DIR
		"${PROJECT_SOURCE_DIR}/config/doxygen"
	)
	set(
		DOXYGEN_HTML_HEADER
		"${IGROS_DOXYGEN_THEME_DIR}/header.html"
	)
	set(
		DOXYGEN_HTML_EXTRA_STYLESHEET
		"${IGROS_DOXYGEN_THEME_DIR}/doxygen-awesome/doxygen-awesome.css"
		"${IGROS_DOXYGEN_THEME_DIR}/doxygen-awesome/doxygen-awesome-sidebar-only.css"
		"${IGROS_DOXYGEN_THEME_DIR}/doxygen-awesome/doxygen-awesome-sidebar-only-darkmode-toggle.css"
		"${IGROS_DOXYGEN_THEME_DIR}/igros.css"
	)
	set(
		DOXYGEN_HTML_EXTRA_FILES
		"${IGROS_DOXYGEN_THEME_DIR}/doxygen-awesome/doxygen-awesome-darkmode-toggle.js"
	)
	# Theme handles dark mode itself (Doxygen's own one would conflict)
	set(
		DOXYGEN_HTML_COLORSTYLE
		LIGHT
	)
	set(
		DOXYGEN_DISABLE_INDEX
		NO
	)
	set(
		DOXYGEN_FULL_SIDEBAR
		NO
	)
	set(
		DOXYGEN_SOURCE_BROWSER
		YES
	)
	set(
		DOXYGEN_HAVE_DOT
		${DOXYGEN_DOT_FOUND}
	)
	set(
		DOXYGEN_OUTPUT_DIRECTORY
		"${CMAKE_BINARY_DIR}/docs"
	)

	# Warnings: documentation errors only, in compiler format for the analysis report
	set(
		DOXYGEN_QUIET
		YES
	)
	set(
		DOXYGEN_WARN_IF_UNDOCUMENTED
		NO
	)
	set(
		DOXYGEN_WARN_IF_DOC_ERROR
		YES
	)
	set(
		DOXYGEN_WARN_IF_INCOMPLETE_DOC
		YES
	)
	set(
		DOXYGEN_WARN_NO_PARAMDOC
		NO
	)
	set(
		DOXYGEN_WARN_FORMAT
		"$file:$line: $text"
	)
	set(
		DOXYGEN_WARN_LOGFILE
		"${IGROS_REPORTS_DIR}/doxygen-warnings.txt"
	)
	file(
		MAKE_DIRECTORY
		"${IGROS_REPORTS_DIR}"
	)

	# Documentation build target (kernel sources only: no tests, scripts or build output)
	doxygen_add_docs(
		documentation
		"${PROJECT_SOURCE_DIR}/README.md"
		"${PROJECT_SOURCE_DIR}/CONTRIBUTING.md"
		"${PROJECT_SOURCE_DIR}/CODE_OF_CONDUCT.md"
		"${PROJECT_SOURCE_DIR}/config/doxygen/namespaces.dox"
		"${PROJECT_SOURCE_DIR}/arch"
		"${PROJECT_SOURCE_DIR}/dev"
		"${PROJECT_SOURCE_DIR}/drivers"
		"${PROJECT_SOURCE_DIR}/klib"
		"${PROJECT_SOURCE_DIR}/mem"
		"${PROJECT_SOURCE_DIR}/multiboot"
		"${PROJECT_SOURCE_DIR}/platform"
		"${PROJECT_SOURCE_DIR}/sys"
		"${PROJECT_SOURCE_DIR}/kmain.cpp"
		WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
		COMMENT "Generating documentation: ${CMAKE_BINARY_DIR}/docs/html/index.html"
	)

endif()
