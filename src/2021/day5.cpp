#include "day5.hpp"

#include "../common.hpp"

#include <map>
#include <ranges>

static Point2d ParsePoint(std::string_view text)
{
    trim(text);
    auto const coords = Split(text, ',');
    return {svtoi(coords[0]), svtoi(coords[1])};
}

struct Line
{
    Point2d start;
    Point2d end;

    static Line Parse(std::string_view text)
    {
        auto const parts = Split(text, "->");
        return {ParsePoint(parts[0]), ParsePoint(parts[1])};
    }

    bool IsHorizontal() const
    {
        return start.y == end.y;
    }

    bool IsVertical() const
    {
        return start.x == end.x;
    }

    bool IsDiagonal() const
    {
        return abs(end.x - start.x) == abs(end.y - start.y);
    }
};

static void DrawHorizontal(std::map<Point2d, int> &map, Line const &line)
{
    int const dir = line.end.x < line.start.x ? -1 : 1;
    int const last = line.end.x + dir;
    int const y = line.start.y;

    for (int x = line.start.x; x != last; x += dir)
    {
        ++map[{x, y}];
    }
}

static void DrawVertical(std::map<Point2d, int> &map, Line const &line)
{
    int const dir = line.end.y < line.start.y ? -1 : 1;
    int const last = line.end.y + dir;
    int const x = line.start.x;

    for (int y = line.start.y; y != last; y += dir)
    {
        ++map[{x, y}];
    }
}

static void DrawDiagonal(std::map<Point2d, int> &map, Line const &line)
{
    int const dirX = line.end.x < line.start.x ? -1 : 1;
    int const dirY = line.end.y < line.start.y ? -1 : 1;
    Point2d const dir{dirX, dirY};
    Point2d const last = line.end + dir;

    for (auto iter = line.start; iter != last; iter += dir)
    {
        ++map[iter];
    }
}

static int GetOverlap(std::map<Point2d, int> const &map)
{
    int count = 0;

    for (auto const &[p, n] : map)
    {
        if (n >= 2)
        {
            ++count;
        }
    }

    return count;
}

static int Part1(std::string_view lines)
{
    std::map<Point2d, int> map;

    for (auto line : Split(lines, '\n'))
    {
        Line const ventLine = Line::Parse(line);

        if (ventLine.IsHorizontal())
        {
            DrawHorizontal(map, ventLine);
        }
        else if (ventLine.IsVertical())
        {
            DrawVertical(map, ventLine);
        }
    }

    return GetOverlap(map);
}

static int Part2(std::string_view lines)
{
    std::map<Point2d, int> map;

    for (auto line : Split(lines, '\n'))
    {
        Line const ventLine = Line::Parse(line);

        if (ventLine.IsHorizontal())
        {
            DrawHorizontal(map, ventLine);
        }
        else if (ventLine.IsVertical())
        {
            DrawVertical(map, ventLine);
        }
        else if (ventLine.IsDiagonal())
        {
            DrawDiagonal(map, ventLine);
        }
    }

    return GetOverlap(map);
}

int main()
{
    // https://adventofcode.com/2021/day/5
    std::println("Day 5, 2021: Hydrothermal Venture");

    Assert(5 == Part1(example::lines));
    Assert((Line{{1, 1}, {3, 3}}.IsDiagonal()));
    Assert((Line{{9, 7}, {7, 9}}.IsDiagonal()));
    Assert(12 == Part2(example::lines));

    auto const part1 = Part1(GetInput());
    std::println("  Part 1: {}", part1);
    Assert(5835 == part1);

    auto const part2 = Part2(GetInput());
    std::println("  Part 2: {}", part2);
    Assert(17'013 == part2);
}
