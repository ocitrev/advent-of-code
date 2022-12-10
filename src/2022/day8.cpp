#include "day8.hpp"
#include "../common.hpp"

struct Tree
{
    int8_t height = 0;
    bool visible = true;
};

struct Map
{
    std::unordered_map<Point2d, Tree> grid;
    int width = 0;
    int height = 0;

    Tree &operator[](Point2d const &p)
    {
        return grid[p];
    }

    bool IsVisible(Point2d pos, Point2d dir)
    {
        auto const h = grid.find(pos)->second.height;
        int8_t other = 0;

        while (true)
        {
            pos += dir;

            if (pos.x < 0 || pos.y < 0 || pos.x == width || pos.y == height)
                break;

            auto const t = grid.find(pos)->second.height;

            if (h < t)
                return false;

            other = std::max(other, t);
        }

        return h > other;
    }

    int CountSmaller(int8_t h, Point2d pos, Point2d dir) const
    {
        int count = 0;

        while (true)
        {
            pos += dir;

            if (pos.x < 0 || pos.y < 0 || pos.x == width || pos.y == height)
                break;

            ++count;
            auto const other = grid.find(pos)->second.height;

            if (h <= other)
                break;
        }

        return count;
    }

    int GetScore(Point2d pos) const
    {
        auto const h = grid.find(pos)->second.height;
        return CountSmaller(h, pos, Point2d::WEST) * CountSmaller(h, pos, Point2d::NORTH)
               * CountSmaller(h, pos, Point2d::EAST) * CountSmaller(h, pos, Point2d::SOUTH);
    }

    int CountVisible() const
    {
        int count = 0;

        for (auto const &[_, tree] : grid)
        {
            if (tree.visible)
            {
                ++count;
            }
        }

        return count;
    }
};

static auto ParseMap(std::string_view mapText)
{
    Map map;
    int y = 0;
    int w = 0;

    for (auto line : Split(mapText, '\n'))
    {
        int x = 0;

        for (char c : line)
        {
            map[{x, y}] = Tree{static_cast<int8_t>(c - '0')};
            ++x;
        }

        w = std::max(w, x);
        ++y;
    }

    map.width = w;
    map.height = y;
    return map;
}

static int CountVisibleTrees(std::string_view mapText)
{
    auto map = ParseMap(mapText);

    for (int y = 1; y != map.height - 1; ++y)
    {
        for (int x = 1; x != map.width - 1; ++x)
        {
            Point2d pos{x, y};

            if (map.IsVisible(pos, Point2d::WEST) || map.IsVisible(pos, Point2d::NORTH)
                || map.IsVisible(pos, Point2d::EAST) || map.IsVisible(pos, Point2d::SOUTH))
            {
                continue;
            }

            map[pos].visible = false;
        }
    }

    return map.CountVisible();
}

static int GetBestScore(std::string_view mapText)
{
    auto map = ParseMap(mapText);
    int score = 0;

    for (int y = 0; y != map.height; ++y)
    {
        for (int x = 0; x != map.width; ++x)
        {
            score = std::max(score, map.GetScore({x, y}));
        }
    }

    return score;
}

static int Part1()
{
    return CountVisibleTrees(input::map);
}

static int Part2()
{
    return GetBestScore(input::map);
}

int main()
{
    // https://adventofcode.com/2022/day/8
    fmt::print("Day 8, 2022: Treetop Tree House\n");

    Assert(21 == CountVisibleTrees(example::map));
    Assert(8 == GetBestScore(example::map));

    auto const part1 = Part1();
    fmt::print("  Part 1: {}\n", part1);
    Assert(1708 == part1);

    auto const part2 = Part2();
    fmt::print("  Part 2: {}\n", part2);
    Assert(504000 == part2);
}
