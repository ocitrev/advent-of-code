#include "day12.hpp"

#include "../common/assert.hpp"

#include <algorithm>
#include <nlohmann/json.hpp>
#include <numeric>
#include <print>
#include <regex>

using json = nlohmann::json;

static int SumAllNumbers(json const &value, bool checkExclusion)
{
    if (value.is_number_integer())
    {
        return value.get<int>();
    }

    if (value.is_array())
    {
        int sum = 0;

        for (auto const &item : value)
        {
            sum += SumAllNumbers(item, checkExclusion);
        }

        return sum;
    }

    if (value.is_object())
    {
        if (checkExclusion)
        {
            for (auto const &[key, val] : value.items())
            {
                if (val.is_string() && val.get<std::string>() == "red")
                {
                    return 0;
                }
            }
        }

        int sum = 0;
        for (auto const &[key, val] : value.items())
        {
            sum += SumAllNumbers(val, checkExclusion);
        }
        return sum;
    }

    return 0;
}

static int SumAllNumbers(std::string_view text)
{
    return SumAllNumbers(json::parse(text), false);
}

static int SumAllNumbersNoRed(std::string_view text)
{
    return SumAllNumbers(json::parse(text), true);
}

int main()
{
    // https://adventofcode.com/2015/day/12
    std::print("Day 12, 2015: JSAbacusFramework.io\n");

    Assert(6 == SumAllNumbers(R"([1,2,3])"));
    Assert(6 == SumAllNumbers(R"({"a":2,"b":4})"));
    Assert(3 == SumAllNumbers(R"([[[3]]])"));
    Assert(3 == SumAllNumbers(R"({"a":{"b":4},"c":-1})"));
    Assert(0 == SumAllNumbers(R"({"a":[-1,1]})"));
    Assert(0 == SumAllNumbers(R"([-1,{"a":1}])"));
    Assert(0 == SumAllNumbers(R"([])"));
    Assert(0 == SumAllNumbers(R"({})"));

    Assert(6 == SumAllNumbersNoRed(R"([1,2,3])"));
    Assert(4 == SumAllNumbersNoRed(R"([1,{"c":"red","b":2},3])"));
    Assert(0 == SumAllNumbersNoRed(R"({"d":"red","e":[1,2,3,4],"f":5})"));
    Assert(6 == SumAllNumbersNoRed(R"([1,"red",5])"));

    int const part1 = SumAllNumbers(GetInput());
    std::print("  Part 1: {}\n", part1);
    Assert(191'164 == part1);

    int const part2 = SumAllNumbersNoRed(GetInput());
    std::print("  Part 2: {}\n", part2);
    Assert(87'842 == part2);
}
