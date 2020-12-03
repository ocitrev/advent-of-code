#include "day02.hpp"
#include "../common/assert.hpp"
#include "../common/string.hpp"
#include <algorithm>
#include <fmt/format.h>

struct PasswordPolicy
{
    int low = 0;
    int high = 0;
    char letter = '\0';
};

static std::pair<PasswordPolicy, std::string_view> Parse(std::string_view text)
{
    auto const colonPos = text.find(':');
    auto const policyText = text.substr(0, colonPos);
    auto const password = text.substr(colonPos + 2);
    auto const spacePos = policyText.find(' ');
    auto const rangeText = policyText.substr(0, spacePos);

    PasswordPolicy policy;
    auto const dashPos = rangeText.find('-');
    policy.low = svtoi(rangeText.substr(0, dashPos));
    policy.high = svtoi(rangeText.substr(dashPos + 1));
    policy.letter = policyText.substr(spacePos + 1)[0];

    return std::make_pair(policy, password);
}

namespace V1
{
    static bool IsPasswordValid(std::string_view text)
    {
        auto [policy, password] = Parse(text);
        auto const count = static_cast<int>(std::count(begin(password), end(password), policy.letter));
        return count >= policy.low && count <= policy.high;
    }
}

namespace V2
{
    static bool IsPasswordValid(std::string_view text)
    {
        auto [policy, password] = Parse(text);
        auto const charLow = password.at(static_cast<std::size_t>(policy.low - 1));
        auto const charHigh = password.at(static_cast<std::size_t>(policy.high - 1));
        return charLow != charHigh && (charLow == policy.letter || charHigh == policy.letter);
    }
}

static void Examples()
{
    Assert(2 == std::count_if(begin(example::passwords), end(example::passwords), &V1::IsPasswordValid));
    Assert(1 == std::count_if(begin(example::passwords), end(example::passwords), &V2::IsPasswordValid));
}

int main()
{
    // https://adventofcode.com/2020/day/2
    fmt::print("Day 2: Password Philosophy\n");

    Examples();

    auto const part1 = std::count_if(begin(input::passwords), end(input::passwords), &V1::IsPasswordValid);
    fmt::print("  Part 1: {}\n", part1);
    Assert(666 == part1);

    auto const part2 = std::count_if(begin(input::passwords), end(input::passwords), &V2::IsPasswordValid);
    fmt::print("  Part 2: {}\n", part2);
}
