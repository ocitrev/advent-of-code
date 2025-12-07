#include "day12.hpp"

#include "../common.hpp"

using namespace std::string_view_literals;

struct Cave
{
    static Cave const Start;
    static Cave const End;
    inline static std::hash<std::string_view> const hasher;

    size_t id;
    bool lower;
    std::string_view cave_;

    Cave(std::string_view cave)
        : id{hasher(cave)}
        , lower{std::all_of(begin(cave), end(cave), &islower)}
        , cave_(std::move(cave))
    {
    }

    bool IsStart() const
    {
        return id == Start.id;
    }

    bool IsEnd() const
    {
        return id == End.id;
    }

    bool operator==(Cave const &c) const
    {
        return id == c.id;
    }

    bool operator<(Cave const &c) const
    {
        return id < c.id;
    }
};

Cave const Cave::Start{"start"sv};
Cave const Cave::End{"end"sv};

template <>
struct std::hash<Cave>
{
    std::size_t operator()(Cave const &c) const noexcept
    {
        return c.id;
    }
};

struct Paths
{
    std::unordered_map<Cave, std::vector<Cave>> paths;

    void Parse(std::string_view map)
    {
        for (auto line : Split(map, '\n'))
        {
            auto parts = Split(line, '-');
            Cave const left = parts[0];
            Cave const right = parts[1];

            if (not right.IsStart() && not left.IsEnd())
            {
                paths[left].push_back(right);
            }

            if (not left.IsStart() && not right.IsEnd())
            {
                paths[right].push_back(left);
            }
        }
    }

    auto FindAllPaths(int maxCount)
    {
        int allPaths = 0;
        FindAllPaths(Cave::Start, {}, allPaths, maxCount);
        return allPaths;
    }

private:
    void FindAllPaths(Cave const &start, std::vector<Cave> parents, int &nbPaths, int maxCount)
    {
        if (start.IsEnd())
        {
            ++nbPaths;
            return;
        }

        auto iter = paths.find(start);

        if (iter == end(paths))
        {
            return;
        }

        // only visit samll caves once
        if (start.lower)
        {
            auto const visitedCount = std::count(begin(parents), end(parents), start);

            if (maxCount == 1 && visitedCount >= 1)
            {
                return;
            }

            if (visitedCount > 0)
            {
                --maxCount;
            }
        }

        parents.push_back(start);

        for (auto p : iter->second)
        {
            FindAllPaths(p, parents, nbPaths, maxCount);
        }
    }
};

static int Part1(std::string_view map)
{
    Paths p;
    p.Parse(map);
    return p.FindAllPaths(1);
}

static int Part2(std::string_view map)
{
    Paths p;
    p.Parse(map);
    return p.FindAllPaths(2);
}

int main()
{
    // https://adventofcode.com/2021/day/12
    std::print("Day 12, 2021: Passage Pathing\n");

    Assert(10 == Part1(example::map1));
    Assert(19 == Part1(example::map2));
    Assert(226 == Part1(example::map3));
    Assert(36 == Part2(example::map1));
    Assert(103 == Part2(example::map2));
    Assert(3509 == Part2(example::map3));

    auto const part1 = Part1(GetInput());
    std::print("  Part 1: {}\n", part1);
    Assert(3298 == part1);

    auto const part2 = Part2(GetInput());
    std::print("  Part 2: {}\n", part2);
    Assert(93'572 == part2);
}
