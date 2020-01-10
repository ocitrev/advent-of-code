cmake_minimum_required(VERSION 3.15)

project(AdventOfCOde)

add_library(project_warnings INTERFACE)
if(MSVC)
  target_compile_options(project_warnings INTERFACE
    /W4
    /WX
    /diagnostics:caret
    /premissive-
  )
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

add_library(common STATIC
  common/common.hpp
  common/main.cpp
  common/main.hpp
  common/intcode.cpp
  common/intcode.hpp
)
target_compile_features(common PUBLIC cxx_std_20)
target_link_libraries(common PUBLIC project_warnings)

add_executable(day01 day01/day01.cpp day01/day01.hpp)
add_executable(day02 day02/day02.cpp day02/day02.hpp)
add_executable(day05 day05/day05.cpp day05/day05.hpp)
add_executable(day07 day07/day07.cpp day07/day07.hpp)
add_executable(day09 day09/day09.cpp day09/day09.hpp)
add_executable(day16 day16/day16.cpp day16/day16.hpp)
add_executable(day17 day17/day17.cpp day17/day17.hpp)
add_executable(day19 day19/day19.cpp day19/day19.hpp)

target_link_libraries(day01 PRIVATE common)
target_link_libraries(day02 PRIVATE common)
target_link_libraries(day05 PRIVATE common)
target_link_libraries(day07 PRIVATE common)
target_link_libraries(day09 PRIVATE common)
target_link_libraries(day16 PRIVATE common)
target_link_libraries(day17 PRIVATE common)
target_link_libraries(day19 PRIVATE common)
