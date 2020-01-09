cmake_minimum_required(VERSION 3.10)

project(AdventOfCOde)

add_library(common STATIC common/input.cpp common/main.cpp common/intcode.cpp)
add_executable(day01 day01/day01.cpp)
add_executable(day02 day02/day02.cpp)
add_executable(day05 day05/day05.cpp)
add_executable(day07 day07/day07.cpp)
add_executable(day09 day09/day09.cpp)
add_executable(day16 day16/day16.cpp)
add_executable(day17 day17/day17.cpp)
add_executable(day19 day19/day19.cpp)

target_link_libraries(day01 common)
target_link_libraries(day02 common)
target_link_libraries(day05 common)
target_link_libraries(day07 common)
target_link_libraries(day09 common)
target_link_libraries(day16 common)
target_link_libraries(day17 common)
target_link_libraries(day19 common)

set_target_properties(common day01 day02 day05 day07 day09 day16 day17 day19 PROPERTIES
    CXX_STANDARD 20
    CXX_EXTENSIONS OFF)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(common PUBLIC /MP /WX /W4)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(common PUBLIC -Wall -Wextra -Werror)
endif()
