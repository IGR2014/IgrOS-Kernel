# - Enable Analysis
# Static/runtime analysis for the freestanding kernel target
#
#  Options:
#   IGROS_CXX_CLANG_TIDY		- ClangTidy pass during the build
#   IGROS_CXX_STRICT_WARNINGS	- Extra warnings (conversions, shadowing, casts...), not fatal
#   IGROS_KERNEL_UBSAN		- UndefinedBehaviorSanitizer in trap mode (no runtime library)
#
#  Targets (defined when the tool is found, run on demand):
#   compiler-warnings		- Compiler diagnostics of all C++ sources (recompiled to a temporary directory)
#   clang-tidy			- ClangTidy over compile_commands.json, report file
#   cppcheck			- Cppcheck over compile_commands.json, report file
#   codeql			- CodeQL database of a separate clean build, SARIF report
#   analysis-report		- HTML page with findings of all reports above (reports/index.html)
#
#  All builds, boot test and coverage in one report: config/script/analysis.sh
#
#  Variables:
#   IGROS_REPORTS_DIR		- Reports directory (default: <build>/reports)
#
#  Usage (after the kernel target is created):
#   igros_enable_analysis(<target>)
#
#  Not here on purpose:
#   - ASan / gcov coverage: both need a hosted runtime (libc, mmap, file I/O)
#     which a kernel doesn't have. Coverage is built by host unit tests
#     (tests/, IGROS_TESTS_COVERAGE), kernel ASan would be KASAN (own shadow memory)
#   - SonarCloud / Codacy: run by CI (.github/workflows/sonarcloud.yml), locally
#     the IDE plugin reads compile_commands.json from the build directory
#


# Script mode (cmake -P, used by clang-tidy target): run ClangTidy over C++ sources
# of the compile database and write its output to the report file
if(CMAKE_SCRIPT_MODE_FILE)
	execute_process(
		COMMAND "${RUN_CLANG_TIDY}"
			-p "${BUILD_DIR}"
			-quiet
			-header-filter=.*
			# GCC build: its own warning options aren't Clang ones
			-extra-arg=-Wno-unknown-warning-option
			-export-fixes "${FIXES_FILE}"
			# Only C++: assembly sources are in compile database too
			".*\\.cpp$"
		# Diagnostics and tool failures (crashes, bad flags) in one report
		OUTPUT_FILE "${REPORT_FILE}"
		ERROR_FILE "${REPORT_FILE}"
		RESULT_VARIABLE CLANG_TIDY_RESULT
	)
	# Findings are the report, not a failure
	message(
		STATUS
		"ClangTidy report: ${REPORT_FILE} (exit code ${CLANG_TIDY_RESULT})"
	)
	return()
endif()


# ClangTidy pass
option(
	IGROS_CXX_CLANG_TIDY
	"Add ClangTidy pass to build process"
	OFF
)
# Strict warnings
option(
	IGROS_CXX_STRICT_WARNINGS
	"Extra kernel warnings (conversions, shadowing, casts...), reported but not fatal"
	OFF
)
# UBSan (trap mode)
option(
	IGROS_KERNEL_UBSAN
	"Kernel UndefinedBehaviorSanitizer in trap mode (invalid opcode on UB, no runtime)"
	OFF
)

# Reports directory
set(
	IGROS_REPORTS_DIR
	"${CMAKE_BINARY_DIR}/reports"
	CACHE PATH
	"Directory for analysis reports"
)

# This module (clang-tidy target runs it in script mode)
set(
	IGROS_ANALYSIS_MODULE
	"${CMAKE_CURRENT_LIST_FILE}"
)

# Warnings collector and report generator (Python)
get_filename_component(
	IGROS_ANALYSIS_SCRIPT
	"${CMAKE_CURRENT_LIST_DIR}/../../config/script/analysis.py"
	ABSOLUTE
)

# Compile commands are the input of every analysis target
set(
	IGROS_COMPILE_COMMANDS
	"${CMAKE_BINARY_DIR}/compile_commands.json"
)

# Find tools
find_program(
	IGROS_CLANG_TIDY_EXE
	NAMES clang-tidy
)
find_program(
	IGROS_RUN_CLANG_TIDY_EXE
	NAMES run-clang-tidy
)
find_program(
	IGROS_CPPCHECK_EXE
	NAMES cppcheck
)
find_program(
	IGROS_CODEQL_EXE
	NAMES codeql
)
find_package(
	Python3
	COMPONENTS Interpreter
	QUIET
)
mark_as_advanced(
	IGROS_CLANG_TIDY_EXE
	IGROS_RUN_CLANG_TIDY_EXE
	IGROS_CPPCHECK_EXE
	IGROS_CODEQL_EXE
)


