#include "day02.hpp"
#include "intcode.hpp"
#include "terminal.hpp"
#include <cassert>
#include <fmt/format.h>
#include <vector>

Int RunAndGetValue(std::vector<Int> const &code, size_t offset)
{
    Intcode a{code};
    a.Run();
    return a.ReadMemory(offset);
}

Int Run(std::vector<Int> const &code, Int noun, Int verb)
{
    Intcode a{code};
    a.WriteMemory(1, noun);
    a.WriteMemory(2, verb);
    a.Run();
    return a.ReadMemory(0);
}

void Part1(std::vector<Int> const &code)
{
    auto part1 = Run(code, 12, 2);
    fmt::print("  Part1: {}\n", part1);
    assert(3706713 == part1);
}

void Part2(std::vector<Int> const &code)
{
    for (Int noun = 0; noun != 100; ++noun)
    {
        for (Int verb = 0; verb != 100; ++verb)
        {
            if (Run(code, noun, verb) == 19690720)
            {
                auto part2 = noun * 100 + verb;
                fmt::print("  Part2: {}\n", part2);
                assert(8609 == part2);
                return;
            }
        }
    }
}

int main()
{
    fmt::print("Day 2: 1202 Program Alarm\n");

    assert(2 == RunAndGetValue({1, 0, 0, 0, 99}, 0));
    assert(6 == RunAndGetValue({2, 3, 0, 3, 99}, 3));
    assert(9801 == RunAndGetValue({2, 4, 4, 5, 99, 0}, 5));
    assert(30 == RunAndGetValue({1, 1, 1, 4, 99, 5, 6, 0, 99}, 0));

    Part1(input::data);
    Part2(input::data);
}
