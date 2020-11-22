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

add_library(md5 OBJECT
  src/common/md5.c
  src/common/md5.h
  src/common/md5.hpp
)

add_library(common STATIC
  src/common/combinations.hpp
  src/common/intcode.cpp
  src/common/intcode.hpp
  src/common/point.hpp
  src/common/string.cpp
  src/common/string.hpp
  src/common/terminal.cpp
  src/common/terminal.hpp
  $<TARGET_OBJECTS:md5>
)

target_compile_definitions(common PUBLIC
  _USE_MATH_DEFINES
  NOMINMAX
  WIN32_LEAN_AND_MEAN
)

target_compile_features(common PUBLIC cxx_std_20)
target_link_libraries(common PUBLIC project_warnings)

find_package(Microsoft.GSL CONFIG REQUIRED)
target_link_libraries(common PUBLIC Microsoft.GSL::GSL)
find_package(fmt CONFIG REQUIRED)
target_link_libraries(common PUBLIC fmt::fmt fmt::fmt-header-only)

# 2015
set(AoC2015 01 02 03 04 05 06 07)
foreach(n IN LISTS AoC2015)
  add_executable(15-day${n} src/2015/day${n}.cpp src/2015/day${n}.hpp)
  target_link_libraries(15-day${n} PRIVATE common)
  add_custom_target(run-15-${n} 15-day${n} DEPENDS 15-day${n})
endforeach()
list(TRANSFORM AoC2015 PREPEND run-15-)
add_custom_target(run-15 DEPENDS ${AoC2015})

# 2019
set(AoC2019 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 19)
foreach(n IN LISTS AoC2019)
  add_executable(19-day${n} src/2019/day${n}.cpp src/2019/day${n}.hpp)
  target_link_libraries(19-day${n} PRIVATE common)
  add_custom_target(run-19-${n} 19-day${n} DEPENDS 19-day${n})
endforeach()
list(TRANSFORM AoC2019 PREPEND run-19-)
add_custom_target(run-19 DEPENDS ${AoC2019})

add_custom_target(run
  DEPENDS run-15 run-19
)

find_program(CLANG_FORMAT NAMES "clang-format")

if(CLANG_FORMAT)
  file(GLOB_RECURSE FILES_TO_FORMAT src/*.cpp src/*.hpp)
  add_custom_target(clang-format COMMAND ${CLANG_FORMAT} -i ${FILES_TO_FORMAT})
endif()
