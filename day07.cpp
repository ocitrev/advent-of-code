#include "day07.hpp"
#include "common/intcode.hpp"
#include "common/main.hpp"
#include <array>
#include <cassert>
#include <iostream>
#include <vector>

Int Amplify1(std::vector<Int> const &code, std::array<Int, 5> const &sequence)
{
    Int param = 0;

    for (Int s : sequence)
    {
        bool first = true;

        auto out = [&](Int value) {
            param = value;
        };

        auto in = [&]() {
            if (first)
            {
                first = false;
                return s;
            }

            return param;
        };

        Intcode::Run(code, in, out);
    }

    return param;
}

void Part1()
{
    std::vector<Int> const code{begin(input::data), end(input::data)};
    std::array<Int, 5> sequence{0, 1, 2, 3, 4};
    Int maxValue = std::numeric_limits<Int>::min();

    do
    {
        maxValue = std::max(maxValue, Amplify1(code, sequence));
    } while (std::next_permutation(begin(sequence), end(sequence)));

    std::cout << "  Part1: " << maxValue << '\n';
}

Int Amplify2(std::vector<Int> const &code, std::array<Int, 5> const &sequence)
{
    Int param = 0;

    Intcode cpus[]{Intcode{code}, Intcode{code}, Intcode{code}, Intcode{code}, Intcode{code}};

    for (size_t i = 0; i != 5; ++i)
    {
        bool first = true;

        auto in = [&]() {
            if (first)
            {
                first = false;
                return sequence[i];
            }

            return param;
        };

        param = *cpus[i].RunUntilOuput(in);
    }

    auto getParam = [&]() {
        return param;
    };

    while (cpus[4].IsHalted() == false)
    {
        for (auto &cpu : cpus)
        {
            auto v = cpu.RunUntilOuput(getParam);

            if (v.has_value())
                param = *v;
        }
    }

    return param;
}

void Part2()
{
    std::vector<Int> const code{begin(input::data), end(input::data)};
    std::array<Int, 5> sequence{5, 6, 7, 8, 9};
    Int maxValue = std::numeric_limits<Int>::min();

    do
    {
        maxValue = std::max(maxValue, Amplify2(code, sequence));
    } while (std::next_permutation(begin(sequence), end(sequence)));

    std::cout << "  Part2: " << maxValue << '\n';
}

void Main()
{
    std::cout << "Day 7: Amplification Circuit\n";

    assert(43210
           == Amplify1({3, 15, 3, 16, 1002, 16, 10, 16, 1, 16, 15, 15, 4, 15, 99, 0, 0},
                       {4, 3, 2, 1, 0}));
    assert(54321
           == Amplify1({3, 23, 3,  24, 1002, 24, 10, 24, 1002, 23, -1, 23, 101,
                        5, 23, 23, 1,  24,   23, 23, 4,  23,   99, 0,  0},
                       {0, 1, 2, 3, 4}));
    assert(65210
           == Amplify1({3,  31, 3,  32, 1002, 32, 10, 32, 1001, 31, -2, 31, 1007, 31, 0, 33, 1002,
                        33, 7,  33, 1,  33,   31, 31, 1,  32,   31, 31, 4,  31,   99, 0, 0,  0},
                       {1, 0, 4, 3, 2}));

    assert(139629729
           == Amplify2({3,  26, 1001, 26,   -4, 26, 3,  27,   1002, 27, 2,  27, 1, 27, 26,
                        27, 4,  27,   1001, 28, -1, 28, 1005, 28,   6,  99, 0,  0, 5},
                       {9, 8, 7, 6, 5}));
    assert(18216
           == Amplify2({3,    52,   1001, 52,   -5,   52, 3,  53, 1,    52, 56, 54, 1007, 54, 5,
                        55,   1005, 55,   26,   1001, 54, -5, 54, 1105, 1,  12, 1,  53,   54, 53,
                        1008, 54,   0,    55,   1001, 55, 1,  55, 2,    53, 55, 53, 4,    53, 1001,
                        56,   -1,   56,   1005, 56,   6,  99, 0,  0,    0,  0,  10},
                       {9, 7, 8, 5, 6}));

    Part1();
    Part2();
}
