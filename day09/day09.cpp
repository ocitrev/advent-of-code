// day09.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "../common/main.hpp"
#include "../common/intcode.hpp"
#include "../common/input.hpp"
#include <iostream>
#include <cassert>

std::vector<Int> Run(std::vector<Int> code, Int input=0)
{
    std::vector<Int> result;

    auto in = [input]()
    {
        return input;
    };

    auto out = [&result](Int value)
    {
        result.push_back(value);
    };

    Intcode::Run(std::move(code), in, out);
    return result;
}

void Main()
{
    std::cout << "Day 9: Sensor Boost\n";

    assert((std::vector<Int>{109,1,204,-1,1001,100,1,100,1008,100,16,101,1006,101,0,99}
        == Run({109,1,204,-1,1001,100,1,100,1008,100,16,101,1006,101,0,99})));
    assert(std::vector<Int>{1219070632396864} == Run({1102, 34915192, 34915192, 7, 4, 7, 99, 0}));
    assert(std::vector<Int>{1125899906842624} == Run({104, 1125899906842624, 99}));

    std::vector<Int> const code = Intcode::ReadFile("9.input");

    std::cout << "Part1: " << Run(code, 1).front() << '\n';
    std::cout << "Part2: " << Run(code, 2).front() << '\n';
}
