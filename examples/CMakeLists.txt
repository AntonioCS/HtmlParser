project(examples DESCRIPTION "Examples on using htmlparser lib")

file(GLOB_RECURSE EXAMPLE_SRC CONFIGURE_DEPENDS
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.h*
)

add_executable(examples ${EXAMPLE_SRC})

target_compile_options(examples
 PRIVATE
	$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
		-Werror -Wall -Wextra>
	$<$<CXX_COMPILER_ID:MSVC>:
		/W4>
)

target_link_libraries(examples
	PRIVATE 
		${HTMLPARSER_LIBNAME}
)

target_include_directories(examples
	PRIVATE
		${HTMLPARSER_SRC_DIR}
)

file(GLOB EXAMPLES_FILES_HTML CONFIGURE_DEPENDS
	${CMAKE_CURRENT_SOURCE_DIR}/*.html
)

add_custom_command(
	TARGET examples POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${EXAMPLES_FILES_HTML}"
        "${PROJECT_BINARY_DIR}/"
)
