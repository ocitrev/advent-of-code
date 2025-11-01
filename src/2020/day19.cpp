#include "day19.hpp"

#include "../common/assert.hpp"
#include "../common/string.hpp"

#include <fmt/format.h>
#include <unordered_map>

enum struct Type
{
    Literal,
    Rule,
};

struct RulePart
{
    Type type;
    std::string_view text;
};

struct Rule
{
    std::vector<RulePart> left;
    std::vector<RulePart> right;

    void Parse(std::string_view text)
    {
        auto exprs = Split(text, '|');

        Parse(left, trim_copy(exprs[0]));

        if (exprs.size() > 1)
        {
            Parse(right, trim_copy(exprs[1]));
        }
    }

    void Parse(std::vector<RulePart> &parts, std::string_view text)
    {
        for (auto part : Split(text, ' '))
        {
            trim(part);

            if (part[0] == '"')
            {
                parts.push_back(RulePart{Type::Literal, part.substr(1, part.size() - 2)});
            }
            else
            {
                parts.push_back(RulePart{Type::Rule, part});
            }
        }
    }
};

struct Rules
{
    std::unordered_map<std::string_view, Rule> rules;

    void Parse(std::string_view rulesText)
    {
        for (auto line : Split(rulesText, '\n'))
        {
            auto const parts = Split(line, ":");
            Add(trim_copy(parts[0]), trim_copy(parts[1]));
        }
    }

    void Add(std::string_view id, std::string_view text)
    {
        Rule &r = rules[id];
        r.Parse(text);
    }

    bool IsValid(std::string_view text)
    {
        Assert(rules.find("0") != end(rules));
        Rule const &rule0 = rules["0"];
        return IsValid(text, 0, rule0) == text.size();
    }

    size_t IsValid(std::string_view text, size_t current, Rule const &rule)
    {
        if (auto const nb = IsValid(text, current, rule.left); nb == rule.left.size())
        {
            return nb;
        }

        return IsValid(text, current, rule.right);
    }

    size_t IsValid(std::string_view text, size_t current, std::vector<RulePart> const &parts)
    {
        if (parts.empty())
        {
            return 0;
        }

        size_t const start = current;

        for (RulePart const &p : parts)
        {
            auto const nb = IsValid(text, current, p);

            if (nb == 0)
            {
                break;
            }

            current += nb;
        }

        return current - start;
    }

    size_t IsValid(std::string_view text, size_t current, RulePart const &rule)
    {
        if (rule.type == Type::Literal)
        {
            return text[current] == rule.text[0] ? 1 : 0;
        }

        return IsValid(text, current, rules[rule.text]);
    }
};

static int Part1(std::string_view rulesText, std::string_view values)
{
    Rules rules;
    rules.Parse(rulesText);
    int count = 0;

    for (auto line : Split(values, '\n'))
    {
        if (rules.IsValid(line))
        {
            ++count;
        }
    }

    return count;
}

static int Part2()
{
    return 0;
}

int main()
{
    // https://adventofcode.com/2020/day/19
    fmt::print("Day 19, 2020: Monster Messages\n");

    // Assert(2 == Part1(example::rules1, "aab\naba"));

    auto const part1 = Part1(example::rules2, example::values2);
    // auto const part1 = Part1(input::rules, input::values);
    fmt::print("  Part 1: {}\n", part1);

    auto const part2 = Part2();
    fmt::print("  Part 2: {}\n", part2);
}
