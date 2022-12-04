#include "day2.hpp"
#include "../common.hpp"

constexpr int MoveUp(int current)
{
    if (current <= 3)
        return current;

    return current - 3;
}

constexpr int MoveDown(int current)
{
    if (current >= 7)
        return current;

    return current + 3;
}

constexpr int MoveLeft(int current)
{
    if (current == 1 || current == 4 || current == 7)
        return current;

    return current - 1;
}

constexpr int MoveRight(int current)
{
    if (current == 3 || current == 6 || current == 9)
        return current;

    return current + 1;
}

constexpr int Move(int current, char dir)
{
    switch (dir)
    {
    case 'U':
        return MoveUp(current);
    case 'D':
        return MoveDown(current);
    case 'L':
        return MoveLeft(current);
    case 'R':
        return MoveRight(current);
    }

    return current;
}

static_assert(1 == Move(1, 'U'));
static_assert(2 == Move(2, 'U'));
static_assert(3 == Move(3, 'U'));
static_assert(1 == Move(4, 'U'));
static_assert(2 == Move(5, 'U'));
static_assert(3 == Move(6, 'U'));
static_assert(4 == Move(7, 'U'));
static_assert(5 == Move(8, 'U'));
static_assert(6 == Move(9, 'U'));
static_assert(4 == Move(1, 'D'));
static_assert(5 == Move(2, 'D'));
static_assert(6 == Move(3, 'D'));
static_assert(7 == Move(4, 'D'));
static_assert(8 == Move(5, 'D'));
static_assert(9 == Move(6, 'D'));
static_assert(7 == Move(7, 'D'));
static_assert(8 == Move(8, 'D'));
static_assert(9 == Move(9, 'D'));
static_assert(1 == Move(1, 'L'));
static_assert(1 == Move(2, 'L'));
static_assert(2 == Move(3, 'L'));
static_assert(4 == Move(4, 'L'));
static_assert(4 == Move(5, 'L'));
static_assert(5 == Move(6, 'L'));
static_assert(7 == Move(7, 'L'));
static_assert(7 == Move(8, 'L'));
static_assert(8 == Move(9, 'L'));
static_assert(2 == Move(1, 'R'));
static_assert(3 == Move(2, 'R'));
static_assert(3 == Move(3, 'R'));
static_assert(5 == Move(4, 'R'));
static_assert(6 == Move(5, 'R'));
static_assert(6 == Move(6, 'R'));
static_assert(8 == Move(7, 'R'));
static_assert(9 == Move(8, 'R'));
static_assert(9 == Move(9, 'R'));

static int ParseInstructions(std::string_view insctructions)
{
    int key = 5;
    int code = 0;

    for (auto line : Split(insctructions, '\n'))
    {
        code *= 10;

        for (char d : line)
        {
            key = Move(key, d);
        }

        code += key;
    }

    return code;
}

static int Part1()
{
    return ParseInstructions(input::instructions);
}

static int Part2()
{
    return 0;
}

int main()
{
    // https://adventofcode.com/2016/day/2
    fmt::print("Day 2, 2016: Bathroom Security\n");

    Assert(1985 == ParseInstructions(example::instructions));

    auto const part1 = Part1();
    fmt::print("  Part 1: {}\n", part1);
    Assert(36629 == part1);

    auto const part2 = Part2();
    fmt::print("  Part 2: {}\n", part2);
    // Assert( == part2);
}