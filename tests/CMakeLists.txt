project(unit_tests DESCRIPTION "Tests for htmlparser")


file(GLOB_RECURSE TEST_FILES_SRC CONFIGURE_DEPENDS
	src/*.c* src/*.h
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
		${HTMLPARSER_INCLUDES_DIR}
)

find_package(Catch2 CONFIG REQUIRED)
target_link_libraries(unit_tests
	PRIVATE
		${HTMLPARSER_LIBNAME}
		Catch2::Catch2
)

#message(Tests binary dir: ${PROJECT_BINARY_DIR})

file(GLOB TEST_FILES_HTML CONFIGURE_DEPENDS
	${CMAKE_CURRENT_SOURCE_DIR}/*.html
)


#copy command does not accept wildcards  - https://cmake.org/cmake/help/latest/manual/cmake.1.html#run-a-command-line-tool
add_custom_command(
	TARGET unit_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${TEST_FILES_HTML}
        "${PROJECT_BINARY_DIR}/"
)


#add_test(unittests unit_tests)

include(Catch)
#catch_discover_tests(unit_tests)
