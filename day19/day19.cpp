// day19.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "../common/main.hpp"
#include "../common/intcode.hpp"
#include <iostream>
#include <array>

void Part1()
{
    Int total = 0;
    std::string beam;

    auto out = [&](Int value)
    {
        total += value;

        if (value == 1)
            beam.append(1, '#');
        else
            beam.append(1, '.');
    };

    int a, b;

    auto in = [&]()
    {
        return std::exchange(a, b);
    };

    auto const code = Intcode::ReadFile("19.input");

    for (int y = 0; y != 50; ++y)
    {
        b = y;

        for (int x = 0; x != 50; ++x)
        {
            a = x;
            Intcode::Run(code, in, out);
        }

        beam.append(1, '\n');
    }

    std::cout << "Part1: " << total << '\n';
    //std::cout << beam << '\n';
}

Int Run(std::vector<Int> const &code, Int x, Int y)
{
    auto in = [&]()
    {
        return std::exchange(x, y);
    };
    Int result;
    auto out = [&](Int value)
    {
        result = value;
    };
    Intcode::Run(code, in, out);
    return result;
}

int FindSquare(int width, int height)
{
    auto const code = Intcode::ReadFile("19.input");
    int minx = 0;
    int miny = 10;


    while (true)
    {
        bool valid = true;

        // trouve le depart
        while (Run(code, minx, miny) == 0)
            ++minx;

        for (int x = 1; x != width; ++x)
        {
            if (Run(code, x + minx, miny) == 0)
            {
                valid = false;
                break;
            }
        }

        if (valid)
        {
            for (int y = 1; y != height; ++y)
            {
                if (Run(code, minx, miny - (height - y)) == 0)
                {
                    valid = false;
                    break;
                }
            }

            if (valid)
            {
                for (int x = 1; x != width; ++x)
                {
                    if (Run(code, minx + (width - x), miny - (height - 1)) == 0)
                    {
                        valid = false;
                        break;
                    }
                }

                if (valid)
                {
                    int x = minx;
                    int y = miny - (height - 1);

                    //std::cout << x << ", " << y << '\n';
                    return x * 10000 + y;
                }
            }
        }

        ++miny;
    }

    return 0;
}

void Part2()
{
    std::cout << "Part2: " << FindSquare(100, 100) << '\n';
}

void Main()
{
    std::cout << "Day 19: Tractor Beam\n";
    Part1();
    Part2();
}
