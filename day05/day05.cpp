#include "day05.hpp"
#include "../common/intcode.hpp"
#include "../common/main.hpp"
#include <cassert>
#include <iostream>
#include <vector>

Int Run(std::vector<Int> const &code, Int input)
{
    Int outValue;
    Intcode::Run(code, [input]() { return input; }, [&](Int output)
    {
        outValue = output;
    });
    return outValue;

}

void Part1(std::vector<Int> const &code)
{
    Intcode::Run(code, []() { return 1; }, [](Int value)
    {
        std::cout << "Part1: " << value << '\n';
    });
}

void Part2(std::vector<Int> const &code)
{
    Intcode::Run(code, []() { return 5; }, [](Int value)
    {
        std::cout << "Part1: " << value << '\n';
    });
}

void Main()
{
    std::cout << "Day 5: Sunny with a Chance of Asteroids\n";

    assert(0 == Run({3, 9, 8, 9, 10, 9, 4, 9, 99, -1, 8}, 7));
    assert(1 == Run({3, 9, 8, 9, 10, 9, 4, 9, 99, -1, 8}, 8));
    assert(0 == Run({3, 9, 8, 9, 10, 9, 4, 9, 99, -1, 8}, 9));
    assert(0 == Run({3, 3, 1108, -1, 8, 3, 4, 3, 99}, 7));
    assert(1 == Run({3, 3, 1108, -1, 8, 3, 4, 3, 99}, 8));
    assert(0 == Run({3, 3, 1108, -1, 8, 3, 4, 3, 99}, 9));

    // LT
    assert(1 == Run({3, 9, 7, 9, 10, 9, 4, 9, 99, -1, 8}, 6));
    assert(1 == Run({3, 9, 7, 9, 10, 9, 4, 9, 99, -1, 8}, 7));
    assert(0 == Run({3, 9, 7, 9, 10, 9, 4, 9, 99, -1, 8}, 8));
    assert(0 == Run({3, 9, 7, 9, 10, 9, 4, 9, 99, -1, 8}, 9));
    assert(1 == Run({3, 3, 1107, -1, 8, 3, 4, 3, 99}, 6));
    assert(1 == Run({3, 3, 1107, -1, 8, 3, 4, 3, 99}, 7));
    assert(0 == Run({3, 3, 1107, -1, 8, 3, 4, 3, 99}, 8));
    assert(0 == Run({3, 3, 1107, -1, 8, 3, 4, 3, 99}, 9));

    // JMP *
    assert(0 == Run({3, 12, 6, 12, 15, 1, 13, 14, 13, 4, 13, 99, -1, 0, 1, 9}, 0));
    assert(1 == Run({3, 12, 6, 12, 15, 1, 13, 14, 13, 4, 13, 99, -1, 0, 1, 9}, 1));
    assert(0 == Run({3, 3, 1105, -1, 9, 1101, 0, 0, 12, 4, 12, 99, 1}, 0));
    assert(1 == Run({3, 3, 1105, -1, 9, 1101, 0, 0, 12, 4, 12, 99, 1}, 1));
    assert(999 == Run({3, 21, 1008, 21, 8, 20, 1005, 20, 22, 107, 8, 21, 20, 1006, 20, 31, 1106, 0, 36, 98, 0, 0, 1002, 21, 125, 20, 4, 20, 1105, 1, 46, 104, 999, 1105, 1, 46, 1101, 1000, 1, 20, 4, 20, 1105, 1, 46, 98, 99}, 7));
    assert(1000 == Run({3, 21, 1008, 21, 8, 20, 1005, 20, 22, 107, 8, 21, 20, 1006, 20, 31, 1106, 0, 36, 98, 0, 0, 1002, 21, 125, 20, 4, 20, 1105, 1, 46, 104, 999, 1105, 1, 46, 1101, 1000, 1, 20, 4, 20, 1105, 1, 46, 98, 99}, 8));
    assert(1001 == Run({3, 21, 1008, 21, 8, 20, 1005, 20, 22, 107, 8, 21, 20, 1006, 20, 31, 1106, 0, 36, 98, 0, 0, 1002, 21, 125, 20, 4, 20, 1105, 1, 46, 104, 999, 1105, 1, 46, 1101, 1000, 1, 20, 4, 20, 1105, 1, 46, 98, 99}, 9));

    std::vector<Int> const code{begin(inputData), end(inputData)};

    Part1(code);
    Part2(code);
}
