#include "day11.hpp"
#include "../common/assert.hpp"
#include "../common/string.hpp"
#include <array>
#include <fmt/format.h>

struct Grid
{
    std::vector<char> seats;
    int width = 0;
    int height = 0;

    void Parse(std::string_view text)
    {
        auto const lines = Split(text, "\n");
        width = static_cast<int>(lines.front().size());
        height = static_cast<int>(lines.size());

        for (auto &&line : lines)
        {
            seats.insert(seats.end(), begin(line), end(line));
        }
    }

    [[nodiscard]] int TotalOccupiedCount() const
    {
        return static_cast<int>(std::count(begin(seats), end(seats), '#'));
    }

    template <typename OccupiedFuncT>
    bool Simulate(OccupiedFuncT &&countOccupied, int occupiedLimit)
    {
        std::vector<char> newSeats = seats;
        bool changed = false;

        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                auto const cell = GetCell(x, y);
                if (cell == '.')
                    continue;

                auto const nbOccupied = (this->*countOccupied)(x, y);

                if (cell == 'L')
                {
                    if (nbOccupied == 0)
                    {
                        newSeats[GetOffset(x, y)] = '#';
                        changed = true;
                    }
                }
                else if (cell == '#')
                {
                    if (nbOccupied >= occupiedLimit)
                    {
                        newSeats[GetOffset(x, y)] = 'L';
                        changed = true;
                    }
                }
            }
        }

        if (changed)
        {
            seats = std::move(newSeats);
            return true;
        }

        return false;
    }

    [[nodiscard]] int CountSurroundingOccupiedFrom(int x, int y) const
    {
        std::array const neighbors{GetCell(x - 1, y - 1), GetCell(x, y - 1), GetCell(x + 1, y - 1), GetCell(x + 1, y),
                                   GetCell(x + 1, y + 1), GetCell(x, y + 1), GetCell(x - 1, y + 1), GetCell(x - 1, y)};
        return static_cast<int>(std::count(begin(neighbors), end(neighbors), '#'));
    }

    [[nodiscard]] int CountVisibleOccupiedFrom(int x, int y) const
    {
        auto loop = [this, x, y](int incX, int incY) {
            int xx = x;
            int yy = y;

            while (true)
            {
                xx += incX;
                yy += incY;
                auto const c = GetCell(xx, yy);

                if (c == '.')
                    continue;

                return c == '#' ? 1 : 0;
            }

            return 0;
        };

        int count{};
        count += loop(-1, -1);
        count += loop(0, -1);
        count += loop(1, -1);
        count += loop(-1, 0);
        count += loop(1, 0);
        count += loop(-1, 1);
        count += loop(0, 1);
        count += loop(1, 1);
        return count;
    }

    int RunUntilStable1()
    {
        int count = 0;

        while (Simulate(&Grid::CountSurroundingOccupiedFrom, 4))
            ++count;

        return count;
    }

    int RunUntilStable2()
    {
        int count = 0;

        while (Simulate(&Grid::CountVisibleOccupiedFrom, 5))
            ++count;

        return count;
    }

    [[nodiscard]] std::size_t GetOffset(int x, int y) const
    {
        return static_cast<std::size_t>(y * width + x);
    }

    [[nodiscard]] char GetCell(int x, int y) const
    {
        if (x < 0 || y < 0)
            return '\0';

        if (x >= width || y >= height)
            return '\0';

        return seats[GetOffset(x, y)];
    }
};

static void Example()
{
#ifndef NDEBUG
    Grid g1;
    g1.Parse(example::seats);
    g1.RunUntilStable1();
    Assert(37 == g1.TotalOccupiedCount());

    Grid visible1;
    visible1.Parse(example::visibleSteats1);
    Assert(8 == visible1.CountVisibleOccupiedFrom(3, 4));

    Grid visible2;
    visible2.Parse(example::visibleSteats2);
    Assert(0 == visible2.CountVisibleOccupiedFrom(1, 1));

    Grid visible3;
    visible3.Parse(example::visibleSteats3);
    Assert(0 == visible3.CountVisibleOccupiedFrom(3, 3));

    Grid g2;
    g2.Parse(example::seats);
    g2.RunUntilStable2();
    Assert(26 == g2.TotalOccupiedCount());
#endif
}

static int Part1()
{
    Grid g;
    g.Parse(input::seats);
    g.RunUntilStable1();
    return g.TotalOccupiedCount();
}

static int Part2()
{
    Grid g;
    g.Parse(input::seats);
    g.RunUntilStable2();
    return g.TotalOccupiedCount();
}

int main()
{
    // https://adventofcode.com/2020/day/11
    fmt::print("Day 11, 2020: Seating System\n");

    Example();

    auto const part1 = Part1();
    fmt::print("  Part 1: {}\n", part1);
    Assert(2273 == part1);

    auto const part2 = Part2();
    fmt::print("  Part 2: {}\n", part2);
    Assert(2064 == part2);
}
