cmake_minimum_required(VERSION 3.15)

project(AdventOfCOde)

add_library(project_warnings INTERFACE)

if(MSVC)
	target_compile_options(project_warnings INTERFACE
		/W4
		/WX
		/diagnostics:caret
		/permissive-
	)
else()
	target_compile_options(project_warnings INTERFACE
		-Wall
		-Wcast-align
		-Wconversion
		-Wdouble-promotion
		-Werror
		-Wextra
		-Wformat=2
		-Wnon-virtual-dtor
		-Wnull-dereference
		-Wold-style-cast
		-Woverloaded-virtual
		-Wpedantic
		-Wshadow
		-Wsign-conversion
		-Wunused
	)
endif()

# setup le force include
if(MSVC)
	add_compile_options("/FI${PROJECT_SOURCE_DIR}/common/common.hpp" /utf-8)
else()
	add_compile_options(-include "${PROJECT_SOURCE_DIR}/common/common.hpp")
endif()


add_library(common STATIC
	common/main.cpp
	common/main.hpp
	common/intcode.cpp
	common/intcode.hpp
	common/string.cpp
	common/string.hpp
)

target_compile_features(common PUBLIC cxx_std_20)
target_link_libraries(common PUBLIC project_warnings)

add_executable(day01 day01.cpp day01.hpp)
add_executable(day02 day02.cpp day02.hpp)
add_executable(day03 day03.cpp day03.hpp)
add_executable(day04 day04.cpp day04.hpp)
add_executable(day05 day05.cpp day05.hpp)
add_executable(day06 day06.cpp day06.hpp)
add_executable(day07 day07.cpp day07.hpp)
add_executable(day08 day08.cpp day08.hpp)
add_executable(day09 day09.cpp day09.hpp)
add_executable(day12 day12.cpp day12.hpp)
add_executable(day16 day16.cpp day16.hpp)
add_executable(day17 day17.cpp day17.hpp)
add_executable(day19 day19.cpp day19.hpp)

target_link_libraries(day01 PRIVATE common)
target_link_libraries(day02 PRIVATE common)
target_link_libraries(day03 PRIVATE common)
target_link_libraries(day04 PRIVATE common)
target_link_libraries(day05 PRIVATE common)
target_link_libraries(day06 PRIVATE common)
target_link_libraries(day07 PRIVATE common)
target_link_libraries(day08 PRIVATE common)
target_link_libraries(day09 PRIVATE common)
target_link_libraries(day12 PRIVATE common)
target_link_libraries(day16 PRIVATE common)
target_link_libraries(day17 PRIVATE common)
target_link_libraries(day19 PRIVATE common)
