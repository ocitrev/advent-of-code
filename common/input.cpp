#include "pch.h"
#include "input.hpp"
#include "main.hpp"

#include <fstream>
#include <charconv>
#include <string>

std::filesystem::path GetInputsPath()
{
    return std::filesystem::absolute(GetExePath().parent_path() / L"..\\..\\inputs");
}

std::vector<int> ReadInts(std::filesystem::path const &filepath, char sep)
{
    std::ifstream input(filepath);
    std::vector<int> results;

    for (std::string line; std::getline(input, line, sep); )
    {
        int value;
        auto result = std::from_chars(line.data(), line.data() + line.size(), value, 10);

        if (result.ec == std::errc{})
            results.push_back(value);
    }

    return results;
}