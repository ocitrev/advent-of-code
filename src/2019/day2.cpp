#include "day2.hpp"

#include "../cpp-utils.hpp"
#include "../cpp-utils/intcode.hpp"
#include "../cpp-utils/terminal.hpp"

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

static Int Part1(std::vector<Int> const &code)
{
    return Run(code, 12, 2);
}

Int Part2(std::vector<Int> const &code)
{
    for (Int noun = 0; noun != 100; ++noun)
    {
        for (Int verb = 0; verb != 100; ++verb)
        {
            if (Run(code, noun, verb) == 19'690'720)
            {
                return noun * 100 + verb;
            }
        }
    }

    return 0;
}

int main()
{
    // https://adventofcode.com/2019/day/2
    std::println("Day 2, 2019: 1202 Program Alarm");

    Assert(2 == RunAndGetValue({1, 0, 0, 0, 99}, 0));
    Assert(6 == RunAndGetValue({2, 3, 0, 3, 99}, 3));
    Assert(9801 == RunAndGetValue({2, 4, 4, 5, 99, 0}, 5));
    Assert(30 == RunAndGetValue({1, 1, 1, 4, 99, 5, 6, 0, 99}, 0));

    auto const part1 = Part1(ParseInputNumbers<Int, ','>());
    std::println("  Part1: {}", part1);
    Assert(3'706'713 == part1);

    auto const part2 = Part2(ParseInputNumbers<Int, ','>());
    std::println("  Part2: {}", part2);
    Assert(8609 == part2);
}
