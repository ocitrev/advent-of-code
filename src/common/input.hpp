#pragma once
#include <fmt/format.h>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <string>

inline std::string ReadInput(int day)
{
    std::ifstream input(fmt::format("../inputs/2022/day{}.txt", day), std::ios_base::ate);
    if (input.bad())
        throw std::invalid_argument("Failed to open file");

    std::string result;
    result.reserve(static_cast<size_t>(input.tellg()));
    input.seekg(0, std::ios::beg);
    result.assign(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>());
    return result;
}
