#include "day09.hpp"
#include "../assert.hpp"
#include "../intcode.hpp"
#include "../terminal.hpp"
#include <fmt/format.h>

std::vector<Int> Run(std::vector<Int> code, Int input = 0)
{
    std::vector<Int> result;

    auto in = [input]() {
        return input;
    };

    auto out = [&result](Int value) {
        result.push_back(value);
    };

    Intcode::Run(std::move(code), in, out);
    return result;
}

int main()
{
    fmt::print("Day 9: Sensor Boost\n");

    Assert(
        (std::vector<Int>{109, 1, 204, -1, 1001, 100, 1, 100, 1008, 100, 16, 101, 1006, 101, 0, 99}
         == Run({109, 1, 204, -1, 1001, 100, 1, 100, 1008, 100, 16, 101, 1006, 101, 0, 99})));
    Assert(std::vector<Int>{1219070632396864} == Run({1102, 34915192, 34915192, 7, 4, 7, 99, 0}));
    Assert(std::vector<Int>{1125899906842624} == Run({104, 1125899906842624, 99}));

    auto part1 = Run(input::data, 1);
    fmt::print("  Part1: {}\n", part1.front());
    Assert(2890527621 == part1.front());

    auto part2 = Run(input::data, 2);
    fmt::print("  Part2: {}\n", part2.front());
    Assert(66772 == part2.front());
}
