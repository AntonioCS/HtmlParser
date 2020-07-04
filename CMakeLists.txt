cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project("Html Parser" DESCRIPTION "Basic html parser" VERSION 0.0.1 LANGUAGES CXX)

option(HTMLPARSER_ENABLE_TESTS "Build the tests" ON)
option(HTMLPARSER_ENABLE_EXAMPLES "Build the examples" ON)

#include(CTest)
#enable_testing()
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(CMAKE_INSTALL_PREFIX $ENV{PROJECTS_INSTALL_DIR})
set(HTMLPARSER_LIBNAME HtmlParser)

if (NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 20)
else()
  if (CMAKE_CXX_STANDARD VERSION_LESS 20)
    message(FATAL_ERROR "CMAKE_CXX_STANDARD of 20 or later required.")
  endif()
endif()
# NOTE: you don't need to fail , if i want a gnu extension let me
if (NOT DEFINED CMAKE_CXX_EXTENSIONS)
  set(CMAKE_CXX_EXTENSIONS OFF)
endif()


#set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)

set(HTMLPARSER_DIR ${PROJECT_SOURCE_DIR})
set(HTMLPARSER_INCLUDES_DIR ${PROJECT_SOURCE_DIR}/include/)
set(HTMLPARSER_SRC_DIR ${PROJECT_SOURCE_DIR}/src/)

if (MSVC)
	add_definitions(-D_UNICODE -DUNICODE -DWIN32_LEAN_AND_MEAN -DNOMINMAX -DWINVER=0x0A00 -D_WIN32_WINNT=0x0A00)
	add_definitions(-D_CRT_SECURE_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_DEPRECATE)
	add_definitions(-D_ATL_SECURE_NO_DEPRECATE -D_SCL_SECURE_NO_WARNINGS)
endif()

#https://cmake.org/cmake/help/v3.14/command/file.html#glob
file(GLOB_RECURSE PROJ_SRC_FILES
	CONFIGURE_DEPENDS
	${HTMLPARSER_SRC_DIR}/*.cpp ${HTMLPARSER_SRC_DIR}/*.h* ${HTMLPARSER_INCLUDES_DIR}/HtmlParser/*.h*
)
#	${HTMLPARSER_SRC_DIR}/*.c* ${HTMLPARSER_SRC_DIR}/*.h*

add_library(${HTMLPARSER_LIBNAME} STATIC
	${PROJ_SRC_FILES}
)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${HTMLPARSER_LIBNAME})

add_library(${HTMLPARSER_LIBNAME}::HtmlParser ALIAS ${HTMLPARSER_LIBNAME})

install(
	TARGETS ${HTMLPARSER_LIBNAME}
	EXPORT ${HTMLPARSER_LIBNAME}-targets

	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(
	EXPORT ${HTMLPARSER_LIBNAME}-targets
	#FILE ${HTMLPARSER_LIBNAME}-targets.cmake
	FILE ${HTMLPARSER_LIBNAME}Config.cmake
	NAMESPACE ${HTMLPARSER_LIBNAME}::
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${HTMLPARSER_LIBNAME}
)

install(
	DIRECTORY ${HTMLPARSER_INCLUDES_DIR}/${HTMLPARSER_LIBNAME}
	DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# Generate the project version information.
#write_basic_package_version_file(
#  #${CMAKE_CURRENT_BINARY_DIR}/htmlparser-config-version.cmake
#  ${CMAKE_CURRENT_BINARY_DIR}/htmlparser-config.cmake
#  VERSION ${PROJECT_VERSION}
#  # htmlparser 0.1.0 and 1.0.0 will not be compatible.
#  COMPATIBILITY SameMajorVersion
#)

# Generate the project config file.
#configure_package_config_file(
#  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/htmlparser/htmlparser-config.cmake.in  ${CMAKE_CURRENT_BINARY_DIR}/htmlparser-config.cmake
#  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/HtmlParser/htmlparser-config.cmake
#  NO_CHECK_REQUIRED_COMPONENTS_MACRO
#)

target_include_directories(
    ${HTMLPARSER_LIBNAME} PUBLIC
    $<BUILD_INTERFACE:${HTMLPARSER_INCLUDES_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_LIBDIR}>
)

#to generate debug lib with d append to it
set_target_properties(${HTMLPARSER_LIBNAME} 
	PROPERTIES 
		DEBUG_POSTFIX _d
)

#To properly set the files in the treeview for MSVC
#if (MSVC)
#	foreach(source IN LISTS PROJ_SRC_FILES)
#		get_filename_component(source_path "${source}" PATH)
#		string(REPLACE "${PROJECT_SOURCE_DIR}/" "" source_path_msvc "${source_path}")    
#		source_group("${source_path_msvc}" FILES "${source}")
#	endforeach()
#endif()

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

#if (HTMLPARSER_ENABLE_EXAMPLES)
#	add_subdirectory(examples)
#endif()