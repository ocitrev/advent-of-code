// day16.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "../common/main.hpp"
#include "../common/input.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <experimental/generator>
#include <cassert>
#include <charconv>

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

std::string ProcessPhaseSlow(std::string const &numbers)
{
    std::string ret(numbers.size(), '\0');

    for (int i = 0; i != numbers.size(); ++i)
    {
        auto &&g = GeneratePatternWithRepetitions(i + 1);
        auto it = begin(g);
        // skip first
        ++it;

        int sum = 0;
        for (char n : numbers)
        {
            sum += (n - '0') * next(it);
        }

        ret[i] = abs(sum) % 10 + '0';
    }

    return ret;
}

static int GetDigit(char c)
{
    return c - '0';
}

std::string Process(std::string const &numbers, int count)
{
    std::string ret = numbers;

    for (int i = 0; i != count; ++i)
    {
        ret = ProcessPhaseSlow(ret);
    }

    if (ret.size() > 8)
        ret.resize(8);

    return ret;
}

std::string ProcessPhaseCheat(std::string const &numbers)
{
    std::string ret(numbers.size(), '\0');
    char prev = '\0';

    std::transform(rbegin(numbers), rend(numbers), rbegin(ret), [&](char c) -> char
    {
        if (prev == '\0')
            prev = c;
        else
            prev = (GetDigit(prev) + GetDigit(c)) % 10 + '0';

        return prev;
    });

    return ret;
}

std::string ProcessWithOffsetTimes10000(std::string const &numbers, int count)
{
    int ncount = static_cast<int>(numbers.size());
    std::string first7{numbers.begin(), numbers.begin() + 7};
    int offset;
    auto result = std::from_chars(numbers.data(), numbers.data() + 7, offset);
    assert(result.ec == std::errc{});
    
    auto r = div(offset, ncount);
    int missing = 10'000 - r.quot;
    
    std::string ret = numbers;
    
    for (int i = 1; i < missing; ++i)
        ret.append(numbers);

    for (int i = 0; i != count; ++i)
        ret = ProcessPhaseCheat(ret);

    return {ret.begin() + r.rem, ret.begin() + r.rem + 8};
}

void Main()
{
    assert("48226158" == Process("12345678", 1));
    assert("34040438" == Process("12345678", 2));
    assert("03415518" == Process("12345678", 3));
    assert("01029498" == Process("12345678", 4));
    assert("24176176" == Process("80871224585914546619083218645595", 100));
    assert("73745418" == Process("19617804207202209144916044189917", 100));
    assert("52432133" == Process("69317163492948606335995924319873", 100));

    assert("84462026" == ProcessWithOffsetTimes10000("03036732577212944063491565474664", 100));
    assert("78725270" == ProcessWithOffsetTimes10000("02935109699940807407585447034323", 100));
    assert("53553731" == ProcessWithOffsetTimes10000("03081770884921959731165446850517", 100));

    std::string inputText = ReadAllText(GetInputsPath() / L"16.input");
    
    std::cout << "Part1: " << Process(inputText, 100) << '\n';
    std::cout << "Part2: " << ProcessWithOffsetTimes10000(inputText, 100) << '\n';
}
