#include "day7.hpp"

#include "../cpp-utils.hpp"
#include "../cpp-utils/intcode.hpp"

static Int Amplify1(std::vector<Int> const &code, std::array<Int, 5> const &sequence)
{
    Int param = 0;

    for (Int s : sequence)
    {
        bool first = true;

        auto out = [&](Int value)
        {
            param = value;
        };

        auto in = [&]()
        {
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

static void Part1()
{
    std::vector<Int> const code = ParseInputNumbers<Int, ','>();
    std::array<Int, 5> sequence{0, 1, 2, 3, 4};
    Int maxValue = std::numeric_limits<Int>::min();

    do
    {
        maxValue = std::max(maxValue, Amplify1(code, sequence));
    }
    while (std::next_permutation(begin(sequence), end(sequence)));

    std::println("  Part1: {}", maxValue);
    Assert(30'940 == maxValue);
}

static Int Amplify2(std::vector<Int> const &code, std::array<Int, 5> const &sequence)
{
    Int param = 0;

    std::array cpus{Intcode{code}, Intcode{code}, Intcode{code}, Intcode{code}, Intcode{code}};

    for (size_t i = 0; i != 5; ++i)
    {
        bool first = true;

        auto in = [&]()
        {
            if (first)
            {
                first = false;
                return sequence.at(i);
            }

            return param;
        };

        param = *cpus.at(i).RunUntilOuput(in);
    }

    auto getParam = [&]()
    {
        return param;
    };

    while (!cpus[4].IsHalted())
    {
        for (auto &cpu : cpus)
        {
            auto v = cpu.RunUntilOuput(getParam);

            if (v.has_value())
            {
                param = *v;
            }
        }
    }

    return param;
}

static void Part2()
{
    std::vector<Int> const code = ParseInputNumbers<Int, ','>();
    std::array<Int, 5> sequence{5, 6, 7, 8, 9};
    Int maxValue = std::numeric_limits<Int>::min();

    do
    {
        maxValue = std::max(maxValue, Amplify2(code, sequence));
    }
    while (std::next_permutation(begin(sequence), end(sequence)));

    std::println("  Part2: {}", maxValue);
    Assert(76'211'147 == maxValue);
}

int main()
{
    std::println("Day 7, 2019: Amplification Circuit");

    Assert(example::N1::result == Amplify1(example::N1::code, example::N1::sequence));
    Assert(example::N2::result == Amplify1(example::N2::code, example::N2::sequence));
    Assert(example::N3::result == Amplify1(example::N3::code, example::N3::sequence));
    Assert(example::N4::result == Amplify2(example::N4::code, example::N4::sequence));
    Assert(example::N5::result == Amplify2(example::N5::code, example::N5::sequence));

    Part1();
    Part2();
}
