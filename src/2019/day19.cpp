#include "day19.hpp"

#include "../common.hpp"
#include "../common/intcode.hpp"

static void Part1()
{
    Int total = 0;
    std::string beam;

    auto out = [&](Int value)
    {
        total += value;

        if (value == 1)
        {
            beam.append(1, '#');
        }
        else
        {
            beam.append(1, '.');
        }
    };

    Int a = 0;
    Int b = 0;
    auto in = [&]()
    {
        return std::exchange(a, b);
    };

    std::vector<Int> const code = ParseInputNumbers<Int, ','>();

    for (Int y = 0; y != 50; ++y)
    {
        b = y;

        for (Int x = 0; x != 50; ++x)
        {
            a = x;
            Intcode::Run(code, in, out);
        }

        beam.append(1, '\n');
    }

    std::print("  Part1: {}\n", total);
    Assert(231 == total);

    if constexpr (false)
    {
        std::print("{}\n", beam);
    }
}

static Int Run(std::vector<Int> const &code, Int x, Int y)
{
    auto in = [&]()
    {
        return std::exchange(x, y);
    };

    Int result = 0;
    auto out = [&](Int value)
    {
        result = value;
    };

    Intcode::Run(code, in, out);
    return result;
}

static Int FindSquare(Int width, Int height)
{
    std::vector<Int> const code = ParseInputNumbers<Int, ','>();
    Int minx = 0;
    Int miny = 10;

    for (;; ++miny)
    {
        // trouve le depart
        while (Run(code, minx, miny) == 0)
        {
            ++minx;
        }

        for (Int x = 1; x != width; ++x)
        {
            if (Run(code, x + minx, miny) == 0)
            {
                goto next;
            }
        }

        for (Int y = 1; y != height; ++y)
        {
            if (Run(code, minx, miny - (height - y)) == 0)
            {
                goto next;
            }
        }

        for (Int x = 1; x != width; ++x)
        {
            if (Run(code, minx + (width - x), miny - (height - 1)) == 0)
            {
                goto next;
            }
        }

        break;
    next:;
    }

    Int x = minx;
    Int y = miny - (height - 1);

    // std::print("{}, {}\n", x, y);
    return x * 10'000 + y;
}

static void Part2()
{
    auto const part2 = FindSquare(100, 100);
    std::print("  Part2: {}\n", part2);
    Assert(9'210'745 == part2);
}

int main()
{
    std::print("Day 19, 2019: Tractor Beam\n");
    Part1();
    Part2();
}
