#include "day16.hpp"

#include "../common.hpp"

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
        {
            throw std::runtime_error{"Invalid rule text"};
        }

        name = std::string_view{&*m[1].first, static_cast<size_t>(m[1].length())};
        range1.first = svtoi({&*m[2].first, static_cast<size_t>(m[2].length())});
        range1.second = svtoi({&*m[3].first, static_cast<size_t>(m[3].length())});
        range2.first = svtoi({&*m[4].first, static_cast<size_t>(m[4].length())});
        range2.second = svtoi({&*m[5].first, static_cast<size_t>(m[5].length())});
    }

    [[nodiscard]] bool IsValid(int n) const
    {
        if (n >= range1.first && n <= range1.second)
        {
            return true;
        }

        if (n >= range2.first && n <= range2.second)
        {
            return true;
        }

        return false;
    }
};

struct Notes
{
    std::vector<Rule> rules;
    std::vector<int> myTicket;
    std::vector<std::vector<int>> nearbyTickets;

    void ParseRules(std::string_view rulesText)
    {
        for (auto line : Split(rulesText, '\n'))
        {
            rules.emplace_back(line);
        }
    }

    static std::vector<int> ParseNumbers(std::string_view list)
    {
        std::vector<int> numbers;

        for (auto number : Split(list, ','))
        {
            numbers.push_back(svtoi(number));
        }

        return numbers;
    }

    void ParseTicket(std::string_view ticket)
    {
        myTicket = ParseNumbers(ticket);
    }

    void ParseNearbyTickets(std::string_view tickets)
    {
        for (auto ticket : Split(tickets, '\n'))
        {
            nearbyTickets.push_back(ParseNumbers(ticket));
        }
    }
};

static Notes ParseInput()
{
    Notes notes;
    auto text = GetInput();
    auto parts = Split(text, "\n\n");
    notes.ParseRules(parts[0]);
    notes.ParseTicket(NextLine(parts[1]));
    notes.ParseNearbyTickets(NextLine(parts[2]));
    return notes;
}

static int Part1()
{
    auto const notes = ParseInput();
    std::vector<int> invalidIds;

    for (auto &&ticket : notes.nearbyTickets)
    {
        for (auto id : ticket)
        {
            if (not std::any_of(begin(notes.rules), end(notes.rules),
                    [id](Rule const &r)
                    {
                        return r.IsValid(id);
                    }))
            {
                invalidIds.push_back(id);
            }
        }
    }

    return std::accumulate(begin(invalidIds), end(invalidIds), 0);
}

static bool IsValidTicket(std::span<int const> ticket, std::span<Rule const> rules)
{
    for (auto id : ticket)
    {
        if (not std::any_of(begin(rules), end(rules),
                [id](Rule const &r)
                {
                    return r.IsValid(id);
                }))
        {
            return false;
        }
    }

    return true;
}

static int64_t Part2()
{
    auto notes = ParseInput();
    int const nbRules = static_cast<int>(notes.rules.size());
    std::vector<std::span<int const>> validTickets{begin(notes.nearbyTickets), end(notes.nearbyTickets)};
    auto last = std::remove_if(begin(validTickets), end(validTickets),
        [&](auto &&ticket)
        {
            return not IsValidTicket(ticket, notes.rules);
        });
    validTickets.erase(last, end(validTickets));
    validTickets.push_back(notes.myTicket);

    for (auto &&rule : notes.rules)
    {
        for (int r = 0; r != nbRules; ++r)
        {
            if (std::all_of(begin(validTickets), end(validTickets),
                    [&](auto const &ticket)
                    {
                        return rule.IsValid(ticket[static_cast<size_t>(r)]);
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

        for (auto const &rule : notes.rules)
        {
            if (rule.possibleIndex.size() != 1)
            {
                continue;
            }

            int const toRemove = *rule.possibleIndex.begin();

            if (done.find(toRemove) != end(done))
            {
                continue;
            }

            done.insert(toRemove);

            for (auto &&r : notes.rules)
            {
                if (&r != &rule)
                {
                    removed |= r.possibleIndex.erase(toRemove) != 0;
                }
            }
        }

        if (not removed)
        {
            break;
        }
    }

    int64_t result = 1;

    for (auto const &rule : notes.rules)
    {
        if (starts_with(rule.name, "departure"))
        {
            Assert(rule.possibleIndex.size() == 1);
            auto const idx = static_cast<size_t>(*rule.possibleIndex.begin());
            result *= static_cast<int64_t>(notes.myTicket.at(idx));
        }
    }

    return result;
}

int main()
{
    // https://adventofcode.com/2020/day/16
    std::println("Day 16, 2020: Ticket Translation");

    auto const part1 = Part1();
    std::println("  Part 1: {}", part1);
    Assert(20'060 == part1);

    auto const part2 = Part2();
    std::println("  Part 2: {}", part2);
    Assert(2'843'534'243'843 == part2);
}
