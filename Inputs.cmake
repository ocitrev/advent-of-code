# This block is executed when generating an intermediate resource file, not when
# running in CMake configure mode
if(_INPUT_GENERATE_MODE)
  file(READ ${INPUT_FILE} hex_content HEX)
  string(REPEAT "[0-9a-f]" 32 column_pattern)
  string(REGEX REPLACE "(${column_pattern})" "\\1\n" content "${hex_content}")
  string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1, " content "${content}")
  string(REGEX REPLACE ", $" "" content "${content}")
  string(CONFIGURE [[
// Auto generated file
#include <string_view>

inline constexpr char inputBytes[]{
@content@
};

constexpr std::string_view GetInput()
{
    std::string_view text{inputBytes, sizeof(inputBytes)};
    
    if (auto const pos = text.find_last_not_of("\r\n"); pos != text.npos)
    {
        text = text.substr(0, pos + 1); 
    }

    return text;
}
]] code)
  file(WRITE ${OUTPUT_FILE} "${code}")
  return()
endif()

set(_INPUTS_SCRIPT ${CMAKE_CURRENT_LIST_FILE} CACHE INTERNAL "Path to Inputs.cmake script")

function(add_aoc_input year day)
  set(libname "resources-${year}-${day}")
  set(input_file "${CMAKE_SOURCE_DIR}/inputs/${year}/day${day}.txt")
  set(output_file "${CMAKE_BINARY_DIR}/inputs/${year}/${day}/input-bytes.hpp")
  add_library(${libname} INTERFACE EXCLUDE_FROM_ALL ${output_file})
  target_include_directories(${libname} INTERFACE "${CMAKE_BINARY_DIR}/inputs/${year}/${day}")

  add_custom_command(
    OUTPUT ${output_file}
    DEPENDS ${input_file} ${_INPUTS_SCRIPT}
    COMMAND 
      ${CMAKE_COMMAND}
        -D_INPUT_GENERATE_MODE=TRUE
        -DINPUT_FILE=${input_file}
        -DOUTPUT_FILE=${output_file}
        -P ${_INPUTS_SCRIPT}
    COMMENT "Generating intermediate file for ${input_file}"
  )
endfunction()