# Enable analysis for target
function(igros_enable_analysis TARGET)

	# Only C++: assembler sources get toolchain assembler flags
	set(
		CXX_ONLY
		"$<COMPILE_LANGUAGE:CXX>"
	)

	# ClangTidy pass during the build (checks come from .clang-tidy, if any)
	if(IGROS_CXX_CLANG_TIDY)
		if(NOT IGROS_CLANG_TIDY_EXE)
			message(
				FATAL_ERROR
				"IGROS_CXX_CLANG_TIDY is ON, but clang-tidy is not found!"
			)
		endif()
		set_target_properties(
			${TARGET}
			PROPERTIES
			CXX_CLANG_TIDY "${IGROS_CLANG_TIDY_EXE};-use-color;-header-filter=.*;-extra-arg=-Wno-unknown-warning-option"
		)
		message(
			STATUS
			"Analysis: ClangTidy pass enabled"
		)
	endif()

	# Strict warnings (toolchain already has -Wall -Wextra -pedantic -Werror)
	if(IGROS_CXX_STRICT_WARNINGS)
		set(
			STRICT_WARNINGS
			shadow
			non-virtual-dtor
			old-style-cast
			cast-align
			cast-qual
			conversion
			sign-conversion
			null-dereference
			double-promotion
			format=2
			implicit-fallthrough
			undef
			redundant-decls
			missing-declarations
			extra-semi
			zero-as-null-pointer-constant
			suggest-override
			overloaded-virtual
			deprecated-copy-dtor
			vla
			alloca
			# Kernel stacks are small (double fault and IST stacks are a few pages)
			frame-larger-than=1024
		)
		# Compiler specific (no GCC useless-cast: casts to width typedefs are useless
		# on one architecture only)
		if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
			list(
				APPEND STRICT_WARNINGS
				duplicated-cond
				duplicated-branches
				logical-op
			)
		elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
			list(
				APPEND STRICT_WARNINGS
				shadow-all
				comma
				extra-semi-stmt
				unreachable-code-aggressive
				loop-analysis
				missing-variable-declarations
				unused-member-function
				unused-template
			)
		endif()
		# Enabled as warnings, not errors: report without breaking the -Werror build
		# (-Wno-error=format=2 isn't valid, format is covered by -Wall errors already;
		# valued warning in -Wno-error=: GCC keeps "=" in its name, Clang doesn't)
		set(
			STRICT_NO_ERROR
			${STRICT_WARNINGS}
		)
		list(
			REMOVE_ITEM STRICT_NO_ERROR
			format=2
		)
		if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
			set(
				VALUE_SUFFIX
				"="
			)
		else()
			set(
				VALUE_SUFFIX
				""
			)
		endif()
		list(
			TRANSFORM STRICT_NO_ERROR
			REPLACE "=.*$" "${VALUE_SUFFIX}"
		)
		list(
			TRANSFORM STRICT_WARNINGS
			PREPEND "-W"
		)
		list(
			TRANSFORM STRICT_NO_ERROR
			PREPEND "-Wno-error="
		)
		target_compile_options(
			${TARGET}
			PRIVATE
			"$<${CXX_ONLY}:${STRICT_WARNINGS};${STRICT_NO_ERROR}>"
		)
		message(
			STATUS
			"Analysis: strict warnings enabled"
		)
	endif()

	# UBSan in trap mode: a check failure executes ud2 (#UD exception, vector 6)
	# instead of calling __ubsan_handle_* from libubsan (kernel has no runtime)
	if(IGROS_KERNEL_UBSAN)
		if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
			# vptr check needs RTTI (kernel is -fno-rtti), function check needs type
			# metadata in front of every function (breaks raw handler/entry addresses)
			set(
				UBSAN_FLAGS
				-fsanitize=undefined
				-fsanitize-trap=undefined
				-fno-sanitize=vptr,function
			)
		elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
			# GCC 11 has no -fsanitize-trap=, trap-on-error is the same for every check
			set(
				UBSAN_FLAGS
				-fsanitize=undefined
				-fsanitize-undefined-trap-on-error
			)
		else()
			message(
				FATAL_ERROR
				"IGROS_KERNEL_UBSAN: unsupported compiler ${CMAKE_CXX_COMPILER_ID}!"
			)
		endif()
		target_compile_options(
			${TARGET}
			PRIVATE
			"$<${CXX_ONLY}:${UBSAN_FLAGS}>"
		)
		message(
			STATUS
			"Analysis: UBSan (trap mode) enabled"
		)
	endif()

	# Reports directory
	file(
		MAKE_DIRECTORY
		"${IGROS_REPORTS_DIR}"
	)

	# Compiler diagnostics of all kernel sources (build output isn't kept, so recompile)
	if(Python3_Interpreter_FOUND)
		add_custom_target(
			compiler-warnings
			COMMAND "${Python3_EXECUTABLE}" "${IGROS_ANALYSIS_SCRIPT}" warnings
				--build-dir "${CMAKE_BINARY_DIR}"
				--output "${IGROS_REPORTS_DIR}/compiler-warnings.txt"
			WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
			COMMENT "Collecting compiler warnings of kernel sources"
			VERBATIM
			USES_TERMINAL
		)
	endif()

	# ClangTidy report over all kernel sources
	if(IGROS_RUN_CLANG_TIDY_EXE)
		add_custom_target(
			clang-tidy
			COMMAND ${CMAKE_COMMAND}
				-DRUN_CLANG_TIDY=${IGROS_RUN_CLANG_TIDY_EXE}
				-DBUILD_DIR=${CMAKE_BINARY_DIR}
				-DREPORT_FILE=${IGROS_REPORTS_DIR}/clang-tidy-report.txt
				-DFIXES_FILE=${IGROS_REPORTS_DIR}/clang-tidy-fixes.yaml
				-P "${IGROS_ANALYSIS_MODULE}"
			WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
			COMMENT "Running ClangTidy over kernel sources"
			VERBATIM
			USES_TERMINAL
		)
	endif()

	# Cppcheck report over all kernel sources (one run: report isn't overwritten per file)
	if(IGROS_CPPCHECK_EXE)
		# Kernel bitness for type sizes
		if(CMAKE_SYSTEM_PROCESSOR STREQUAL "i386")
			set(
				CPPCHECK_PLATFORM
				unix32
			)
		else()
			set(
				CPPCHECK_PLATFORM
				unix64
			)
		endif()
		# Incremental analysis cache
		set(
			CPPCHECK_BUILD_DIR
			"${CMAKE_BINARY_DIR}/cppcheck"
		)
		file(
			MAKE_DIRECTORY
			"${CPPCHECK_BUILD_DIR}"
		)
		add_custom_target(
			cppcheck
			COMMAND "${IGROS_CPPCHECK_EXE}"
				--project=${IGROS_COMPILE_COMMANDS}
				--cppcheck-build-dir=${CPPCHECK_BUILD_DIR}
				--platform=${CPPCHECK_PLATFORM}
				--enable=warning,style,performance,portability
				--inconclusive
				--check-level=exhaustive
				--inline-suppr
				--suppress=missingIncludeSystem
				--suppress=unmatchedSuppression
				--quiet
				--template=gcc
				--output-file=${IGROS_REPORTS_DIR}/cppcheck-report.txt
			COMMAND ${CMAKE_COMMAND} -E echo "Cppcheck report: ${IGROS_REPORTS_DIR}/cppcheck-report.txt"
			WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
			COMMENT "Running Cppcheck over kernel sources"
			VERBATIM
			USES_TERMINAL
		)
	endif()

	# CodeQL: database is extracted from a separate clean build directory
	# (building this build directory from its own target would recurse into it)
	if(IGROS_CODEQL_EXE)
		set(
			CODEQL_DIR
			"${CMAKE_BINARY_DIR}/codeql"
		)
		set(
			CODEQL_SARIF
			"${IGROS_REPORTS_DIR}/codeql-results.sarif"
		)
		add_custom_target(
			codeql
			COMMAND ${CMAKE_COMMAND} -E rm -rf "${CODEQL_DIR}"
			# Configure outside tracing: compiler checks aren't kernel code
			COMMAND ${CMAKE_COMMAND}
				-S "${CMAKE_SOURCE_DIR}"
				-B "${CODEQL_DIR}/build"
				-G "${CMAKE_GENERATOR}"
				-DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
				-DCMAKE_BUILD_TYPE=$<CONFIG>
			# CodeQL must see every compiler invocation: no ccache
			COMMAND ${CMAKE_COMMAND} -E env CCACHE_DISABLE=1
				"${IGROS_CODEQL_EXE}" database create "${CODEQL_DIR}/db"
				--language=c-cpp
				--source-root=${CMAKE_SOURCE_DIR}
				"--command=${CMAKE_COMMAND} --build ${CODEQL_DIR}/build"
			COMMAND "${IGROS_CODEQL_EXE}" database analyze "${CODEQL_DIR}/db"
				codeql/cpp-queries:codeql-suites/cpp-security-and-quality.qls
				--download
				--format=sarif-latest
				--output=${CODEQL_SARIF}
			COMMAND ${CMAKE_COMMAND} -E echo "CodeQL report: ${CODEQL_SARIF}"
			WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
			COMMENT "Running CodeQL analysis (separate clean build)"
			VERBATIM
			USES_TERMINAL
		)
	endif()

	# HTML page with findings of reports already in reports directory
	if(Python3_Interpreter_FOUND)
		add_custom_target(
			analysis-report
			COMMAND "${Python3_EXECUTABLE}" "${IGROS_ANALYSIS_SCRIPT}" report
				--source-dir "${CMAKE_SOURCE_DIR}"
				--output "${IGROS_REPORTS_DIR}/index.html"
				"${CMAKE_SYSTEM_PROCESSOR}=${CMAKE_BINARY_DIR}"
			WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
			COMMENT "Generating analysis report: ${IGROS_REPORTS_DIR}/index.html"
			VERBATIM
			USES_TERMINAL
		)
	endif()

endfunction()
