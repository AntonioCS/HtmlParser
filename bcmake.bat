@echo off
@setlocal EnableExtensions EnableDelayedExpansion	
REM NOTE: Run this in admin mode if it doesn't work

SET rootDir=%~dp0%
SET rootDir=%rootDir:\=/%
SET srcDir=%rootDir%
SET srcCodeDir=%rootDir%src
SET testDir=%rootDir%tests
SET builDir=%rootDir%build
SET toolChainPath=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
SET vtt=x64-windows

IF not exist "%srcCodeDir%" (
	REM You need to use ^ for special chars
	@mkdir "%srcCodeDir%"
	(
		echo #include ^<iostream^>
		echo:
		echo int main^(int argc, char** argv^) {
		echo:
		echo    std::cout ^<^< "Hello World!\n";
		echo:
		echo }
	) > "%srcCodeDir%/main.cpp"
)

IF not exist "%testDir%" (
	REM You need to use ^ for special chars
	@mkdir "%testDir%"
	(
		echo #define CATCH_CONFIG_MAIN
		echo #include ^<catch2/catch.hpp^>		
	) > "%testDir%/main.cpp"
	
	(
		echo #include ^<catch2/catch.hpp^>
		echo:
		echo TEST_CASE^("Test1", "[Example]"^) {
		echo:
		echo }
	) > "%testDir%/ExampleTest.cpp"
)

IF exist "%builDir%/*.sln" (
	SET /p delete_project="Solution present. Delete it?[yes/no default yes]: "
	IF NOT DEFINED delete_project (
		echo Removing directory
		rmdir /S "%builDir%"
	)
)

IF not exist "%builDir%" mkdir "%builDir%"

REM Specifying the source and build dir does not work for whatever reason
REM cmake -S "%srcDir%" -B "%builDir%" -DCMAKE_TOOLCHAIN_FILE="%toolChainPath%" -DVCPKG_TARGET_TRIPLET="%vtt%"
IF not exist "%rootDir%/CMakeLists.txt" (		
	:pname
	SET /p project_name="Enter Project name: "
	
	IF NOT DEFINED project_name (
		echo The name of the project is required
		GOTO pname		
	)

	:pdesc
	SET /p project_desc="Enter Project description: "
	IF NOT DEFINED project_desc (
		echo A description is required
		GOTO pdesc
	)	

	SET /p enable_testing="Enable testing?[yes/no, default yes]: "

	IF NOT DEFINED enable_testing SET enable_testing=yes

	SET /p exe_name="Enter exe name(leave empty to use Project Name): "
	IF NOT DEFINED exe_name SET exe_name=!project_name!
	
	REM echo !project_name!
	REM echo !project_desc!
	REM echo !enable_testing!
	REM echo !exe_name!

	(
		echo cmake_minimum_required^(VERSION 3.15 FATAL_ERROR^)
		echo project^("!project_name!" DESCRIPTION "!project_desc!" VERSION 0.0.1 LANGUAGES CXX^)
		echo:
		echo include^(CTest^)
		echo:
		echo set^(EXENAME !exe_name!^)
		echo set^(LIBNAME !exe_name!lib)
		echo:
		echo set^(CMAKE_CXX_STANDARD 20^)
		echo set^(CMAKE_CXX_STANDARD_REQUIRED ON^)
		echo set^(CMAKE_CXX_EXTENSIONS OFF^)
		echo:
		echo if ^(MSVC^)
		echo 	add_definitions^(-D_UNICODE -DUNICODE -DWIN32_LEAN_AND_MEAN -DNOMINMAX -DWINVER=0x0A00 -D_WIN32_WINNT=0x0A00^)
		echo 	add_definitions^(-D_CRT_SECURE_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_DEPRECATE^)
		echo 	add_definitions^(-D_ATL_SECURE_NO_DEPRECATE -D_SCL_SECURE_NO_WARNINGS^)
		echo endif^(^)
		echo:		
		echo file^(GLOB_RECURSE PROJ_SRC_FILES
		echo 	CONFIGURE_DEPENDS
		echo 	src/*.c* src/*.h*
		echo ^)
		echo:
		echo #To properly set the files in the treeview for MSVC
		echo foreach^(source IN LISTS PROJ_SRC_FILES^)
		echo 	get_filename_component^(source_path "${source}" PATH^)
		echo 	string^(REPLACE "${PROJECT_SOURCE_DIR}/" "" source_path_msvc "${source_path}"^)
		echo 	source_group^("${source_path_msvc}" FILES "${source}"^)
		echo endforeach^(^)
		echo:
		IF !enable_testing! == yes (
			echo enable_testing^(^)	
			echo file^(GLOB_RECURSE TEST_FILES 
			echo 	CONFIGURE_DEPENDS 
			echo 	${PROJECT_SOURCE_DIR}/tests/*.cpp
			echo ^)
			echo: 
			echo add_executable^(tests
			echo 	${TEST_FILES}
			echo ^)
			echo:
			echo target_include_directories^(tests
			echo 	PUBLIC
			echo 		tests/
			echo 		src/
			echo ^)
			echo:
			echo find_package^(Catch2 CONFIG REQUIRED^)
			echo target_link_libraries^(tests 
			echo 	PRIVATE 
			echo 		Catch2::Catch2
			echo 		${LIBNAME}
			echo ^)
		)
	) > "%rootDir%/CMakeLists.txt"	
)

cd "%builDir%"
cmake .. -DCMAKE_TOOLCHAIN_FILE="%toolChainPath%" -DVCPKG_TARGET_TRIPLET="%vtt%"
cd "%rootDir%"
pause
