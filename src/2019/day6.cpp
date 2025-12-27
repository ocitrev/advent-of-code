#include "day6.hpp"

#include "../common/assert.hpp"
#include "../common/string.hpp"
#include "../common/terminal.hpp"

#include <algorithm>
#include <map>
#include <print>
#include <string>
#include <vector>

static std::map<std::string, int> countCache; // NOLINT

static int CountIndirect(std::vector<std::string_view> const &list, std::string_view name)
{
    if (name == "COM")
    {
        return 0;
    }

    std::string const key{name};

    if (auto iter = countCache.find(key); iter != end(countCache))
    {
        return iter->second;
    }

    for (auto const &item : list)
    {
        auto parts = Split(item, ')');

        if (parts[1] == name)
        {
            int ret = 1 + CountIndirect(list, parts[0]);
            countCache[key] = ret;
            return ret;
        }
    }

    throw std::invalid_argument{"Not found"};
}

static std::map<std::string, std::vector<std::string>> pathCache; // NOLINT

static std::vector<std::string> GetPath(std::vector<std::string_view> const &list, std::string_view name)
{
    if (name == "COM")
    {
        return {};
    }

    std::string const key{name};

    if (auto iter = pathCache.find(key); iter != end(pathCache))
    {
        return iter->second;
    }

    for (auto const &item : list)
    {
        auto parts = Split(item, ')');

        if (parts[1] == name)
        {
            auto ret = GetPath(list, parts[0]);
            ret.emplace_back(parts[0]);
            pathCache[key] = ret;
            return ret;
        }
    }

    throw std::invalid_argument{"Not found"};
}

int main()
{
    std::println("Day 6, 2019: Universal Orbit Map");
    Assert(3 == CountIndirect(Split(example::data, '\n'), "D"));
    Assert(7 == CountIndirect(Split(example::data, '\n'), "L"));
    Assert(0 == CountIndirect(Split(example::data, '\n'), "COM"));

    pathCache.clear();
    countCache.clear();
    auto const list = Split(GetInput(), '\n');

    int count = 0;

    for (auto const &item : list)
    {
        auto last = Split(item, ')')[1];
        count += CountIndirect(list, last);
    }

    std::println("  Part1: {}", count);
    Assert(247'089 == count);

    auto you = GetPath(list, "YOU");
    auto san = GetPath(list, "SAN");

    std::sort(begin(you), end(you));
    std::sort(begin(san), end(san));

    std::vector<std::string> result;
    std::set_symmetric_difference(begin(you), end(you), begin(san), end(san), std::back_inserter(result),
        [](auto const &a, auto const &b)
        {
            return a < b;
        });

    std::println("  Part2: {}", result.size());
    Assert(442 == result.size());
}
