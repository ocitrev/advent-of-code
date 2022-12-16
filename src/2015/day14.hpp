#pragma once
#include "input-bytes.hpp"
#include <string_view>

namespace example
{
    constexpr std::string_view text{R"(Comet can fly 14 km/s for 10 seconds, but then must rest for 127 seconds.
Dancer can fly 16 km/s for 11 seconds, but then must rest for 162 seconds.)"};
}

namespace input
{
    constexpr int limit = 2503;
}
