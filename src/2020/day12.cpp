#include "day12.hpp"
#include "../common/assert.hpp"
#include "../common/point2d.hpp"
#include "../common/string.hpp"
#include <fmt/format.h>

struct Ship
{
    bool part2;
    Point2d direction{1, 0};
    Point2d position{};

    Ship(bool part2_ = false)
        : part2(part2_)
    {
        if (part2)
        {
            direction = Point2d{10, 1};
        }
    }

    void TurnLeft(int n)
    {
        switch (n)
        {
        case 270:
            direction.Rotate90Right();
            break;
        case 180:
            direction.Rotate180();
            break;
        case 90:
            direction.Rotate90Left();
            break;
        }
    }

    void TurnRight(int n)
    {
        switch (n)
        {
        case 270:
            direction.Rotate90Left();
            break;
        case 180:
            direction.Rotate180();
            break;
        case 90:
            direction.Rotate90Right();
            break;
        }
    }

    void Move(int n)
    {
        position += direction * n;
    }

    void Move(Point2d const &dir, int n)
    {
        if (part2)
        {
            direction += dir * n;
            return;
        }

        position += dir * n;
    }

    void Step(std::string_view instruction)
    {
        int const n = svtoi(instruction.substr(1));

        switch (instruction[0])
        {
        case 'L':
            TurnLeft(n);
            break;
        case 'R':
            TurnRight(n);
            break;
        case 'F':
            Move(n);
            break;
        case 'N':
            Move(Point2d{0, 1}, n);
            break;
        case 'S':
            Move(Point2d{0, -1}, n);
            break;
        case 'E':
            Move(Point2d{1, 0}, n);
            break;
        case 'W':
            Move(Point2d{-1, 0}, n);
            break;
        }
    }

    Ship &Execute(std::string_view instructions)
    {
        for (auto &&line : Split(instructions, "\n"))
        {
            Step(line);
        }

        return *this;
    }

    [[nodiscard]] int Distance() const
    {
        return position.Distance();
    }
};

int Part1()
{
    Ship s;
    s.Execute(input::instructions);
    return s.Distance();
}

int Part2()
{
    Ship s{true};
    s.Execute(input::instructions);
    return s.Distance();
}

int main()
{
    // https://adventofcode.com/2020/day/12
    fmt::print("Day 12: Rain Risk\n");

    Assert(25 == Ship{}.Execute(example::instructions).Distance());
    Assert(286 == Ship{true}.Execute(example::instructions).Distance());

    auto const part1 = Part1();
    fmt::print("  Part 1: {}\n", part1);
    Assert(1032 == part1);

    auto const part2 = Part2();
    fmt::print("  Part 2: {}\n", part2);
    Assert(156735 == part2);
}
