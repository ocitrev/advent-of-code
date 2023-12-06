#include "day6.hpp"
#include "../common.hpp"

struct Race
{
    int64_t time = 0;
    int64_t distance = 0;

    int64_t GetWinableCount() const
    {
        int64_t count = 0;

        for (int64_t t = 0; t != time; ++t)
        {
            if (int64_t const d = (time - t) * t; d > distance)
            {
                ++count;
            }
            else if (count != 0)
            {
                break;
            }
        }

        return count;
    }
};

static std::vector<Race> ParseRaces(std::string_view text)
{
    auto times = NextToken(text, '\n');
    auto distances = NextToken(text, '\n');

    SkipToken(times, ':');
    trim(times);

    std::vector<Race> races;

    for (auto time : Split(times, ' '))
    {
        if (time.empty())
        {
            continue;
        }

        races.push_back(Race{svtoi<int64_t>(time), 0});
    }

    SkipToken(distances, ':');
    trim(distances);

    auto iter = begin(races);
    for (auto distance : Split(distances, ' '))
    {
        if (distance.empty())
        {
            continue;
        }

        iter->distance = svtoi<int64_t>(distance);
        ++iter;
    }

    return races;
}

auto CalculateMargin(std::string_view text)
{
    auto const races = ParseRaces(text);

    return std::transform_reduce(begin(races), end(races), int64_t{1}, std::multiplies{},
        [](Race r)
        {
            return r.GetWinableCount();
        });
}

static auto Part1()
{
    return CalculateMargin(GetInput());
}

static Race ParseRace2(std::string_view text)
{
    auto times = NextToken(text, '\n');
    auto distances = NextToken(text, '\n');

    Race race;

    for (char c : times)
    {
        if (std::isdigit(c))
        {
            race.time *= 10;
            race.time += c - '0';
        }
    }

    for (char c : distances)
    {
        if (std::isdigit(c))
        {
            race.distance *= 10;
            race.distance += c - '0';
        }
    }

    return race;
}

auto CalculateMargin2(std::string_view text)
{
    Race r = ParseRace2(text);
    return r.GetWinableCount();
}

static auto Part2()
{
    return CalculateMargin2(GetInput());
}

int main()
{
    // https://adventofcode.com/2023/day/6
    fmt::print("Day 6, 2023: Wait For It\n");

    Assert(288 == CalculateMargin(example::races));
    Assert(71503 == CalculateMargin2(example::races));

    auto const part1 = Part1();
    fmt::print("  Part 1: {}\n", part1);
    Assert(293046 == part1);

    auto const part2 = Part2();
    fmt::print("  Part 2: {}\n", part2);
    Assert(35150181 == part2);
}
