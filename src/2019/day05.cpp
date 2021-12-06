#include "day05.hpp"
#include "../common/assert.hpp"
#include "../common/intcode.hpp"
#include "../common/terminal.hpp"
#include <fmt/format.h>
#include <vector>

[[maybe_unused]] static Int Run(std::vector<Int> const &code, Int input)
{
    Int outValue = 0;
    Intcode::Run(
        code,
        [input]()
        {
            return input;
        },
        [&](Int output)
        {
            outValue = output;
        });
    return outValue;
}

static void Part1(std::vector<Int> const &code)
{
    Intcode::Run(
        code,
        []()
        {
            return 1;
        },
        [](Int value)
        {
            fmt::print("  Part1: {}\n", value);
            // 0 is outputed
            Assert(value == 0 || 13285749 == value);
        });
}

static void Part2(std::vector<Int> const &code)
{
    Intcode::Run(
        code,
        []()
        {
            return 5;
        },
        [](Int value)
        {
            fmt::print("  Part2: {}\n", value);
            Assert(5000972 == value);
        });
}

int main()
{
    fmt::print("Day 5, 2019: Sunny with a Chance of Asteroids\n");

    Assert(0 == Run({3, 9, 8, 9, 10, 9, 4, 9, 99, -1, 8}, 7));
    Assert(1 == Run({3, 9, 8, 9, 10, 9, 4, 9, 99, -1, 8}, 8));
    Assert(0 == Run({3, 9, 8, 9, 10, 9, 4, 9, 99, -1, 8}, 9));
    Assert(0 == Run({3, 3, 1108, -1, 8, 3, 4, 3, 99}, 7));
    Assert(1 == Run({3, 3, 1108, -1, 8, 3, 4, 3, 99}, 8));
    Assert(0 == Run({3, 3, 1108, -1, 8, 3, 4, 3, 99}, 9));

    // LT
    Assert(1 == Run({3, 9, 7, 9, 10, 9, 4, 9, 99, -1, 8}, 6));
    Assert(1 == Run({3, 9, 7, 9, 10, 9, 4, 9, 99, -1, 8}, 7));
    Assert(0 == Run({3, 9, 7, 9, 10, 9, 4, 9, 99, -1, 8}, 8));
    Assert(0 == Run({3, 9, 7, 9, 10, 9, 4, 9, 99, -1, 8}, 9));
    Assert(1 == Run({3, 3, 1107, -1, 8, 3, 4, 3, 99}, 6));
    Assert(1 == Run({3, 3, 1107, -1, 8, 3, 4, 3, 99}, 7));
    Assert(0 == Run({3, 3, 1107, -1, 8, 3, 4, 3, 99}, 8));
    Assert(0 == Run({3, 3, 1107, -1, 8, 3, 4, 3, 99}, 9));

    // JMP *
    Assert(0 == Run({3, 12, 6, 12, 15, 1, 13, 14, 13, 4, 13, 99, -1, 0, 1, 9}, 0));
    Assert(1 == Run({3, 12, 6, 12, 15, 1, 13, 14, 13, 4, 13, 99, -1, 0, 1, 9}, 1));
    Assert(0 == Run({3, 3, 1105, -1, 9, 1101, 0, 0, 12, 4, 12, 99, 1}, 0));
    Assert(1 == Run({3, 3, 1105, -1, 9, 1101, 0, 0, 12, 4, 12, 99, 1}, 1));
    Assert(999
           == Run({3,    21,   1008, 21, 8,    20,   1005, 20, 22,  107, 8,    21, 20,   1006, 20, 31,
                   1106, 0,    36,   98, 0,    0,    1002, 21, 125, 20,  4,    20, 1105, 1,    46, 104,
                   999,  1105, 1,    46, 1101, 1000, 1,    20, 4,   20,  1105, 1,  46,   98,   99},
                  7));
    Assert(1000
           == Run({3,    21,   1008, 21, 8,    20,   1005, 20, 22,  107, 8,    21, 20,   1006, 20, 31,
                   1106, 0,    36,   98, 0,    0,    1002, 21, 125, 20,  4,    20, 1105, 1,    46, 104,
                   999,  1105, 1,    46, 1101, 1000, 1,    20, 4,   20,  1105, 1,  46,   98,   99},
                  8));
    Assert(1001
           == Run({3,    21,   1008, 21, 8,    20,   1005, 20, 22,  107, 8,    21, 20,   1006, 20, 31,
                   1106, 0,    36,   98, 0,    0,    1002, 21, 125, 20,  4,    20, 1105, 1,    46, 104,
                   999,  1105, 1,    46, 1101, 1000, 1,    20, 4,   20,  1105, 1,  46,   98,   99},
                  9));

    Part1(input::data);
    Part2(input::data);
}
