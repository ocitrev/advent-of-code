#include "day07.hpp"
#include "../common/assert.hpp"
#include "../common/string.hpp"
#include <fmt/format.h>
#include <gsl/gsl>
#include <map>
#include <set>
#include <string>

struct Rule
{
    std::string_view color;
    std::map<std::string_view, int> contains;
};

static Rule ParseRule(std::string_view ruleText)
{
    using namespace std::string_view_literals;
    auto parts = Split(ruleText, "bags contain"sv);
    Rule rule{trim_copy(parts[0]), {}};
    parts = Split(parts[1], ","sv);

    for (auto &&p : parts)
    {
        trim(p);
        int nbBags = 0;
        // NOLINTNEXTLINE cppcoreguidelines-pro-bounds-pointer-arithmetic
        auto const result = std::from_chars(p.data(), p.data() + p.size(), nbBags);

        if (result.ec != std::errc{})
            continue;

        p.remove_prefix(static_cast<size_t>(result.ptr - p.data()));
        p.remove_suffix(p.size() - p.find("bag"sv));
        trim(p);
        rule.contains.insert(std::make_pair(p, nbBags));
    }

    return rule;
}

static std::vector<Rule> ParseRules(gsl::span<std::string_view const> rawRules)
{
    std::vector<Rule> rules;
    for (auto &&rule : rawRules)
    {
        rules.emplace_back(ParseRule(rule));
    }

    return rules;
}

static std::vector<std::string_view> GetParents(std::vector<Rule> const &rules, std::string_view bag)
{
    std::vector<std::string_view> parents;
    for (auto &&r : rules)
    {
        if (auto iter = r.contains.find(bag); iter != end(r.contains))
        {
            parents.emplace_back(r.color);
        }
    }

    return parents;
}

void GetAllParents(std::vector<Rule> const &rules, std::string_view bag, std::set<std::string_view> &parents)
{
    for (auto &&p : GetParents(rules, bag))
    {
        parents.insert(p);
        GetAllParents(rules, p, parents);
    }
}

int GetAllParentsCount(std::vector<Rule> const &rules, std::string_view bag)
{
    std::set<std::string_view> parents;
    GetAllParents(rules, bag, parents);
    return static_cast<int>(parents.size());
}

int CountBags(std::vector<Rule> const &rules, std::string_view bag)
{
    int total = 0;

    auto iter = std::find_if(begin(rules), end(rules), [&](Rule const &rule) {
        return rule.color == bag;
    });

    for (auto &&child : iter->contains)
    {
        total += child.second;
        total += CountBags(rules, child.first) * child.second;
    }

    return total;
}

void Example()
{
#ifndef NDEBUG
    auto const rules1 = ParseRules(example::rules1);
    auto const rules2 = ParseRules(example::rules2);

    Assert(4 == GetAllParentsCount(rules1, input::mybag));
    Assert(32 == CountBags(rules1, input::mybag));
    Assert(126 == CountBags(rules2, input::mybag));
#endif
}

int Part1()
{
    auto const rules = ParseRules(input::rules);
    return GetAllParentsCount(rules, input::mybag);
}

int Part2()
{
    auto const rules = ParseRules(input::rules);
    return CountBags(rules, input::mybag);
}

int main()
{
    // https://adventofcode.com/2020/day/7
    fmt::print("Day 07: Handy Haversacks\n");

    Example();

    auto const part1 = Part1();
    fmt::print("  Part 1: {}\n", part1);
    Assert(224 == part1);

    auto const part2 = Part2();
    fmt::print("  Part 2: {}\n", part2);
    Assert(1488 == part2);
}
