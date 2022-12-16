#pragma once
#include "../common/point2d.hpp"
#include "input-bytes.hpp"
#include <string_view>

namespace example
{
    using namespace std::literals;
    static constexpr auto scanlines = R"(498,4 -> 498,6 -> 496,6
503,4 -> 502,4 -> 502,9 -> 494,9)"sv;

    static constexpr Point2d dropLocation{500, 0};
}
