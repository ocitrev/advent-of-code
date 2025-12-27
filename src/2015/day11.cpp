#include "day11.hpp"

#include "../common/assert.hpp"

#include <print>

bool IsForbiddenLetter(char c)
{
    return c == 'i' || c == 'o' || c == 'l';
}

void Increment(std::string &number, bool checkForbidden)
{
    for (auto iter = rbegin(number); iter != rend(number); ++iter)
    {
        if (*iter == 'z')
        {
            *iter = 'a';
        }
        else
        {
            *iter = ++(*iter);
            break;
        }
    }

    if (not checkForbidden)
    {
        return;
    }

    bool setToZero = false;
    for (char &c : number)
    {
        if (setToZero)
        {
            c = 'a';
        }
        else if (IsForbiddenLetter(c))
        {
            setToZero = true;
            ++c;
        }
    }
}

bool HasStraight(std::string_view text)
{
    char prev1 = '\0';
    char prev2 = '\0';

    for (char c : text)
    {
        if (prev1 != '\0' && prev2 != '\0')
        {
            if (prev1 + 2 == prev2 + 1 && c == prev1 + 2)
            {
                return true;
            }
        }

        prev1 = prev2;
        prev2 = c;
    }

    return false;
}

bool HasRepeats(std::string_view text)
{
    char prev = '\0';
    int nbRepeats = 0;

    for (char c : text)
    {
        if (prev != '\0' && prev == c)
        {
            ++nbRepeats;
            prev = '\0';
            continue;
        }

        prev = c;
    }

    return nbRepeats >= 2;
}

bool HasForbiddenLetters(std::string_view text)
{
    return text.find_first_of("iol") != std::string_view::npos;
}

bool IsValidPassword(std::string_view text)
{
    if (HasForbiddenLetters(text))
    {
        return false;
    }

    if (not HasStraight(text))
    {
        return false;
    }

    if (not HasRepeats(text))
    {
        return false;
    }

    return true;
}

std::string NextPassword(std::string_view text)
{
    std::string nextPassword{text};
    do
    {
        Increment(nextPassword, true);
    }
    while (not IsValidPassword(nextPassword));
    return nextPassword;
}

int main()
{
    // https://adventofcode.com/2015/day/11
    std::println("Day 11, 2015: Corporate Policy");

    Assert(HasStraight("hijklmmn"));
    Assert(HasRepeats("abbceffg"));
    Assert(IsValidPassword("abcdffaa"));
    Assert(IsValidPassword("ghjaabcc"));
    Assert("abcdffaa" == NextPassword("abcdefgh"));
    Assert("ghjaabcc" == NextPassword("ghijklmn"));

    auto const part1 = NextPassword(GetInput());
    std::println("  Part 1 = {}", part1);
    Assert("hepxxyzz" == part1);

    auto const part2 = NextPassword(part1);
    std::println("  Part 2 = {}", part2);
    Assert("heqaabcc" == part2);
}
