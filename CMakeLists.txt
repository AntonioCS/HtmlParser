#In build dir: 
#cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET="x64-windows"

cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project("Html parser" DESCRIPTION "Html parser" VERSION 0.0.1 LANGUAGES CXX)

include(CTest)
enable_testing()

set(LIBNAME htmlparser)

# Add local project module path so CMake includes custom CMake modules.
#list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
	add_definitions(-D_UNICODE -DUNICODE -DWIN32_LEAN_AND_MEAN -DNOMINMAX -DWINVER=0x0A00 -D_WIN32_WINNT=0x0A00)
	add_definitions(-D_CRT_SECURE_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_DEPRECATE)
	add_definitions(-D_ATL_SECURE_NO_DEPRECATE -D_SCL_SECURE_NO_WARNINGS)
endif()


#https://cmake.org/cmake/help/v3.14/command/file.html#glob
file(GLOB_RECURSE PROJ_SRC_FILES
	CONFIGURE_DEPENDS
	src/*.c* src/*.h*
)

add_library(
	${LIBNAME}
	${PROJ_SRC_FILES}
)

target_include_directories(${LIBNAME}
	PUBLIC ${PROJECT_SOURCE_DIR}/src/
)

#add_custom_command(
#	TARGET ${LIBNAME} POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different
#        "${PROJECT_SOURCE_DIR}/src/*.h"
#        "${PROJECT_SOURCE_DIR}/build/HtmlParser"
#)

#to generate debug lib with d append to it
set_target_properties(${LIBNAME} PROPERTIES DEBUG_POSTFIX _d)

#To properly set the files in the treeview for MSVC
foreach(source IN LISTS PROJ_SRC_FILES)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "${PROJECT_SOURCE_DIR}/" "" source_path_msvc "${source_path}")    
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()

#Taken from https://github.com/erichschroeter/cmake-catch2-example/blob/master/CMakeLists.txt
target_compile_options(${LIBNAME}
	 PRIVATE
		 $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
			 -Werror -Wall -Wextra>
		$<$<CXX_COMPILER_ID:MSVC>:
			 /W4>
)

file(GLOB_RECURSE TEST_FILES 
	CONFIGURE_DEPENDS 
	${PROJECT_SOURCE_DIR}/tests/*.cpp ${PROJECT_SOURCE_DIR}/tests/*.h
)

add_executable(unit_tests
	${TEST_FILES}
)

target_compile_options(unit_tests
	 PRIVATE
		 $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
			 -Werror -Wall -Wextra>
		$<$<CXX_COMPILER_ID:MSVC>:
			 /W4>
)


target_include_directories(unit_tests
	PUBLIC
		tests/
		src/
)

find_package(Catch2 CONFIG REQUIRED)
target_link_libraries(unit_tests 
	PRIVATE 
		Catch2::Catch2
		${LIBNAME}
)

add_custom_command(
	TARGET unit_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/tests/basic.html"
        "${PROJECT_SOURCE_DIR}/build"
)

add_test(unittests unit_tests)

include(Catch)
catch_discover_tests(unit_tests)

add_subdirectory(examples/youtube.com)
