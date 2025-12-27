#include "day9.hpp"

#include "../cpp-utils.hpp"
#include "../cpp-utils/intcode.hpp"

static std::vector<Int> Run(std::vector<Int> code, Int input = 0)
{
    std::vector<Int> result;

    auto in = [input]()
    {
        return input;
    };

    auto out = [&result](Int value)
    {
        result.push_back(value);
    };

    Intcode::Run(std::move(code), in, out);
    return result;
}

int main()
{
    std::println("Day 9, 2019: Sensor Boost");

    Assert((std::vector<Int>{109, 1, 204, -1, 1001, 100, 1, 100, 1008, 100, 16, 101, 1006, 101, 0, 99}
        == Run({109, 1, 204, -1, 1001, 100, 1, 100, 1008, 100, 16, 101, 1006, 101, 0, 99})));
    Assert(std::vector<Int>{1'219'070'632'396'864} == Run({1102, 34'915'192, 34'915'192, 7, 4, 7, 99, 0}));
    Assert(std::vector<Int>{1'125'899'906'842'624} == Run({104, 1'125'899'906'842'624, 99}));

    auto const part1 = Run(ParseInputNumbers<Int, ','>(), 1).front();
    std::println("  Part1: {}", part1);
    Assert(2'890'527'621 == part1);

    auto const part2 = Run(ParseInputNumbers<Int, ','>(), 2).front();
    std::println("  Part2: {}", part2);
    Assert(66'772 == part2);
}
