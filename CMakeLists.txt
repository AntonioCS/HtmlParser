cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project("Html parser" DESCRIPTION "Html parser" VERSION 0.0.1 LANGUAGES CXX)

option(HTMLPARSER_ENABLE_TESTS "Build the tests" ON)

set(HTMLPARSER_LIBNAME htmlparser)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(HTMLPARSER_DIR ${PROJECT_SOURCE_DIR})
set(HTMLPARSER_SRC_DIR ${PROJECT_SOURCE_DIR}/src/)

if (MSVC)
	add_definitions(-D_UNICODE -DUNICODE -DWIN32_LEAN_AND_MEAN -DNOMINMAX -DWINVER=0x0A00 -D_WIN32_WINNT=0x0A00)
	add_definitions(-D_CRT_SECURE_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_DEPRECATE)
	add_definitions(-D_ATL_SECURE_NO_DEPRECATE -D_SCL_SECURE_NO_WARNINGS)
endif()

#https://cmake.org/cmake/help/v3.14/command/file.html#glob
file(GLOB_RECURSE PROJ_SRC_FILES
	CONFIGURE_DEPENDS
	${HTMLPARSER_SRC_DIR}/*.c* ${HTMLPARSER_SRC_DIR}/*.h*
)

add_library(${HTMLPARSER_LIBNAME}
	${PROJ_SRC_FILES}
)

target_include_directories(${HTMLPARSER_LIBNAME}
	PUBLIC 
		${HTMLPARSER_SRC_DIR}
)

#to generate debug lib with d append to it
set_target_properties(${HTMLPARSER_LIBNAME} 
	PROPERTIES 
		DEBUG_POSTFIX _d
)

#To properly set the files in the treeview for MSVC
foreach(source IN LISTS PROJ_SRC_FILES)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "${PROJECT_SOURCE_DIR}/" "" source_path_msvc "${source_path}")    
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()

#Taken from https://github.com/erichschroeter/cmake-catch2-example/blob/master/CMakeLists.txt
target_compile_options(${HTMLPARSER_LIBNAME}
	 PRIVATE
		 $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
			 -Werror -Wall -Wextra>
		$<$<CXX_COMPILER_ID:MSVC>:
			 /W4>
)

if (HTMLPARSER_ENABLE_TESTS)
	add_subdirectory(tests)
endif()
