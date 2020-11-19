#include "day16.hpp"
#include "../assert.hpp"
#include "../terminal.hpp"
#include <algorithm>
#include <array>
#include <fmt/format.h>
#include <fstream>
#include <string>
#ifdef COROUTINE
#    include <experimental/generator>
#endif

static constexpr std::array<int, 4> PATTERN{0, 1, 0, -1};

struct PatternGenerator
{
    int const nb;
    int step = 1;
    decltype(begin(PATTERN)) iter = begin(PATTERN);

    explicit PatternGenerator(int nb_)
        : nb(nb_)
    {
    }

    int next()
    {
        if (step == nb)
        {
            step = 1;

            if (++iter == end(PATTERN))
                iter = begin(PATTERN);
        }
        else
        {
            ++step;
        }

        return *iter;
    }
};

#ifdef COROUTINE
std::experimental::generator<int> GeneratePatternWithRepetitions(int nb)
{
    constexpr std::array PATTERN{0, 1, 0, -1};

    while (true)
        for (int p : PATTERN)
            for (int i = 0; i != nb; ++i)
                co_yield p;
}

template <typename T>
static int next(T &it)
{
    int v = *it;
    ++it;
    return v;
}
#endif

std::string ProcessPhaseSlow(std::string_view numbers)
{
    std::string ret(numbers.size(), '\0');
    int const numCount = static_cast<int>(numbers.size());

    for (int i = 0; i != numCount; ++i)
    {
        PatternGenerator p{i + 1};

        int sum = 0;
        for (char n : numbers)
        {
            sum += (n - '0') * p.next();
        }

        ret[static_cast<size_t>(i)] = static_cast<char>('0' + abs(sum) % 10);
    }

    return ret;
}

static int GetDigit(char c)
{
    return c - '0';
}

std::string Process(std::string_view numbers, int count)
{
    std::string ret{numbers.data(), numbers.size()};

    for (int i = 0; i != count; ++i)
    {
        ret = ProcessPhaseSlow(ret);
    }

    if (ret.size() > 8)
        ret.resize(8);

    return ret;
}

std::string ProcessPhaseCheat(std::string_view numbers)
{
    std::string ret(numbers.size(), '\0');
    char prev = '\0';

    std::transform(rbegin(numbers), rend(numbers), rbegin(ret), [&](char c) -> char {
        if (prev == '\0')
            prev = c;
        else
            prev = static_cast<char>((GetDigit(prev) + GetDigit(c)) % 10 + '0');

        return prev;
    });

    return ret;
}

std::string ProcessWithOffsetTimes10000(std::string_view numbers, int count)
{
    int ncount = static_cast<int>(numbers.size());
    std::string first7{numbers.begin(), numbers.begin() + 7};
    int offset = std::stoi(first7);
    auto r = div(offset, ncount);
    int missing = 10'000 - r.quot;

    std::string ret{numbers.data(), numbers.size()};

    for (int i = 1; i < missing; ++i)
        ret.append(numbers);

    for (int i = 0; i != count; ++i)
        ret = ProcessPhaseCheat(ret);

    return {ret.begin() + r.rem, ret.begin() + r.rem + 8};
}

int main()
{
    fmt::print("Day 16: Flawed Frequency Transmission\n");

    Assert("48226158" == Process("12345678", 1));
    Assert("34040438" == Process("12345678", 2));
    Assert("03415518" == Process("12345678", 3));
    Assert("01029498" == Process("12345678", 4));
    Assert("24176176" == Process("80871224585914546619083218645595", 100));
    Assert("73745418" == Process("19617804207202209144916044189917", 100));
    Assert("52432133" == Process("69317163492948606335995924319873", 100));

    Assert("84462026" == ProcessWithOffsetTimes10000("03036732577212944063491565474664", 100));
    Assert("78725270" == ProcessWithOffsetTimes10000("02935109699940807407585447034323", 100));
    Assert("53553731" == ProcessWithOffsetTimes10000("03081770884921959731165446850517", 100));

    auto part1 = Process(input::data, 100);
    fmt::print("  Part1: {}\n", part1);
    Assert("88323090" == part1);

    auto part2 = ProcessWithOffsetTimes10000(input::data, 100);
    fmt::print("  Part2: {}\n", part2);
    Assert("50077964" == part2);
}
