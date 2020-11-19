cmake_minimum_required(VERSION 3.15)
project(AdventOfCOde)

if(NOT MSVC)
  find_program(CLANG_TIDY NAMES "clang-tidy")

  if(CLANG_TIDY)
    set(CLANG_TIDY_CHECK_LIST
      cppcoreguidelines-*
      -cppcoreguidelines-avoid-magic-numbers
      modernize-*
      -modernize-use-trailing-return-type
      performance-*
      readability-*
      -readability-braces-around-statements # ne fonctionne pas avec le style Alleman
      -readability-magic-numbers
      )
    list(JOIN CLANG_TIDY_CHECK_LIST "," CLANG_TIDY_CHECKS)
    set(CMAKE_CXX_CLANG_TIDY
      ${CLANG_TIDY}
      -checks=${CLANG_TIDY_CHECKS}
      -header-filter=.*
      --format-style=file
      --warnings-as-errors=*
      )
  endif()
endif()

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

add_library(common STATIC
  src/combinations.hpp
  src/intcode.cpp
  src/intcode.hpp
  src/point.hpp
  src/string.cpp
  src/string.hpp
  src/terminal.cpp
  src/terminal.hpp
)

target_compile_definitions(common PUBLIC
  _USE_MATH_DEFINES
  NOMINMAX
  WIN32_LEAN_AND_MEAN
)
target_compile_features(common PUBLIC cxx_std_20)
target_link_libraries(common PUBLIC project_warnings)

# fmt library
include(ExternalProject)

set(FMT_LIB_PREFIX ${PROJECT_BINARY_DIR}/deps/fmtlib)
ExternalProject_Add(fmtlib
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 6.1.2
  CMAKE_ARGS "-DFMT_TEST=OFF" "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
  PREFIX ${FMT_LIB_PREFIX}
  INSTALL_COMMAND ""
  UPDATE_COMMAND ""
)

add_dependencies(common fmtlib)
target_include_directories(common SYSTEM PUBLIC ${FMT_LIB_PREFIX}/src/fmtlib/include)
link_directories(${FMT_LIB_PREFIX}/src/fmtlib-build)
target_link_libraries(common PUBLIC fmt$<$<CONFIG:Debug>:d>)

add_executable(15-day01 src/2015/day01.cpp src/2015/day01.hpp)
add_executable(15-day02 src/2015/day02.cpp src/2015/day02.hpp)
add_executable(15-day03 src/2015/day03.cpp src/2015/day03.hpp)

add_executable(19-day01 src/2019/day01.cpp src/2019/day01.hpp)
add_executable(19-day02 src/2019/day02.cpp src/2019/day02.hpp)
add_executable(19-day03 src/2019/day03.cpp src/2019/day03.hpp)
add_executable(19-day04 src/2019/day04.cpp src/2019/day04.hpp)
add_executable(19-day05 src/2019/day05.cpp src/2019/day05.hpp)
add_executable(19-day06 src/2019/day06.cpp src/2019/day06.hpp)
add_executable(19-day07 src/2019/day07.cpp src/2019/day07.hpp)
add_executable(19-day08 src/2019/day08.cpp src/2019/day08.hpp)
add_executable(19-day09 src/2019/day09.cpp src/2019/day09.hpp)
add_executable(19-day10 src/2019/day10.cpp src/2019/day10.hpp)
add_executable(19-day11 src/2019/day11.cpp src/2019/day11.hpp)
add_executable(19-day12 src/2019/day12.cpp src/2019/day12.hpp)
add_executable(19-day13 src/2019/day13.cpp src/2019/day13.hpp)
add_executable(19-day14 src/2019/day14.cpp src/2019/day14.hpp)
add_executable(19-day15 src/2019/day15.cpp src/2019/day15.hpp)
add_executable(19-day16 src/2019/day16.cpp src/2019/day16.hpp)
add_executable(19-day17 src/2019/day17.cpp src/2019/day17.hpp)
add_executable(19-day19 src/2019/day19.cpp src/2019/day19.hpp)

target_link_libraries(15-day01 PRIVATE common)
target_link_libraries(15-day02 PRIVATE common)
target_link_libraries(15-day03 PRIVATE common)

target_link_libraries(19-day01 PRIVATE common)
target_link_libraries(19-day02 PRIVATE common)
target_link_libraries(19-day03 PRIVATE common)
target_link_libraries(19-day04 PRIVATE common)
target_link_libraries(19-day05 PRIVATE common)
target_link_libraries(19-day06 PRIVATE common)
target_link_libraries(19-day07 PRIVATE common)
target_link_libraries(19-day08 PRIVATE common)
target_link_libraries(19-day09 PRIVATE common)
target_link_libraries(19-day10 PRIVATE common)
target_link_libraries(19-day11 PRIVATE common)
target_link_libraries(19-day12 PRIVATE common)
target_link_libraries(19-day13 PRIVATE common)
target_link_libraries(19-day14 PRIVATE common)
target_link_libraries(19-day15 PRIVATE common)
target_link_libraries(19-day16 PRIVATE common)
target_link_libraries(19-day17 PRIVATE common)
target_link_libraries(19-day19 PRIVATE common)

add_custom_target(run-15-01 15-day01 DEPENDS 15-day01)
add_custom_target(run-15-02 15-day02 DEPENDS 15-day02)
add_custom_target(run-15-03 15-day03 DEPENDS 15-day03)

add_custom_target(run-19-01 19-day01 DEPENDS 19-day01)
add_custom_target(run-19-02 19-day02 DEPENDS 19-day02)
add_custom_target(run-19-03 19-day03 DEPENDS 19-day03)
add_custom_target(run-19-04 19-day04 DEPENDS 19-day04)
add_custom_target(run-19-05 19-day05 DEPENDS 19-day05)
add_custom_target(run-19-06 19-day06 DEPENDS 19-day06)
add_custom_target(run-19-07 19-day07 DEPENDS 19-day07)
add_custom_target(run-19-08 19-day08 DEPENDS 19-day08)
add_custom_target(run-19-09 19-day09 DEPENDS 19-day09)
add_custom_target(run-19-10 19-day10 DEPENDS 19-day10)
add_custom_target(run-19-11 19-day11 DEPENDS 19-day11)
add_custom_target(run-19-12 19-day12 DEPENDS 19-day12)
add_custom_target(run-19-13 19-day13 DEPENDS 19-day13)
add_custom_target(run-19-14 19-day14 DEPENDS 19-day14)
add_custom_target(run-19-15 19-day15 DEPENDS 19-day15)
add_custom_target(run-19-16 19-day16 DEPENDS 19-day16)
add_custom_target(run-19-17 19-day17 DEPENDS 19-day17)
add_custom_target(run-19-19 19-day19 DEPENDS 19-day19)

add_custom_target(run-15
  DEPENDS run-15-01 run-15-02 run-15-03
)

add_custom_target(run-19
  DEPENDS run-19-01 run-19-02 run-19-03 run-19-04 run-19-05 run-19-06 run-19-07 run-19-08 run-19-09 run-19-10
  run-19-11 run-19-12 run-19-13 run-19-14 run-19-15 run-19-16 run-19-17 run-19-19
)

find_program(CLANG_FORMAT NAMES "clang-format")

if(CLANG_FORMAT)
  file(GLOB_RECURSE FILES_TO_FORMAT src/*.cpp src/*.hpp)
  add_custom_target(clang-format COMMAND ${CLANG_FORMAT} -i ${FILES_TO_FORMAT})
endif()
