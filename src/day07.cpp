#include "day07.hpp"
#include "assert.hpp"
#include "intcode.hpp"
#include "terminal.hpp"
#include <array>
#include <fmt/format.h>
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
    std::vector<Int> const code{input::data};
    std::array<Int, 5> sequence{0, 1, 2, 3, 4};
    Int maxValue = std::numeric_limits<Int>::min();

    do
    {
        maxValue = std::max(maxValue, Amplify1(code, sequence));
    } while (std::next_permutation(begin(sequence), end(sequence)));

    fmt::print("  Part1: {}\n", maxValue);
    Assert(30940 == maxValue);
}

Int Amplify2(std::vector<Int> const &code, std::array<Int, 5> const &sequence)
{
    Int param = 0;

    std::array cpus{Intcode{code}, Intcode{code}, Intcode{code}, Intcode{code}, Intcode{code}};

    for (size_t i = 0; i != 5; ++i)
    {
        bool first = true;

        auto in = [&]() {
            if (first)
            {
                first = false;
                return sequence.at(i);
            }

            return param;
        };

        param = *cpus.at(i).RunUntilOuput(in);
    }

    auto getParam = [&]() {
        return param;
    };

    while (!cpus[4].IsHalted())
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
    std::vector<Int> const code{input::data};
    std::array<Int, 5> sequence{5, 6, 7, 8, 9};
    Int maxValue = std::numeric_limits<Int>::min();

    do
    {
        maxValue = std::max(maxValue, Amplify2(code, sequence));
    } while (std::next_permutation(begin(sequence), end(sequence)));

    fmt::print("  Part2: {}\n", maxValue);
    Assert(76211147 == maxValue);
}

int main()
{
    fmt::print("Day 7: Amplification Circuit\n");

    Assert(example::N1::result == Amplify1(example::N1::code, example::N1::sequence));
    Assert(example::N2::result == Amplify1(example::N2::code, example::N2::sequence));
    Assert(example::N3::result == Amplify1(example::N3::code, example::N3::sequence));
    Assert(example::N4::result == Amplify2(example::N4::code, example::N4::sequence));
    Assert(example::N5::result == Amplify2(example::N5::code, example::N5::sequence));

    Part1();
    Part2();
}
