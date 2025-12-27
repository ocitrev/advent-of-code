#include "day18.hpp"

#include "../cpp-utils/assert.hpp"
#include "../cpp-utils/string.hpp"

#include <array>
#include <numeric>
#include <print>
#include <vector>

struct GOL
{
    std::vector<bool> map;
    int width = 0;
    int height = 0;
    bool cornerAlwaysOn = false;

    [[nodiscard]] std::size_t GetOffset(int x, int y) const
    {
        return static_cast<std::size_t>(y * width + x);
    }

    [[nodiscard]] bool IsCorner(int x, int y) const
    {
        return (x == 0 && y == 0) || (x == width - 1 && y == 0) || (x == width - 1 && y == height - 1)
            || (x == 0 && y == height - 1);
    }

    [[nodiscard]] bool GetCell(int x, int y) const
    {
        if (x < 0 || y < 0)
        {
            return false;
        }

        if (x >= width || y >= height)
        {
            return false;
        }

        if (cornerAlwaysOn && IsCorner(x, y))
        {
            return true;
        }

        return map[GetOffset(x, y)];
    }

    void LoadMap(std::string_view mapText)
    {
        auto const &lines = Split(mapText, '\n');
        int w = width = static_cast<int>(lines.front().size());
        int h = height = static_cast<int>(lines.size());
        map.resize(static_cast<std::size_t>(h * w));
        std::size_t offset = 0;

        for (auto &&line : lines)
        {
            for (char c : line)
            {
                map[offset++] = c == '#';
            }
        }
    }

    void StepOnce()
    {
        std::vector<bool> newState(map.size());

        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                if (cornerAlwaysOn && IsCorner(x, y))
                {
                    newState[GetOffset(x, y)] = true;
                    continue;
                }

                auto const cell = GetCell(x, y);
                std::array const neighbors{GetCell(x - 1, y - 1), GetCell(x, y - 1), GetCell(x + 1, y - 1),
                    GetCell(x + 1, y), GetCell(x + 1, y + 1), GetCell(x, y + 1), GetCell(x - 1, y + 1), GetCell(x - 1, y)};

                auto const nbAlive = std::accumulate(begin(neighbors), end(neighbors), 0);

                if (cell)
                {
                    newState[GetOffset(x, y)] = nbAlive == 2 || nbAlive == 3;
                }
                else
                {
                    newState[GetOffset(x, y)] = nbAlive == 3;
                }
            }
        }

        map = std::move(newState);
    }

    void Run(int nbSteps)
    {
        for (int i = 0; i < nbSteps; ++i)
        {
            StepOnce();
        }
    }

    [[nodiscard]] int GetAliveCount() const
    {
        return std::accumulate(begin(map), end(map), 0);
    }
};

static void Example()
{
#ifndef NDEBUG
    auto ParseMap = [](std::string_view mapText)
    {
        GOL gol;
        gol.LoadMap(mapText);
        return gol.map;
    };

    {
        GOL gameOfLife1;
        gameOfLife1.LoadMap(example1::state0);
        gameOfLife1.StepOnce();
        Assert(ParseMap(example1::state1) == gameOfLife1.map);
        gameOfLife1.StepOnce();
        Assert(ParseMap(example1::state2) == gameOfLife1.map);
        gameOfLife1.StepOnce();
        Assert(ParseMap(example1::state3) == gameOfLife1.map);
        gameOfLife1.StepOnce();
        Assert(ParseMap(example1::state4) == gameOfLife1.map);
        Assert(4 == gameOfLife1.GetAliveCount());
    }

    {
        GOL gameOfLife2;
        gameOfLife2.cornerAlwaysOn = true;
        gameOfLife2.LoadMap(example2::state0);
        gameOfLife2.StepOnce();
        Assert(ParseMap(example2::state1) == gameOfLife2.map);
        gameOfLife2.StepOnce();
        Assert(ParseMap(example2::state2) == gameOfLife2.map);
        gameOfLife2.StepOnce();
        Assert(ParseMap(example2::state3) == gameOfLife2.map);
        gameOfLife2.StepOnce();
        Assert(ParseMap(example2::state4) == gameOfLife2.map);
        gameOfLife2.StepOnce();
        Assert(ParseMap(example2::state5) == gameOfLife2.map);
        Assert(17 == gameOfLife2.GetAliveCount());
    }
#endif
}

static int Part1()
{
    GOL gameOfLife;
    gameOfLife.LoadMap(GetInput());
    gameOfLife.Run(100);
    return gameOfLife.GetAliveCount();
}

static int Part2()
{
    GOL gameOfLife;
    gameOfLife.cornerAlwaysOn = true;
    gameOfLife.LoadMap(GetInput());
    gameOfLife.Run(100);
    return gameOfLife.GetAliveCount();
}

int main()
{
    // https://adventofcode.com/2015/day/18
    std::println("Day 18, 2015: Like a GIF For Your Yard");

    Example();

    auto const part1 = Part1();
    std::println("  Part 1: {}", part1);
    Assert(768 == part1);

    auto const part2 = Part2();
    std::println("  Part 2: {}", part2);
    Assert(781 == part2);
}
