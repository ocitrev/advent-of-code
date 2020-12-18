#include "day06.hpp"
#include "../common/assert.hpp"
#include "../common/string.hpp"
#include <array>
#include <charconv>
#include <fmt/format.h>
#include <functional>
#include <numeric>
#include <vector>

struct Rect
{
    int left;
    int top;
    int right;
    int bottom;

    [[nodiscard]] int Width() const
    {
        return right - left + 1;
    }

    [[nodiscard]] int Height() const
    {
        return bottom - top + 1;
    }

    [[nodiscard]] static Rect Parse(std::string_view corner1, std::string_view corner2)
    {
        auto pos1 = corner1.find(',');
        auto left = corner1.substr(0, pos1);
        auto top = corner1.substr(pos1 + 1, corner1.size());
        auto pos2 = corner2.find(',');
        auto right = corner2.substr(0, pos2);
        auto bottom = corner2.substr(pos2 + 1, corner2.size());

        return {
            svtoi(left),
            svtoi(top),
            svtoi(right),
            svtoi(bottom),
        };
    }
};

class Grid
{
    std::vector<int> grid = std::vector<int>(1'000'000);

    void ParseLine(std::string_view line, void (*turnOn)(int &), void (*turnOff)(int &), void (*toggle)(int &))
    {
        auto throughPos = line.find("through");
        Rect rc{};
        void (*op)(int &) = nullptr;

        if (starts_with(line, "turn on"))
        {
            rc = Rect::Parse(line.substr(8, throughPos - 8), line.substr(throughPos + 8, line.size()));
            op = turnOn;
        }
        else if (starts_with(line, "turn off"))
        {
            rc = Rect::Parse(line.substr(9, throughPos - 9), line.substr(throughPos + 8, line.size()));
            op = turnOff;
        }
        else if (starts_with(line, "toggle"))
        {
            rc = Rect::Parse(line.substr(7, throughPos - 7), line.substr(throughPos + 8, line.size()));
            op = toggle;
        }
        else
        {
            throw std::runtime_error("invalid command");
        }

        for (auto y = static_cast<std::size_t>(rc.top); y <= static_cast<std::size_t>(rc.bottom); ++y)
        {
            std::size_t const offset = y * 1000;

            for (auto x = static_cast<std::size_t>(rc.left); x <= static_cast<std::size_t>(rc.right); ++x)
            {
                op(grid[offset + x]);
            }
        }
    }

public:
    void ParseLine1(std::string_view line)
    {
        ParseLine(
            line,
            [](int &value) {
                value = 1;
            },
            [](int &value) {
                value = 0;
            },
            [](int &value) {
                value = value == 0 ? 1 : 0;
            });
    }

    void ParseLine2(std::string_view line)
    {
        ParseLine(
            line,
            [](int &value) {
                ++value;
            },
            [](int &value) {
                if (value > 0)
                    --value;
            },
            [](int &value) {
                value += 2;
            });
    }

    void ParseInstructions1(std::string_view instructions)
    {
        for (auto const &line : Split(instructions, '\n'))
        {
            ParseLine1(line);
        }
    }

    void ParseInstructions2(std::string_view instructions)
    {
        for (auto const &line : Split(instructions, '\n'))
        {
            ParseLine2(line);
        }
    }

    [[nodiscard]] int HowManyOn() const
    {
        return std::accumulate(begin(grid), end(grid), 0, [](int sum, int v) {
            return sum + (v == 0 ? 0 : 1);
        });
    }

    [[nodiscard]] int TotalBrightness() const
    {
        return std::accumulate(begin(grid), end(grid), 0);
    }

    void Print()
    {
        for (std::size_t y = 0; y < 1'000; ++y)
        {
            for (std::size_t x = 0; x < 1'000; ++x)
            {
                fmt::print(grid[y * 1000 + x] == 0 ? " " : "#");
            }

            fmt::print("\n");
        }
    }
};

int Part1()
{
    Grid grid;
    grid.ParseInstructions1(input::data);
    return grid.HowManyOn();
}

int Part2()
{
    Grid grid;
    grid.ParseInstructions2(input::data);
    return grid.TotalBrightness();
}

int main()
{
    // https://adventofcode.com/2015/day/6
    fmt::print("Day 6: Probably a Fire Hazard\n");

    int const part1 = Part1();
    fmt::print("  Part1: {}\n", part1);
    Assert(569999 == part1);

    int const part2 = Part2();
    fmt::print("  Part2: {}\n", part2);
    Assert(17836115 == part2);
}
