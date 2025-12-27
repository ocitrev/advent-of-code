#include "day10.hpp"

#include "../common.hpp"

constexpr static int GetErrorScore(char c)
{
    switch (c)
    {
        case ')':
            return 3;
        case ']':
            return 57;
        case '}':
            return 1197;
        case '>':
            return 25'137;
    }

    return 0;
}

static char GetCorruptedChar(std::string_view line, std::vector<char> &stack)
{
    for (char c : line)
    {
        switch (c)
        {
            case '(':
            case '{':
            case '[':
            case '<':
                stack.push_back(c);
                break;
            case ')':
                if (stack.back() != '(')
                {
                    return c;
                }
                stack.pop_back();
                break;
            case '}':
                if (stack.back() != '{')
                {
                    return c;
                }
                stack.pop_back();
                break;
            case ']':
                if (stack.back() != '[')
                {
                    return c;
                }
                stack.pop_back();
                break;
            case '>':
                if (stack.back() != '<')
                {
                    return c;
                }
                stack.pop_back();
                break;
        }
    }

    return '\0';
}

constexpr static int64_t GetAutoCompleteScore(char c)
{
    switch (c)
    {
        case '(':
        case ')':
            return 1;
        case '{':
        case '}':
            return 3;
        case '[':
        case ']':
            return 2;
        case '<':
        case '>':
            return 4;
    }

    return 0;
}

static char GetCorruptedChar(std::string_view line)
{
    std::vector<char> stack;
    return GetCorruptedChar(line, stack);
}

static int64_t AutoCompleteScore(std::string_view incomplete)
{
    return std::accumulate(begin(incomplete), end(incomplete), 0LL,
        [](int64_t total, char c)
        {
            return (total * 5) + GetAutoCompleteScore(c);
        });
}

static int64_t AutoCompleteScore(std::vector<char> const &incomplete)
{
    return AutoCompleteScore(std::string_view{incomplete.data(), incomplete.size()});
}

static int Part1(std::string_view lines)
{
    int score = 0;

    for (auto line : Split(lines, '\n'))
    {
        auto const c = GetCorruptedChar(line);
        if (c == '\0')
        {
            continue;
        }

        score += GetErrorScore(c);
    }

    return score;
}

static int64_t Part2(std::string_view lines)
{
    std::vector<int64_t> scores;

    for (auto line : Split(lines, '\n'))
    {
        std::vector<char> stack;

        if (GetCorruptedChar(line, stack) != '\0')
        {
            continue;
        }

        std::ranges::reverse(stack);
        auto const score = AutoCompleteScore(stack);
        scores.push_back(score);
    }

    size_t const middle = scores.size() / 2;
    std::ranges::nth_element(scores, begin(scores) + static_cast<ptrdiff_t>(middle));
    return scores[middle];
}

int main()
{
    // https://adventofcode.com/2021/day/10
    std::println("Day 10, 2021: Syntax Scoring");

    Assert(']' == GetCorruptedChar("(]"));
    Assert('>' == GetCorruptedChar("{()()()>"));
    Assert('>' == GetCorruptedChar("{()()()>"));
    Assert('}' == GetCorruptedChar("(((()))}"));
    Assert(26'397 == Part1(example::lines));
    Assert(294 == AutoCompleteScore("])}>"));
    Assert(288'957 == Part2(example::lines));

    std::println("Debug: {}", AutoCompleteScore("])}>"));

    auto const part1 = Part1(GetInput());
    std::println("  Part 1: {}", part1);
    Assert(288'291 == part1);

    auto const part2 = Part2(GetInput());
    std::println("  Part 2: {}", part2);
    Assert(820'045'242 == part2);
}
