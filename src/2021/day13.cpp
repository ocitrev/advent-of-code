#include "day13.hpp"

#include "../common.hpp"

struct Map
{
    std::unordered_set<Point2d> map;
    std::vector<Point2d> folds;

    void Parse(std::string_view text)
    {
        for (auto line : Split(text, '\n'))
        {
            if (starts_with(line, "fold along x="))
            {
                folds.emplace_back(svtoi(line.substr(13)), 0);
                continue;
            }
            else if (starts_with(line, "fold along y="))
            {
                folds.emplace_back(0, svtoi(line.substr(13)));
                continue;
            }
            else if (line.empty())
            {
                continue;
            }

            auto parts = Split(line, ',');
            map.emplace(svtoi(parts[0]), svtoi(parts[1]));
        }
    }

    void FoldX(int x)
    {
        std::unordered_set<Point2d> folded;

        for (Point2d p : map)
        {
            if (p.x < x)
            {
                folded.insert(p);
            }
            else
            {
                folded.insert({x - (p.x - x), p.y});
            }
        }

        map = folded;
    }

    void FoldY(int y)
    {
        std::unordered_set<Point2d> folded;

        for (Point2d p : map)
        {
            if (p.y < y)
            {
                folded.insert(p);
            }
            else
            {
                folded.insert({p.x, y - (p.y - y)});
            }
        }

        map = folded;
    }

    void FoldOne()
    {
        auto fold = folds.front();
        folds.erase(begin(folds));

        if (fold.x != 0)
        {
            FoldX(fold.x);
        }
        else if (fold.y != 0)
        {
            FoldY(fold.y);
        }
    }

    void FoldAll()
    {
        for (auto fold : folds)
        {
            if (fold.x != 0)
            {
                FoldX(fold.x);
            }
            else if (fold.y != 0)
            {
                FoldY(fold.y);
            }
        }
    }

    std::pair<Point2d, Point2d> GetBounds() const
    {
        auto getX = [](Point2d const &p)
        {
            return p.x;
        };

        auto getY = [](Point2d const &p)
        {
            return p.y;
        };

        auto const [minX, maxX] = std::ranges::minmax(map | std::views::transform(getX));
        auto const [minY, maxY] = std::ranges::minmax(map | std::views::transform(getY));
        return {{minX, minY}, {maxX, maxY}};
    }

    std::string Print() const
    {
        auto const [min, max] = GetBounds();
        std::string out;

        for (int y = min.y; y <= max.y; ++y)
        {
            for (int x = min.x; x <= max.x; ++x)
            {
                Point2d const p{x, y};

                if (map.find(p) == end(map))
                {
                    out.push_back(' ');
                }
                else
                {
                    out.append("█");
                }
            }

            out.push_back('\n');
        }

        return out;
    }

    auto Count() const
    {
        return map.size();
    }
};

static auto Part1(std::string_view text)
{
    Map map;
    map.Parse(text);
    map.FoldOne();
    return map.Count();
}

static auto Part2(std::string_view text)
{
    Map map;
    map.Parse(text);
    map.FoldAll();
    return map.Print();
}

int main()
{
    // https://adventofcode.com/2021/day/13
    std::println("Day 13, 2021: Transparent Origami");

    Assert(17 == Part1(example::instructions));

    auto const part1 = Part1(GetInput());
    std::println("  Part 1: {}", part1);
    Assert(592 == part1);

    auto const part2 = Part2(GetInput());
    std::println("  Part 2:\n{}", part2);
}
