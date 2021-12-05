cmake_minimum_required(VERSION 3.20)

# integrate with vcpkg
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "")
endif()

project(AdventOfCOde)

if(MSVC)
  find_program(CPPCHECK NAMES "cppcheck.exe")
  if(CPPCHECK)
    find_file(FMT_FORMAT_INL NAMES fmt/format-inl.h)
    set(CMAKE_CXX_CPPCHECK ${CPPCHECK} -q --enable=all --suppress=*:${FMT_FORMAT_INL} --suppress=missingIncludeSystem)
  endif()
else()
  # find_program(CLANG_TIDY NAMES "clang-tidy")
  # if(CLANG_TIDY)
  #   set(CLANG_TIDY_CHECK_LIST
  #     cppcoreguidelines-*
  #     -cppcoreguidelines-avoid-magic-numbers
  #     modernize-*
  #     -modernize-use-trailing-return-type
  #     performance-*
  #     readability-*
  #     -readability-braces-around-statements # ne fonctionne pas avec le style Alleman
  #     -readability-magic-numbers
  #     )
  #   list(JOIN CLANG_TIDY_CHECK_LIST "," CLANG_TIDY_CHECKS)
  #   set(CMAKE_CXX_CLANG_TIDY
  #     ${CLANG_TIDY}
  #     -checks=${CLANG_TIDY_CHECKS}
  #     -header-filter=.*
  #     --format-style=file
  #     --warnings-as-errors=*
  #     )
  # endif()
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
  src/common/point2d.hpp
  src/common/point3d.hpp
  src/common/point4d.hpp
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

target_compile_features(common PUBLIC cxx_std_23)
target_link_libraries(common PUBLIC project_warnings)

find_package(Microsoft.GSL CONFIG REQUIRED)
target_link_libraries(common PUBLIC Microsoft.GSL::GSL)
find_package(fmt CONFIG REQUIRED)
target_link_libraries(common PUBLIC fmt::fmt-header-only)
find_package(RapidJSON CONFIG REQUIRED)
target_include_directories(common PRIVATE ${RAPIDJSON_INCLUDE_DIRS})

function(add_aoc year day)
  add_executable(${year}-day${day} src/${year}/day${day}.cpp src/${year}/day${day}.hpp)
  target_link_libraries(${year}-day${day} PRIVATE common)
  add_custom_target(run-${year}-${day} ${year}-day${day} DEPENDS ${year}-day${day})
  
  if(TARGET run-${year})
    add_dependencies(run-${year} run-${year}-${day})
  else()
    add_custom_target(run-${year} DEPENDS run-${year}-${day})
  endif()

  if(TARGET run)
    add_dependencies(run run-${year}-${day})
  else()
    add_custom_target(run DEPENDS run-${year}-${day})
  endif()
endfunction()

add_aoc(2015 01)
add_aoc(2015 02)
add_aoc(2015 03)
add_aoc(2015 04)
add_aoc(2015 05)
add_aoc(2015 06)
add_aoc(2015 07)
add_aoc(2015 08)
add_aoc(2015 09)
add_aoc(2015 10)
add_aoc(2015 11)
add_aoc(2015 12)
add_aoc(2015 13)
add_aoc(2015 14)
add_aoc(2015 15)
add_aoc(2015 16)
add_aoc(2015 17)
add_aoc(2015 18)
add_aoc(2015 19)
add_aoc(2015 20)
add_aoc(2015 21)
add_aoc(2015 22)
add_aoc(2015 23)
add_aoc(2015 24)
add_aoc(2015 25)

add_aoc(2019 01)
add_aoc(2019 02)
add_aoc(2019 03)
add_aoc(2019 04)
add_aoc(2019 05)
add_aoc(2019 06)
add_aoc(2019 07)
add_aoc(2019 08)
add_aoc(2019 09)
add_aoc(2019 10)
add_aoc(2019 11)
add_aoc(2019 12)
add_aoc(2019 13)
add_aoc(2019 14)
add_aoc(2019 15)
add_aoc(2019 16)
add_aoc(2019 17)
add_aoc(2019 19)

add_aoc(2020 01)
add_aoc(2020 02)
add_aoc(2020 03)
add_aoc(2020 04)
add_aoc(2020 05)
add_aoc(2020 06)
add_aoc(2020 07)
add_aoc(2020 08)
add_aoc(2020 09)
add_aoc(2020 10)
add_aoc(2020 11)
add_aoc(2020 12)
add_aoc(2020 13)
add_aoc(2020 14)
add_aoc(2020 15)
add_aoc(2020 16)
add_aoc(2020 17)
add_aoc(2020 18)
add_aoc(2020 19)

add_aoc(2021 01)
add_aoc(2021 02)
add_aoc(2021 03)
add_aoc(2021 04)
add_aoc(2021 05)

find_program(CLANG_FORMAT NAMES "clang-format")

if(CLANG_FORMAT)
  file(GLOB_RECURSE FILES_TO_FORMAT src/*.cpp src/*.hpp)
  add_custom_target(clang-format COMMAND ${CLANG_FORMAT} -i ${FILES_TO_FORMAT})
endif()
