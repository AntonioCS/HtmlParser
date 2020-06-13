project(examples_youtube DESCRIPTION "Use htmlparser lib in html from youtube.com")

file(GLOB_RECURSE EXAMPLE_YOUTUBE_SRC
	CONFIGURE_DEPENDS
	src/*.c* src/*.h*
)

add_executable(${PROJECT_NAME} ${EXAMPLE_YOUTUBE_SRC})

target_compile_options(${PROJECT_NAME}
 PRIVATE
	$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
		-Werror -Wall -Wextra>
	$<$<CXX_COMPILER_ID:MSVC>:
		/W4>
)

add_library(HtmlParser SHARED IMPORTED)

target_link_libraries(
	${PROJECT_NAME} 
	debug ${PROJECT_BINARY_DIR}/../../Debug/htmlparser_d.lib 
	optimized ${PROJECT_BINARY_DIR}/../../Release/htmlparser.lib 
)

include_directories(
	${PROJECT_SOURCE_DIR}/../../src/
)

add_custom_command(
	TARGET examples_youtube POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/youtube.html"
        "${PROJECT_BINARY_DIR}/"
)
