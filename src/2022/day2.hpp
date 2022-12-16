#pragma once
#include "input-bytes.hpp"
#include <string_view>

namespace example
{
    using namespace std::literals;
    constexpr std::string_view lines = R"(A Y
B X
C Z)"sv;
}
