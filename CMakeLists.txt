cmake_minimum_required(VERSION 3.18)

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
    -Wno-psabi
  )
endif()

add_library(md5 OBJECT
  src/common/md5.c
  src/common/md5.h
  src/common/md5.hpp
)

add_library(common STATIC
  src/common.hpp
  src/common/combinations.hpp
  src/common/intcode.cpp
  src/common/intcode.hpp
  src/common/numbers.hpp
  src/common/point2d.hpp
  src/common/point3d.hpp
  src/common/point4d.hpp
  src/common/string.cpp
  src/common/string.hpp
  src/common/terminal.cpp
  src/common/terminal.hpp
  $<TARGET_OBJECTS:md5>
)

if(WIN32)
  target_compile_definitions(common PUBLIC
    _USE_MATH_DEFINES
    NOMINMAX
    WIN32_LEAN_AND_MEAN
  )
endif()

target_compile_features(common PUBLIC cxx_std_20)
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
  set_property(TARGET ${year}-day${day} APPEND PROPERTY ADDITIONAL_CLEAN_FILES ${year}-day${day}.pdb)
endfunction()

add_aoc(2015 1)
add_aoc(2015 2)
add_aoc(2015 3)
add_aoc(2015 4)
add_aoc(2015 5)
add_aoc(2015 6)
add_aoc(2015 7)
add_aoc(2015 8)
add_aoc(2015 9)
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

add_aoc(2016 1)
add_aoc(2016 2)

add_aoc(2019 1)
add_aoc(2019 2)
add_aoc(2019 3)
add_aoc(2019 4)
add_aoc(2019 5)
add_aoc(2019 6)
add_aoc(2019 7)
add_aoc(2019 8)
add_aoc(2019 9)
add_aoc(2019 10)
add_aoc(2019 11)
add_aoc(2019 12)
add_aoc(2019 13)
add_aoc(2019 14)
add_aoc(2019 15)
add_aoc(2019 16)
add_aoc(2019 17)
add_aoc(2019 19)

add_aoc(2020 1)
add_aoc(2020 2)
add_aoc(2020 3)
add_aoc(2020 4)
add_aoc(2020 5)
add_aoc(2020 6)
add_aoc(2020 7)
add_aoc(2020 8)
add_aoc(2020 9)
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

add_aoc(2021 1)
add_aoc(2021 2)
add_aoc(2021 3)
add_aoc(2021 4)
add_aoc(2021 5)
add_aoc(2021 6)
add_aoc(2021 7)
add_aoc(2021 8)
add_aoc(2021 9)
add_aoc(2021 10)
add_aoc(2021 11)
add_aoc(2021 12)
add_aoc(2021 13)
add_aoc(2021 14)
add_aoc(2021 15)

add_aoc(2022 1)
add_aoc(2022 2)
add_aoc(2022 3)
add_aoc(2022 4)
add_aoc(2022 5)
add_aoc(2022 6)

find_program(CLANG_FORMAT NAMES "clang-format")

if(CLANG_FORMAT)
  file(GLOB_RECURSE FILES_TO_FORMAT src/*.cpp src/*.hpp)
  add_custom_target(clang-format COMMAND ${CLANG_FORMAT} -i ${FILES_TO_FORMAT})
endif()
