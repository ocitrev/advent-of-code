#include "day15.hpp"

#include "../common.hpp"
#include "../common/range.hpp"

#include <ctre.hpp>
#include <optional>

struct Sensor
{
    Point2d pos;
    Point2d closestBeacon;

    int Radius() const
    {
        return pos.Distance(closestBeacon);
    }

    std::optional<Range> GetSizeAtHeight(int y) const
    {
        int const dist = abs(pos.y - y);
        int const halfWidth = Radius() - dist;

        if (halfWidth <= 0)
        {
            return std::nullopt;
        }

        return Range{pos.x - halfWidth, pos.x + halfWidth};
    }
};

static auto ParseInput(std::string_view text)
{
    std::vector<Sensor> sensors;

    for (auto line : Split(text, '\n'))
    {
        auto [matched, sx, sy, bx, by] = ctre::match<"Sensor at x=(.+), y=(.+): closest beacon is at x=(.+), y=(.+)">(line);

        if (matched)
        {
            sensors.push_back(Sensor{{svtoi(sx), svtoi(sy)}, {svtoi(bx), svtoi(by)}});
        }
        else
        {
            throw std::invalid_argument("text");
        }
    }

    return sensors;
}

static auto CountImpossible(std::vector<Sensor> const &sensors, int y)
{
    std::vector<Range> ranges;

    for (auto const &sensor : sensors)
    {
        if (auto width = sensor.GetSizeAtHeight(y); width.has_value())
        {
            if (sensor.closestBeacon.y == y && sensor.closestBeacon.x >= width->low && sensor.closestBeacon.x <= width->high)
            {
                if (sensor.closestBeacon.x == width->low)
                {
                    ranges.push_back({width->low + 1, width->high});
                }
                else if (sensor.closestBeacon.x == width->high)
                {
                    ranges.push_back({width->low, width->high - 1});
                }
                else
                {
                    ranges.push_back(Range{width->low, sensor.closestBeacon.x - 1});
                    ranges.push_back(Range{sensor.closestBeacon.x + 1, width->high});
                }
            }
            else
            {
                ranges.push_back(*width);
            }
        }
    }

    int count = 0;

    for (auto const &r : MergeRanges(ranges))
    {
        count += r.Length();
    }

    return count;
}

static auto CountImpossible(std::string_view sensors, int y)
{
    return CountImpossible(ParseInput(sensors), y);
}

static int64_t FindBeacon(std::vector<Sensor> const &sensors, int limit)
{
    Range const limitWidth{0, limit};
    bool const stdoutIsConsole = IsTerminal(stdout);

    // force brute ... il doit y avoir une meilleure solution
    for (int y = limit; y >= 0; --y)
    {
        std::vector<Range> ranges;

        if (stdoutIsConsole && y % 10'000 == 0)
        {
            fmt::print("  {}/{} {}%          \r", y, limit, 100 - (y * 100LL / limit));
        }

        for (auto const &sensor : sensors)
        {
            if (auto width = sensor.GetSizeAtHeight(y); width.has_value())
            {
                if (width->Intersection(limitWidth))
                {
                    ranges.push_back(*width);
                }
            }
        }

        ranges = MergeRanges(ranges);

        if (ranges.size() > 1)
        {
            static constexpr int64_t tuningFrequency = 4'000'000;
            Range a{ranges[0].high + 1, ranges[1].low - 1};
            Range b{ranges[1].high + 1, ranges[0].low - 1};

            if (a.Length() == 1)
            {
                return a.low * tuningFrequency + y;
            }

            return b.low * tuningFrequency + y;
        }
    }

    return 0;
}

static auto FindBeacon(std::string_view sensors, int limit)
{
    return FindBeacon(ParseInput(sensors), limit);
}

static auto Part1()
{
    return CountImpossible(GetInput(), 2'000'000);
}

static auto Part2()
{
    return FindBeacon(GetInput(), 4'000'000);
}

int main()
{
    // https://adventofcode.com/2022/day/15
    fmt::print("Day 15, 2022: Beacon Exclusion Zone\n");

    Assert(26 == CountImpossible(example::beacons, 10));
    Assert(56'000'011 == FindBeacon(example::beacons, 20));

    auto const part1 = Part1();
    fmt::print("  Part 1: {}\n", part1);
    Assert(5'525'847 == part1);

    auto const part2 = Part2();
    fmt::print("  Part 2: {}\n", part2);
    Assert(13'340'867'187'704 == part2);
}
