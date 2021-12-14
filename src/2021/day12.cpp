#include "day12.hpp"
#include "../common.hpp"

using namespace std::string_view_literals;

static constexpr auto kStart = "start"sv;
static constexpr auto kEnd = "end"sv;

static bool IsLower(std::string_view text)
{
    return std::all_of(begin(text), end(text), &islower);
}

struct Paths
{
    std::unordered_map<std::string_view, std::vector<std::string_view>> paths;

    void Parse(std::string_view map)
    {
        for (auto line : Split(map, '\n'))
        {
            auto parts = Split(line, '-');
            auto const &left = parts[0];
            auto const &right = parts[1];

            if (right != kStart && left != kEnd)
                paths[left].push_back(right);

            if (left != kStart && right != kEnd)
                paths[right].push_back(left);
        }
    }

    auto FindAllPaths(int maxCount)
    {
        std::vector<std::string> allPaths;
        FindAllPaths(kStart, {}, allPaths, maxCount);
        return allPaths;
    }

    void FindAllPaths(std::string_view start, std::vector<std::string_view> parents, std::vector<std::string> &allPaths,
        int maxCount)
    {
        if (start == kEnd)
        {
            parents.push_back(start);
            allPaths.push_back(Join(begin(parents), end(parents), ","));
            return;
        }

        auto iter = paths.find(start);

        if (iter == end(paths))
            return;

        // only visit samll caves once
        if (IsLower(start))
        {
            auto const visitedCount = std::count(begin(parents), end(parents), start);

            if (maxCount == 1 && visitedCount >= 1)
                return; 
            
            if (visitedCount > 0)
                --maxCount;
        }

        parents.push_back(start);

        for (auto p : iter->second)
        {
            FindAllPaths(p, parents, allPaths, maxCount);
        }
    }
};

static int Part1(std::string_view map)
{
    Paths p;
    p.Parse(map);
    auto const allPaths = p.FindAllPaths(1);
    return static_cast<int>(allPaths.size());
}

static int Part2(std::string_view map)
{
    Paths p;
    p.Parse(map);
    auto const allPaths = p.FindAllPaths(2);
    return static_cast<int>(allPaths.size());
}

int main()
{
    // https://adventofcode.com/2021/day/12
    fmt::print("Day 12, 2021: Passage Pathing\n");

    Assert(10 == Part1(example::map1));
    Assert(19 == Part1(example::map2));
    Assert(226 == Part1(example::map3));
    Assert(36 == Part2(example::map1));
    Assert(103 == Part2(example::map2));
    Assert(3509 == Part2(example::map3));

    auto const part1 = Part1(input::map);
    fmt::print("  Part 1: {}\n", part1);
    Assert(3298 == part1);

    auto const part2 = Part2(input::map);
    fmt::print("  Part 2: {}\n", part2);
    Assert(93572 == part2);
}
