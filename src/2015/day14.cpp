#include "day14.hpp"
#include "../common/assert.hpp"
#include "../common/string.hpp"
#include <algorithm>
#include <fmt/format.h>
#include <gsl/gsl>
#include <vector>

struct Deer
{
    int speed = 0;
    int time = 0;
    int rest = 0;
    std::string name;

    static Deer Parse(std::string_view text)
    {
        constexpr size_t nameIndex = 0;
        constexpr size_t speedIndex = 3;
        constexpr size_t timeIndex = 6;
        constexpr size_t restIndex = 13;

        auto const parts = Split(text, ' ');
        return {
            svtoi(parts[speedIndex]),
            svtoi(parts[timeIndex]),
            svtoi(parts[restIndex]),
            std::string{parts[nameIndex]},
        };
    }

    bool operator==(Deer const &other) const
    {
        return speed == other.speed && time == other.time && rest == other.rest && name == other.name;
    }

    [[nodiscard]] int GetDistance(int seconds) const
    {
        auto result = std::div(seconds, time + rest);
        int distance = result.quot * speed * time;
        distance += std::min(result.rem, time) * speed;
        return distance;
    }
};

std::vector<Deer> ParseDeers(std::string_view text)
{
    std::vector<Deer> deers;

    for (auto &&line : Split(text, '\n'))
    {
        deers.emplace_back(Deer::Parse(line));
    }

    return deers;
}

int Part1()
{
    auto const deers = ParseDeers(GetInput());
    std::vector<int> distances(deers.size());
    Ensures(not deers.empty());

    std::transform(begin(deers), end(deers), begin(distances),
        [](Deer const &deer)
        {
            return deer.GetDistance(input::limit);
        });

    return *std::max_element(begin(distances), end(distances));
}

[[nodiscard]] int GetWinnerPoints(std::string_view text, int limit)
{
    auto const deers = ParseDeers(text);
    std::vector<int> points(deers.size());
    std::vector<int> distances(deers.size());
    Ensures(not deers.empty());

    for (int s = 0; s < limit; ++s)
    {
        std::transform(begin(deers), end(deers), begin(distances),
            [elapsed = s + 1](Deer const &deer)
            {
                return deer.GetDistance(elapsed);
            });

        int max = *std::max_element(begin(distances), end(distances));

        for (std::size_t i = 0; i < deers.size(); ++i)
        {
            if (distances[i] == max)
                ++points[i];
        }
    }

    return *std::max_element(begin(points), end(points));
}

int Part2()
{
    return GetWinnerPoints(GetInput(), input::limit);
}

int main()
{
    // https://adventofcode.com/2015/day/14
    fmt::print("Day 14, 2015: Reindeer Olympics\n");

#ifndef NDEBUG
    auto const deers = ParseDeers(example::text);
    Assert(2 == deers.size());
    Assert((Deer{14, 10, 127, "Comet"} == deers[0]));
    Assert(14 == deers[0].GetDistance(1));
    Assert(140 == deers[0].GetDistance(10));
    Assert(140 == deers[0].GetDistance(11));
    Assert(1120 == deers[0].GetDistance(1000));
    Assert((Deer{16, 11, 162, "Dancer"} == deers[1]));
    Assert(16 == deers[1].GetDistance(1));
    Assert(160 == deers[1].GetDistance(10));
    Assert(176 == deers[1].GetDistance(11));
    Assert(1056 == deers[1].GetDistance(1000));
    Assert(689 == GetWinnerPoints(example::text, 1000));
#endif

    int const part1 = Part1();
    fmt::print("  Part 1: {}\n", part1);
    Assert(2660 == part1);

    int const part2 = Part2();
    fmt::print("  Part 2: {}\n", part2);
    Assert(1256 == part2);
}
