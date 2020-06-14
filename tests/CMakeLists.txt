project(unit_tests DESCRIPTION "Tests for htmlparser")

include(CTest)
enable_testing()

file(GLOB_RECURSE TEST_FILES_SRC 
	CONFIpp src/*.h
)

add_executable(unit_tests ${TEST_FILES_SRC})

target_compile_options(unit_tests
 PRIVATE
	$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
		-Werror -Wall -Wextra
	>
	$<$<CXX_COMPILER_ID:MSVC>:
		/W4
	>
)

target_include_directories(unit_tests
	PUBLIC
		${HTMLPARSER_SRC_DIR}
)

find_package(Catch2 CONFIG REQUIRED)
target_link_libraries(unit_tests
	PRIVATE
		${HTMLPARSER_LIBNAME}
		Catch2::Catch2
)

add_custom_command(
	TARGET unit_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/basic.html"
        "${PROJECT_SOURCE_DIR}/build"
)

add_test(unittests unit_tests)

include(Catch)
catch_discover_tests(unit_tests)
