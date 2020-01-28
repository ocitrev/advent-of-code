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
      -header-filter=.
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
  common/combinations.hpp
  common/intcode.cpp
  common/intcode.hpp
  common/main.cpp
  common/main.hpp
  common/point.hpp
  common/string.cpp
  common/string.hpp
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

add_executable(day01 day01.cpp day01.hpp)
add_executable(day02 day02.cpp day02.hpp)
add_executable(day03 day03.cpp day03.hpp)
add_executable(day04 day04.cpp day04.hpp)
add_executable(day05 day05.cpp day05.hpp)
add_executable(day06 day06.cpp day06.hpp)
add_executable(day07 day07.cpp day07.hpp)
add_executable(day08 day08.cpp day08.hpp)
add_executable(day09 day09.cpp day09.hpp)
add_executable(day10 day10.cpp day10.hpp)
add_executable(day11 day11.cpp day11.hpp)
add_executable(day12 day12.cpp day12.hpp)
add_executable(day13 day13.cpp day13.hpp)
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
target_link_libraries(day10 PRIVATE common)
target_link_libraries(day11 PRIVATE common)
target_link_libraries(day12 PRIVATE common)
target_link_libraries(day13 PRIVATE common)
target_link_libraries(day16 PRIVATE common)
target_link_libraries(day17 PRIVATE common)
target_link_libraries(day19 PRIVATE common)

add_custom_target(run01 day01 DEPENDS day01)
add_custom_target(run02 day02 DEPENDS day02)
add_custom_target(run03 day03 DEPENDS day03)
add_custom_target(run04 day04 DEPENDS day04)
add_custom_target(run05 day05 DEPENDS day05)
add_custom_target(run06 day06 DEPENDS day06)
add_custom_target(run07 day07 DEPENDS day07)
add_custom_target(run08 day08 DEPENDS day08)
add_custom_target(run09 day09 DEPENDS day09)
add_custom_target(run10 day10 DEPENDS day10)
add_custom_target(run11 day11 DEPENDS day11)
add_custom_target(run12 day12 DEPENDS day12)
add_custom_target(run13 day13 DEPENDS day13)
add_custom_target(run16 day16 DEPENDS day16)
add_custom_target(run17 day17 DEPENDS day17)
add_custom_target(run19 day19 DEPENDS day19)

add_custom_target(run
  DEPENDS run01 run02 run03 run04 run05 run06 run07 run08 run09
  run10 run11 run12 run13 run16 run17 run19
)

find_program(CLANG_FORMAT NAMES "clang-format")

if(CLANG_FORMAT)
  file(GLOB FILES_TO_FORMAT *.cpp *.hpp common/*.cpp common/*.hpp)
  add_custom_target(clang-format COMMAND ${CLANG_FORMAT} -i ${FILES_TO_FORMAT})
endif()
