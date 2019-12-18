// day02.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "../common/input.hpp"
#include "../common/intcode.hpp"
#include "../common/main.hpp"
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int RunAndGetValue(std::vector<int> const &code, int offset)
{
    Intcode a{code};
    a.Run();
    return a.ReadMemory(offset);
}

int Run(std::vector<int> const &code, int noun, int verb)
{
    Intcode a{code};
    a.WriteMemory(1, noun);
    a.WriteMemory(2, verb);
    a.Run();
    return a.ReadMemory(0);
}

void Part1(std::vector<int> const &input)
{
    std::cout << "Part1: " << Run(input, 12, 2) << '\n';
}

void Part2(std::vector<int> const &input)
{
    for (int noun = 0; noun != 100; ++noun)
    {
        for (int verb = 0; verb != 100; ++verb)
        {
            if (Run(input, noun, verb) == 19690720)
            {
                std::cout << "Part2: " << noun * 100 + verb << '\n';
                return;
            }
        }
    }


}

void Main()
{
    assert(2 == RunAndGetValue({1, 0, 0, 0, 99}, 0));
    assert(6 == RunAndGetValue({2, 3, 0, 3, 99}, 3));
    assert(9801 == RunAndGetValue({2, 4, 4, 5, 99, 0}, 5));
    assert(30 == RunAndGetValue({1, 1, 1, 4, 99, 5, 6, 0, 99}, 0));

    std::vector const input = ReadInts(GetInputsPath() / L"2.input", ',');
    Part1(input);
    Part2(input);
}
