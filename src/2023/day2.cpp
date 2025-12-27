#include "day2.hpp"

#include "../cpp-utils.hpp"

#include <unordered_map>

using namespace std::literals;

using Dices = std::unordered_map<std::string_view, int>;

struct SetOfCubes
{
    int red = 0;
    int green = 0;
    int blue = 0;

    constexpr SetOfCubes() = default;

    constexpr SetOfCubes(int red_, int green_, int blue_)
        : red{red_}
        , green{green_}
        , blue{blue_}
    {
    }

    explicit SetOfCubes(std::string_view text)
    {
        for (auto dice : Split(trim_copy(text), ','))
        {
            auto const parts = Split(trim_copy(dice), ' ');

            if (parts[1] == "red"sv)
            {
                red += svtoi(parts[0]);
            }
            else if (parts[1] == "green"sv)
            {
                green += svtoi(parts[0]);
            }
            else if (parts[1] == "blue"sv)
            {
                blue += svtoi(parts[0]);
            }
        }
    }

    int64_t GetPower() const
    {
        return red * green * blue;
    }
};

static constexpr SetOfCubes bag{12, 13, 14};

static bool IsGamePossible(std::string_view game)
{
    for (auto draw : Split(game, ';'))
    {
        SetOfCubes const cubes{draw};
        if (cubes.red > bag.red || cubes.green > bag.green || cubes.blue > bag.blue)
        {
            return false;
        }
    }

    return true;
}

static int CountPossibleGames(std::string_view records)
{
    int total = 0;

    for (auto record : Split(records, '\n'))
    {
        auto const gameAndCubes = Split(record, ':');

        if (IsGamePossible(gameAndCubes[1]))
        {
            total += svtoi<int>(Split(gameAndCubes[0], ' ')[1]);
        }
    }

    return total;
}

static auto Part1()
{
    return CountPossibleGames(GetInput());
}

int64_t GetPower(std::string_view record)
{
    auto const gameAndCubes = Split(record, ':');
    SetOfCubes max;

    for (auto draw : Split(gameAndCubes[1], ';'))
    {
        SetOfCubes const cubes{draw};
        max.red = std::max(max.red, cubes.red);
        max.green = std::max(max.green, cubes.green);
        max.blue = std::max(max.blue, cubes.blue);
    }

    return max.GetPower();
}

static auto SumOfPowers(std::string_view records)
{
    int64_t total = 0;

    for (auto record : Split(records, '\n'))
    {
        total += GetPower(record);
    }

    return total;
}

static auto Part2()
{
    return SumOfPowers(GetInput());
}

int main()
{
    // https://adventofcode.com/2023/day/2
    std::println("Day 2, 2023: Cube Conundrum");

    Assert(8 == CountPossibleGames(example::records));
    Assert(48 == GetPower(Split(example::records, '\n')[0]));
    Assert(2286 == SumOfPowers(example::records));

    auto const part1 = Part1();
    std::println("  Part 1: {}", part1);
    Assert(2101 == part1);

    auto const part2 = Part2();
    std::println("  Part 2: {}", part2);
    Assert(58'269 == part2);
}
