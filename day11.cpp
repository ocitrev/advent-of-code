#include "day11.hpp"
#include "common/intcode.hpp"
#include "common/main.hpp"
#include "common/point.hpp"
#include <array>
#include <cassert>
#include <iostream>
#include <map>

struct Robot
{
    Point pos{0, 0};
    std::map<Point, Int> grid;
    std::array<Point, 4> dirs = {{{0, -1}, {-1, 0}, {0, 1}, {1, 0}}};
    Point dir = dirs.front();
    bool paint = true;

    void RotateLeft()
    {
        std::rotate(begin(dirs), begin(dirs) + 1, end(dirs));
    }

    void RotateRight()
    {
        std::rotate(rbegin(dirs), rbegin(dirs) + 1, rend(dirs));
    }

    void Run()
    {
        auto getPanel = [&]() -> Int {
            auto iter = grid.find(pos);
            if (iter != end(grid))
                return iter->second;

            return 0;
        };

        auto readOutput = [&](Int value) {
            if (paint)
            {
                paint = false;
                grid[pos] = value;
            }
            else
            {
                paint = true;
                if (value == 0)
                {
                    RotateLeft();
                }
                else if (value == 1)
                {
                    RotateRight();
                }

                dir = dirs.front();
                pos += dir;
            }
        };

        Intcode::Run(input::code, getPanel, readOutput);
    }
};

void Part1()
{
    Robot robot;
    robot.Run();
    std::cout << "Part1: " << robot.grid.size() << '\n';
    assert(expected::part1 == robot.grid.size());
}

void Part2()
{
    Robot robot;
    robot.grid[{0, 0}] = 1;
    robot.Run();

    Point min;
    Point max;

    for (auto const &elem : robot.grid)
    {
        min.x = std::min(min.x, elem.first.x);
        min.y = std::min(min.y, elem.first.y);
        max.x = std::max(max.x, elem.first.x);
        max.y = std::max(max.y, elem.first.y);
    }

    std::string result;
    std::array<std::string_view, 2> lookup = {" ", "█"};

    for (int y = min.y; y <= max.y; ++y)
    {
        for (int x = min.x; x <= max.x; ++x)
        {
            result.append(lookup.at(static_cast<std::size_t>(robot.grid[{x, y}])));
        }

        result.append("\n");
    }

    std::cout << "Part2:\n" << result;
    assert(expected::part2 == result);
}

void Main()
{
    std::cout << "Day 11: Space Police\n";

    Part1();
    Part2();
}
