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
        auto [matched, sx, sy, bx, by]
            = ctre::match<"Sensor at x=(.+), y=(.+): closest beacon is at x=(.+), y=(.+)">(line);

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

static auto Part1()
{
    return CountImpossible(GetInput(), 2'000'000);
}

static auto Part2()
{
    // ####B######################
    return 0;
}

int main()
{
    // https://adventofcode.com/2022/day/15
    fmt::print("Day 15, 2022: Beacon Exclusion Zone\n");

    Assert(26 == CountImpossible(example::beacons, 10));

    auto const part1 = Part1();
    fmt::print("  Part 1: {}\n", part1);
    Assert(5525847 == part1);

    auto const part2 = Part2();
    fmt::print("  Part 2: {}\n", part2);
    // Assert( == part2);
}
