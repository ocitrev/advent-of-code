#include "day16.hpp"
#include "../common/assert.hpp"
#include "../common/string.hpp"
#include <fmt/format.h>
#include <map>

using Aunt = std::map<std::string, int>;

static void ParseValue(Aunt &values, std::string_view text)
{
    auto parts = Split(text, ':');
    trim(parts[0]);
    trim(parts[1]);
    values[std::string{parts[0]}] = svtoi(parts[1]);
}

static Aunt ParseMFCSAM(std::string_view text)
{
    Aunt result;

    for (auto &&line : Split(text, '\n'))
    {
        ParseValue(result, line);
    }

    return result;
}

static Aunt ParseAunt(std::string_view text)
{
    Aunt result;
    auto firstColon = text.find(':');
    auto name = text.substr(0, firstColon);
    result["name"] = svtoi(name.substr(name.find(' ') + 1));

    for (auto const &v : Split(text.substr(firstColon + 1), ','))
    {
        ParseValue(result, v);
    }

    return result;
}

static std::vector<Aunt> ParseAunts(std::string_view text)
{
    std::vector<Aunt> aunts;

    for (auto &&line : Split(text, '\n'))
    {
        aunts.emplace_back(ParseAunt(line));
    }

    return aunts;
}

template <typename PredicateT>
static bool IsValue(Aunt const &aunt, std::string const &name, int expected, PredicateT &&pred)
{
    auto iter = aunt.find(name);

    if (iter == end(aunt))
        return false;

    return pred(iter->second, expected);
}

template <typename GetPredicateFunc>
static int FindBestMatch(GetPredicateFunc &&getPredicate)
{
    auto const MFCSAMResult = ParseMFCSAM(input::MFCSAM);
    auto const aunts = ParseAunts(input::Sues);

    for (auto const &aunt : aunts)
    {
        bool candidate = true;

        for (auto const &[wantedName, wantedValue] : MFCSAMResult)
        {
            if (IsValue(aunt, wantedName, wantedValue, getPredicate(wantedName)))
            {
                candidate = false;
                break;
            }
        }

        if (candidate)
        {
            return aunt.find("name")->second;
        }
    }

    return 0;
}

static int Part1()
{
    return FindBestMatch(
        []([[maybe_unused]] std::string const &name)
        {
            return std::not_equal_to{};
        });
}

static int Part2()
{
    return FindBestMatch(
        [](std::string const &name) -> std::function<bool(int, int)>
        {
            if (name == "cats" || name == "trees")
                return std::less_equal{};

            if (name == "pomeranians" || name == "goldfish")
                return std::greater_equal{};

            return std::not_equal_to{};
        });
}

int main()
{
    // https://adventofcode.com/2015/day/16
    fmt::print("Day 16, 2015: Aunt Sue\n");

    auto const part1 = Part1();
    fmt::print("  Part 1: {}\n", part1);
    Assert(40 == part1);

    auto const part2 = Part2();
    fmt::print("  Part 2: {}\n", part2);
    Assert(241 == part2);
}
