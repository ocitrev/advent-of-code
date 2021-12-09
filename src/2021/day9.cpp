#include "day9.hpp"
#include "../common/assert.hpp"
#include "../common/point2d.hpp"
#include "../common/string.hpp"
#include <algorithm>
#include <fmt/format.h>
#include <unordered_map>
#include <unordered_set>

struct Map
{
    std::unordered_map<Point2d, int> map;
    using Bassin = std::unordered_set<Point2d>;
    std::vector<Bassin> bassins;
    int width = 0;
    int height = 0;

    void ParseMap(std::string_view mapText)
    {
        int y = 0;
        for (auto row : Split(mapText, '\n'))
        {
            int x = 0;

            for (char c : row)
            {
                map[{x, y}] = c - '0';
                ++x;
            }

            width = std::max(x, width);
            ++y;
        }

        height = y;
    }

    int GetValue(Point2d p) const
    {
        if (auto iter = map.find(p); iter != end(map))
            return iter->second;

        return 9;
    }

    std::vector<Point2d> GetLowSpots() const
    {
        std::vector<Point2d> lowSpots;

        for (int y = 0; y != height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                std::vector<int> surrounding;
                Point2d const pos{x, y};
                surrounding.push_back(GetValue(pos + Point2d::NORTH));
                surrounding.push_back(GetValue(pos + Point2d::WEST));
                surrounding.push_back(GetValue(pos + Point2d::EAST));
                surrounding.push_back(GetValue(pos + Point2d::SOUTH));
                int const here = GetValue(pos);
                bool const isLowSpot = std::all_of(begin(surrounding), end(surrounding),
                    [here](int v)
                    {
                        return here < v;
                    });

                if (isLowSpot)
                    lowSpots.push_back(pos);
            }
        }

        return lowSpots;
    }

    int ComputeRiskLevel() const
    {
        int riskLevel = 0;

        for (auto p : GetLowSpots())
        {
            riskLevel += GetValue(p) + 1;
        }

        return riskLevel;
    }

    Bassin *GetBassin(Point2d pos)
    {
        for (auto &b : bassins)
        {
            if (b.find(pos) != end(b))
                return &b;
        }

        return nullptr;
    }

    void FillBassin(Bassin &bassin, Point2d pos) const
    {
        if (GetValue(pos) == 9)
            return;

        if (auto const [iter, inserted] = bassin.insert(pos); not inserted)
            return;

        FillBassin(bassin, pos + Point2d::NORTH);
        FillBassin(bassin, pos + Point2d::WEST);
        FillBassin(bassin, pos + Point2d::EAST);
        FillBassin(bassin, pos + Point2d::SOUTH);
    }

    void FindAllBassins()
    {
        for (auto p : GetLowSpots())
        {
            auto &bassin = bassins.emplace_back();
            FillBassin(bassin, p);
        }
    }

    int GetSizeOfLargestBassins(int nb)
    {
        auto first = begin(bassins);
        auto topn = first + nb;

        std::nth_element(first, topn, end(bassins),
            [](Bassin const &a, Bassin const &b)
            {
                return b.size() < a.size();
            });

        return std::accumulate(first, topn, 1,
            [](int total, auto const &bassin)
            {
                return total * static_cast<int>(bassin.size());
            });
    }
};

static int Part1(std::string_view mapText)
{
    Map m;
    m.ParseMap(mapText);
    return m.ComputeRiskLevel();
}

static int Part2(std::string_view mapText)
{
    Map m;
    m.ParseMap(mapText);
    m.FindAllBassins();
    return m.GetSizeOfLargestBassins(3);
}

int main()
{
    // https://adventofcode.com/2021/day/9
    fmt::print("Day 9, 2021: Smoke Basin\n");

    Assert(15 == Part1(example::map));
    Assert(1134 == Part2(example::map));

    auto const part1 = Part1(input::map);
    fmt::print("  Part 1: {}\n", part1);
    Assert(562 == part1);

    auto const part2 = Part2(input::map);
    fmt::print("  Part 2: {}\n", part2);
    Assert(1076922 == part2);
}
