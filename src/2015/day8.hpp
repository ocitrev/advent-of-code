#pragma once
#include "input-bytes.hpp"

#include <string_view>

namespace input
{
    constexpr std::string_view example1{R"(""
"abc"
"aaa\"aaa"
"\x27")"};
}
