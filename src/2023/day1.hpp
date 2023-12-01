#pragma once
#include "input-bytes.hpp"
#include <string_view>

namespace example
{
    using namespace std::literals;
    static constexpr auto part1 = R"(1abc2
pqr3stu8vwx
a1b2c3d4e5f
treb7uchet)"sv;

    static constexpr auto part2 = R"(two1nine
eightwothree
abcone2threexyz
xtwone3four
4nineeightseven2
zoneight234
7pqrstsixteen)"sv;
}
