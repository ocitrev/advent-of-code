#include "day2.hpp"
#include "../common.hpp"
#include "../common/intcode.hpp"
#include "../common/terminal.hpp"

[[maybe_unused]] static Int RunAndGetValue(std::vector<Int> code, size_t offset)
{
    Intcode a{code};
    a.Run();
    return a.ReadMemory(offset);
}

static Int Run(std::vector<Int> const &code, Int noun, Int verb)
{
    Intcode a{code};
    a.WriteMemory(1, noun);
    a.WriteMemory(2, verb);
    a.Run();
    return a.ReadMemory(0);
}

static void Part1(std::vector<Int> const &code)
{
    auto part1 = Run(code, 12, 2);
    fmt::print("  Part1: {}\n", part1);
    Assert(3706713 == part1);
}

static void Part2(std::vector<Int> const &code)
{
    for (Int noun = 0; noun != 100; ++noun)
    {
        for (Int verb = 0; verb != 100; ++verb)
        {
            if (Run(code, noun, verb) == 19690720)
            {
                auto part2 = noun * 100 + verb;
                fmt::print("  Part2: {}\n", part2);
                Assert(8609 == part2);
                return;
            }
        }
    }
}

int main()
{
    fmt::print("Day 2, 2019: 1202 Program Alarm\n");

    Assert(2 == RunAndGetValue({1, 0, 0, 0, 99}, 0));
    Assert(6 == RunAndGetValue({2, 3, 0, 3, 99}, 3));
    Assert(9801 == RunAndGetValue({2, 4, 4, 5, 99, 0}, 5));
    Assert(30 == RunAndGetValue({1, 1, 1, 4, 99, 5, 6, 0, 99}, 0));

    Part1(ParseInputNumbers<Int, ','>());
    Part2(ParseInputNumbers<Int, ','>());
}
