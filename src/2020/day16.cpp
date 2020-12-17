#include "day16.hpp"
#include "../common/assert.hpp"
#include "../common/string.hpp"
#include <fmt/format.h>
#include <gsl/gsl>
#include <numeric>
#include <regex>
#include <set>

struct Rule
{
    std::string_view name;
    std::pair<int, int> range1;
    std::pair<int, int> range2;
    std::set<int> possibleIndex;

    Rule(std::string_view text)
    {
        std::regex re(R"~(([^:]+): (\d+)-(\d+) or (\d+)-(\d+))~");
        std::match_results<std::string_view::const_iterator> m;

        if (not std::regex_match(begin(text), end(text), m, re))
            throw std::runtime_error{"Invalid rule text"};

        name = std::string_view{m[1].first, m[1].second};
        range1.first = svtoi({m[2].first, m[2].second});
        range1.second = svtoi({m[3].first, m[3].second});
        range2.first = svtoi({m[4].first, m[4].second});
        range2.second = svtoi({m[5].first, m[5].second});
    }

    [[nodiscard]] bool IsValid(int n) const
    {
        if (n >= range1.first && n <= range1.second)
            return true;

        if (n >= range2.first && n <= range2.second)
            return true;

        return false;
    }
};

int Part1()
{
    std::vector<Rule> rules;
    for (auto &&line : Split(input::rules, "\n"))
        rules.emplace_back(line);

    std::vector<int> invalidIds;
    for (auto &&ticket : input::nearbyTickets)
    {
        for (auto id : ticket)
        {
            if (not std::any_of(begin(rules), end(rules), [id](Rule const &r) {
                    return r.IsValid(id);
                }))
            {
                invalidIds.push_back(id);
            }
        }
    }

    return std::accumulate(begin(invalidIds), end(invalidIds), 0);
}

static bool IsValidTicket(gsl::span<int const> ticket, gsl::span<Rule> rules)
{
    for (auto id : ticket)
    {
        if (not std::any_of(begin(rules), end(rules), [id](Rule const &r) {
                return r.IsValid(id);
            }))
        {
            return false;
        }
    }

    return true;
}

int64_t Part2()
{
    std::vector<Rule> rules;
    for (auto &&line : Split(input::rules, "\n"))
        rules.emplace_back(line);

    int const nbRules = static_cast<int>(rules.size());
    std::vector<gsl::span<int const>> validTickets{begin(input::nearbyTickets), end(input::nearbyTickets)};
    auto last = std::remove_if(begin(validTickets), end(validTickets), [&](auto &&ticket) {
        return not IsValidTicket(ticket, rules);
    });
    validTickets.erase(last, end(validTickets));
    validTickets.emplace_back(input::myTicket);

    for (auto &&rule : rules)
    {
        for (int r = 0; r != nbRules; ++r)
        {
            if (std::all_of(begin(validTickets), end(validTickets), [&](auto const &ticket) {
                    return rule.IsValid(gsl::at(ticket, r));
                }))
            {
                rule.possibleIndex.insert(r);
            }
        }
    }

    std::set<int> done;

    while (true)
    {
        bool removed = false;

        for (auto const &rule : rules)
        {
            if (rule.possibleIndex.size() != 1)
                continue;
            
            int const toRemove = *rule.possibleIndex.begin();

            if (done.contains(toRemove))
                continue;
            
            done.insert(toRemove);

            for (auto &&r : rules)
            {
                if (&r != &rule)
                {
                    removed |= r.possibleIndex.erase(toRemove) != 0;
                }
            }
        }

        if (not removed)
            break;
    }

    int64_t result = 1;

    for (auto const &rule : rules)
    {
        if (starts_with(rule.name, "departure"))
        {
            Assert(rule.possibleIndex.size() == 1);
            auto const idx = static_cast<gsl::index>(*rule.possibleIndex.begin());
            result *= gsl::at(input::myTicket, idx);
        }
    }

    return result;
}

int main()
{
    // https://adventofcode.com/2020/day/16
    fmt::print("Day 16: Ticket Translation\n");

    auto const part1 = Part1();
    fmt::print("  Part 1: {}\n", part1);
    Assert(20060 == part1);

    auto const part2 = Part2();
    fmt::print("  Part 2: {}\n", part2);
    Assert(2843534243843 == part2);
}
