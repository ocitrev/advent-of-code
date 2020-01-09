cmake_minimum_required(VERSION 3.10)

project(AdventOfCOde)

add_library(project_warnings INTERFACE)
if(MSVC)
  target_compile_options(project_warnings INTERFACE /W4)
else()
  target_compile_options(project_warnings INTERFACE
    -Wall
    -Wextra
    -Werror
    -Wshadow
    -Wold-style-cast
    -Wcast-align
    -Wunused
    -Woverloaded-virtual
    -Wpedantic
    -Wconversion
    -Wsign-conversion
    )
endif()

add_library(common STATIC common/input.cpp common/main.cpp common/intcode.cpp)
add_executable(day01 day01/day01.cpp)
add_executable(day02 day02/day02.cpp)
add_executable(day05 day05/day05.cpp)
add_executable(day07 day07/day07.cpp)
add_executable(day09 day09/day09.cpp)
add_executable(day16 day16/day16.cpp)
add_executable(day17 day17/day17.cpp)
add_executable(day19 day19/day19.cpp)

target_link_libraries(common PUBLIC project_warnings)
target_link_libraries(day01 PRIVATE common)
target_link_libraries(day02 PRIVATE common)
target_link_libraries(day05 PRIVATE common)
target_link_libraries(day07 PRIVATE common)
target_link_libraries(day09 PRIVATE common)
target_link_libraries(day16 PRIVATE common)
target_link_libraries(day17 PRIVATE common)
target_link_libraries(day19 PRIVATE common)

set_target_properties(common day01 day02 day05 day07 day09 day16 day17 day19 PROPERTIES
    CXX_STANDARD 20
    CXX_EXTENSIONS OFF)
