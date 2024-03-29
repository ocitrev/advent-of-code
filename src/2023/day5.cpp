#include "day5.hpp"
#include "../common.hpp"
#include <execution>

using namespace std::literals;
using integer = int64_t;

struct MapRange
{
    integer destination;
    integer source;
    integer length;

    MapRange(std::string_view text)
    {
        Tokenizer t(text);
        destination = svtoi<integer>(t.NextToken(' '));
        source = svtoi<integer>(t.NextToken(' '));
        length = svtoi<integer>(t.NextToken(' '));
    }
};

struct Mapping
{
    std::vector<MapRange> ranges;

    Mapping() = default;

    Mapping(std::string_view text)
    {
        for (auto line : Split(text))
        {
            ranges.emplace_back(line);
        }
    }

    [[nodiscard]] integer Convert(integer value) const
    {
        for (auto const &range : ranges)
        {
            if (value >= range.source)
            {
                if (auto const delta = value - range.source; delta < range.length)
                {
                    return range.destination + delta;
                }
            }
        }

        return value;
    }

    [[nodiscard]] integer Unconvert(integer value) const
    {
        for (auto const &range : ranges)
        {
            if (value >= range.destination)
            {
                if (auto const delta = value - range.destination; delta < range.length)
                {
                    return range.source + delta;
                }
            }
        }

        return value;
    }
};

struct Almanac
{
    std::vector<integer> seeds;

    Mapping seedToSoil;
    Mapping soilToFertilizer;
    Mapping fertilizerToWater;
    Mapping waterToLight;
    Mapping lightToTemperature;
    Mapping temperatureToHumidity;
    Mapping humidityToLocation;

    Almanac(std::string_view text)
    {
        Mapping *mapping = nullptr;

        for (auto line : Split(text))
        {
            if (seeds.empty())
            {
                for (auto seed : Split(line.substr(7), ' '))
                {
                    seeds.push_back(svtoi<integer>(seed));
                }
            }
            else
            {
                if (mapping == nullptr)
                {
                    if (line.empty())
                    {
                        continue;
                    }

                    Tokenizer t(line);
                    auto const name = t.NextToken(' ');

                    if (name == "seed-to-soil")
                    {
                        mapping = &seedToSoil;
                    }
                    else if (name == "soil-to-fertilizer")
                    {
                        mapping = &soilToFertilizer;
                    }
                    else if (name == "fertilizer-to-water")
                    {
                        mapping = &fertilizerToWater;
                    }
                    else if (name == "water-to-light")
                    {
                        mapping = &waterToLight;
                    }
                    else if (name == "light-to-temperature")
                    {
                        mapping = &lightToTemperature;
                    }
                    else if (name == "temperature-to-humidity")
                    {
                        mapping = &temperatureToHumidity;
                    }
                    else if (name == "humidity-to-location")
                    {
                        mapping = &humidityToLocation;
                    }
                }
                else if (line.empty())
                {
                    mapping = nullptr;
                }
                else
                {
                    mapping->ranges.emplace_back(line);
                }
            }
        }
    }

    [[nodiscard]] integer GetLocation(integer seed)
    {
        auto const soil = seedToSoil.Convert(seed);
        auto const fertilizer = soilToFertilizer.Convert(soil);
        auto const water = fertilizerToWater.Convert(fertilizer);
        auto const light = waterToLight.Convert(water);
        auto const temperature = lightToTemperature.Convert(light);
        auto const humidity = temperatureToHumidity.Convert(temperature);
        return humidityToLocation.Convert(humidity);
    }

    [[nodiscard]] integer GetSeed(integer location)
    {
        auto const humidity = humidityToLocation.Unconvert(location);
        auto const temperature = temperatureToHumidity.Unconvert(humidity);
        auto const light = lightToTemperature.Unconvert(temperature);
        auto const water = waterToLight.Unconvert(light);
        auto const fertilizer = fertilizerToWater.Unconvert(water);
        auto const soil = soilToFertilizer.Unconvert(fertilizer);
        return seedToSoil.Unconvert(soil);
    }

    [[nodiscard]] integer GetLowestLocation()
    {
        std::vector<integer> locations(seeds.size());
        std::transform(begin(seeds), end(seeds), begin(locations),
            [this](auto seed)
            {
                return GetLocation(seed);
            });

        return *std::min_element(begin(locations), end(locations));
    }

    [[nodiscard]] bool IsValidSeed(integer seed) const
    {
        auto const lastSeed = end(seeds);
        for (auto iter = begin(seeds); iter != lastSeed; iter += 2)
        {
            auto const start = *iter;
            auto const count = *std::next(iter);

            if (seed >= start && seed < start + count)
            {
                return true;
            }
        }

        return false;
    }

    [[nodiscard]] integer GetLowestLocationFromSeedRanges()
    {
        integer location = 0;
        bool const stdoutIsConsole = IsTerminal(stdout);
        auto ts = std::chrono::steady_clock::now();

        while (true)
        {
            if (IsValidSeed(GetSeed(location)))
            {
                if (stdoutIsConsole)
                {
                    fmt::print(CSI("2K"));
                }

                return location;
            }

            ++location;

            if (stdoutIsConsole)
            {
                if (auto const now = std::chrono::steady_clock::now(); now - ts > 100ms)
                {
                    ts = now;
                    fmt::print("  testing location {:L} ...\r", location);
                }
            }
        }
    }
};

static auto GetLowestLocation(std::string_view almanacText)
{
    Almanac almanac(almanacText);
    return almanac.GetLowestLocation();
}

static auto Part1()
{
    return GetLowestLocation(GetInput());
}

static auto GetLowestLocationFromSeedRanges(std::string_view almanacText)
{
    Almanac almanac(almanacText);
    return almanac.GetLowestLocationFromSeedRanges();
}

static auto Part2()
{
    return GetLowestLocationFromSeedRanges(GetInput());
}

int main()
{
    // https://adventofcode.com/2023/day/5
    fmt::print("Day 5, 2023: If You Give A Seed A Fertilizer\n");

    Assert(35 == GetLowestLocation(example::almanac));
    Assert(46 == GetLowestLocationFromSeedRanges(example::almanac));

    auto const part1 = Part1();
    fmt::print("  Part 1: {}\n", part1);
    Assert(389056265 == part1);

    auto const part2 = Part2();
    fmt::print("  Part 2: {}\n", part2);
    Assert(137516820 == part2);
}
