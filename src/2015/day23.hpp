#pragma once
#include "input-bytes.hpp"
#include <string_view>

namespace example
{
    static constexpr std::string_view instructions{R"(inc a
jio a, +2
tpl a
inc a)"};
}
