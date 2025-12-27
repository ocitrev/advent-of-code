#include "day3.hpp"

#include "../common.hpp"

#include <unordered_map>

static bool IsSymbol(char c)
{
    if (c == '.' || std::isdigit(c))
    {
        return false;
    }

    return true;
}

struct Grid
{
    std::unordered_map<Point2d, char> grid;
    std::unordered_map<Point2d, size_t> partNumber;
    std::vector<int> partNumbers;
    int w = 0;
    int h = 0;

    void ParseSchematic(std::string_view text)
    {
        int y = 0;

        for (auto line : Split(text, '\n'))
        {
            int x = 0;

            for (char c : line)
            {
                grid[Point2d{x, y}] = c;
                ++x;
                w = std::max(w, x);
            }

            ++y;
            h = std::max(h, y);
        }
    }

    void ParsePartNumbers()
    {
        std::string numberText;
        Point2d lastNumber;
        std::unordered_set<Point2d> numberCoords;

        for (int y = 0; y != h; ++y)
        {
            for (int x = 0; x != w; ++x)
            {
                Point2d const p{x, y};
                char const c = GetCell(p);

                if (std::isdigit(c))
                {
                    numberText.append(1, c);
                    numberCoords.insert(p);
                    lastNumber = p;
                }
                else if (not numberText.empty())
                {
                    bool const isSymbol = [&]
                    {
                        for (int yy = -1; yy <= 1; ++yy)
                        {
                            int const nbChars = static_cast<int>(numberText.size());
                            int const firstX = lastNumber.x - nbChars;
                            int const lastX = lastNumber.x + 1;

                            for (int xx = firstX; xx <= lastX; ++xx)
                            {
                                if (yy == 0 && xx > firstX && xx < lastX)
                                {
                                    continue;
                                }

                                Point2d const nei{xx, lastNumber.y + yy};

                                if (IsSymbol(GetCell(nei)))
                                {
                                    return true;
                                }
                            }
                        }

                        return false;
                    }();

                    int const number = svtoi(numberText);

                    if (isSymbol)
                    {
                        auto const index = partNumbers.size();
                        partNumbers.push_back(number);

                        for (auto pp : numberCoords)
                        {
                            partNumber[pp] = index;
                        }
                    }

                    numberCoords.clear();
                    numberText.clear();
                }
            }
        }
    }

    auto SumOfPartNumber() const
    {
        return std::accumulate(begin(partNumbers), end(partNumbers), int64_t{0});
    }

    char GetCell(Point2d p) const
    {
        if (auto iter = grid.find(p); iter != end(grid))
        {
            return iter->second;
        }

        return '.';
    }

    auto GetGearRatios() const
    {
        int64_t total = 0;
        for (int y = 0; y != h; ++y)
        {
            for (int x = 0; x != w; ++x)
            {
                Point2d const p{x, y};
                char const c = GetCell(p);

                if (c != '*')
                {
                    continue;
                }

                static constexpr size_t kInvalid = static_cast<size_t>(-1);
                size_t a = kInvalid;
                size_t b = kInvalid;

                for (int yy = -1; yy <= 1; ++yy)
                {
                    for (int xx = -1; xx <= 1; ++xx)
                    {
                        if (xx == 0 && yy == 0)
                        {
                            continue;
                        }

                        if (auto iter = partNumber.find(p + Point2d{xx, yy}); iter != end(partNumber))
                        {
                            if (a == kInvalid)
                            {
                                a = iter->second;
                            }
                            else if (b == kInvalid)
                            {
                                if (a != iter->second)
                                {
                                    b = iter->second;
                                }
                            }
                            else
                            {
                                // there should never be 3 part numbers near a gear
                                Assert(b == iter->second);
                            }
                        }
                    }
                }

                if (a != kInvalid && b != kInvalid)
                {
                    total += partNumbers[a] * partNumbers[b];
                }
            }
        }

        return total;
    }
};

static auto SumOfPartNumber(std::string_view schematic)
{
    Grid g;
    g.ParseSchematic(schematic);
    g.ParsePartNumbers();
    return g.SumOfPartNumber();
}

static auto Part1()
{
    return SumOfPartNumber(GetInput());
}

static auto GetGearRatios(std::string_view schematic)
{
    Grid g;
    g.ParseSchematic(schematic);
    g.ParsePartNumbers();
    return g.GetGearRatios();
}

static auto Part2()
{
    return GetGearRatios(GetInput());
}

int main()
{
    // https://adventofcode.com/2023/day/3
    std::println("Day 3, 2023: Gear Ratios");

    Assert(4361 == SumOfPartNumber(example::schematic));
    Assert(467'835 == GetGearRatios(example::schematic));

    auto const part1 = Part1();
    std::println("  Part 1: {}", part1);
    Assert(527'364 == part1);

    auto const part2 = Part2();
    std::println("  Part 2: {}", part2);
    Assert(79'026'871 == part2);
}
