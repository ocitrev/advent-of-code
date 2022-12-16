#pragma once
#include "input-bytes.hpp"
#include <string_view>

namespace input
{
    constexpr std::string_view example{R"(123 -> x
456 -> y
x AND y -> d
x OR y -> e
x LSHIFT 2 -> f
y RSHIFT 2 -> g
NOT x -> h
NOT y -> i)"};
}
